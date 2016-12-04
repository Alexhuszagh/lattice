//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Unix networking protocols.
 */

#ifndef _WIN32

#include "lattice.hpp"

#include <sys/time.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>


namespace lattice
{
// CONSTANTS
// ---------

static const size_t BUFFER_SIZE = 8092;

// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
AddressIterator::AddressIterator(value_type *value):
    value(value)
{}


/** \brief Copy constructor.
 */
AddressIterator::AddressIterator(const AddressIterator &other):
    value(other.value)
{}


/** \brief Dereference iterator.
 */
auto AddressIterator::operator*() const
    -> const value_type &
{
    return *value;
}


/** \brief Get pointer from iterator.
 */
auto AddressIterator::operator->() const
    -> const value_type *
{
    return value;
}


/** \brief Pre-increment iterator.
 */
auto AddressIterator::operator++()
    -> AddressIterator &
{
    value = value->ai_next;
    return *this;
}


/** \brief Post-increment iterator.
 */
auto AddressIterator::operator++(int)
    -> AddressIterator
{
    AddressIterator copy(*this);
    ++(*this);

    return copy;
}


/** \brief Equality operator.
 */
bool AddressIterator::operator==(const AddressIterator& other) const
{
    short sum = bool(value) + bool(other.value);
    switch (sum) {
        case 0:
            return true;
        case 1:
            return false;
        case 2:
            /* fallthrough */
        default: {
            return (
                value->ai_flags == other.value->ai_flags &&
                value->ai_family == other.value->ai_family &&
                value->ai_socktype == other.value->ai_socktype &&
                value->ai_protocol == other.value->ai_protocol &&
                value->ai_addrlen == other.value->ai_addrlen &&
                value->ai_addr == other.value->ai_addr &&
                value->ai_canonname == other.value->ai_canonname
            );
        }
    }
}


/** \brief Inequality operator.
 */
bool AddressIterator::operator!=(const AddressIterator& other) const
{
    return !operator==(other);
}


/** \brief Null constructor.
 */
Address::Address()
{}


/** \brief Initializer list constructor.
 */
Address::Address(const std::string &host,
    const std::string &service)
{
    open(host, service);
}


/** \brief Perform address lookup.
 */
void Address::open(const std::string &host,
    const std::string &service)
{
    // initialize our hints
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.data(), service.data(), &hints, &result)) {
        throw UnixAddressError(host, service);
    }

    info = result;
}


/** \brief Close and free address info.
 */
void Address::close()
{
    if (info) {
        freeaddrinfo(info);
        info = nullptr;
    }
}


/** \brief Destructor.
 */
Address::~Address()
{
    close();
}


/** \brief Get iterator at first address.
 */
AddressIterator Address::begin() const
{
    return AddressIterator(info);
}


/** \brief Get iterator past end of addresses.
 */
AddressIterator Address::end() const
{
    return AddressIterator(nullptr);
}


/** \brief Attempt to open an address.
 */
bool Connection::openAddress(const AddressInfo &info)
{
    sock = ::socket(info.family, info.socket_type, info.protocol);
    if (sock < 0) {
        return false;
    }
    if (::connect(sock, &info.address, info.length) >= 0) {
        return true;
    }

    ::close(sock);
    return false;
}


/** \brief Null constructor.
 */
Connection::Connection()
{}


/** \brief Initializer list constructor.
 */
Connection::Connection(const std::string &host,
        const std::string &service,
        DnsCache *cache)
{
    open(host, service, cache);
}


/** \brief Destructor.
 */
Connection::~Connection()
{
    close();
}


/** \brief Open connection to server.
 */
void Connection::open(const std::string &host,
    const std::string &service,
    DnsCache *cache)
{
    // try cached results
    DnsCache::iterator it;
    if (cache && (it = cache->find(host)) != cache->end()) {
        if (openAddress(AddressInfo(it->second))) {
            return;
        }
    }

    // perform DNS lookup
    address.open(host, service);
    for (const AddressInfo info: address) {
        if (openAddress(info)) {
            // cache result
            if (cache) {
                cache->emplace(host, info);
            }
            return;
        }
    }

    // no suitable addresses found
    throw ConnectionError();
}


/** \brief Close active connection and address lookup.
 */
void Connection::close()
{
    address.close();
    if (sock >= 0) {
        ::close(sock);
        sock = -1;
    }
}


/** \brief Set a symmetric max timeout for requests.
 */
void Connection::setTimeout(const Timeout &timeout)
{
    setTimeout(timeout, false);
    setTimeout(timeout, true);
}


/** \brief Set a maximum timeout for requests.
 *
 *  Separates both input and output requests, in case asymmetric timeouts
 *  wish to be set.
 */
void Connection::setTimeout(const Timeout &timeout,
    const bool input)
{
    // create timeout, do not set microseconds as it causes errors
    struct timeval value;
    value.tv_sec = timeout.seconds();
    value.tv_usec = 0;

    // set options
    int name = input ? SO_RCVTIMEO : SO_SNDTIMEO;
    char *option = reinterpret_cast<char*>(&value);
    socklen_t size = sizeof(timeval);
    if (::setsockopt(sock, SOL_SOCKET, name, option, size)) {
        throw SocketOptionError();
    }
}


/** \brief Send request to server.
 */
void Connection::send(const std::string &request)
{
    int sent = ::send(sock, request.data(), request.size(), 0);
    if (sent != static_cast<int>(request.size())) {
        throw RequestError(sent, request.size());
    }
}


/** \brief Read headers data from server.
 *
 *  Slowly read from buffer until a double carriage return is found.
 */
std::string Connection::headers()
{
    std::string string;
    int result;
    char src;
    while ((result = ::read(sock, &src, 1))) {
        string += src;
        size_t size = string.size() - 4;
        if (size > 0 && src == '\n' && string.substr(size) == "\r\n\r\n") {
            break;
        }
    }

    return string;
}


/** \brief Read chunked transfer encoding.
 *
 *  Each message is prefixed with a single line denoting how
 *  long the message is, in hex.
 */
std::string Connection::chunked()
{
    // initialize alloc
    std::string hex;
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(offset));
    char byte, *src = buffer + offset;

    while ((result = ::read(sock, &byte, 1))) {
        if (!(byte == '\r' || byte == '\n')) {
            hex += byte;
        } else if (hex.size()) {
            // get carriage return
            result = ::read(sock, &byte, 1);

            // read our bytes
            long bytes = std::strtoul(hex.data(), nullptr, 16);
            buffer = static_cast<char*>(realloc(buffer, bytes + offset));
            offset += ::read(sock, buffer, bytes);
            src = buffer + offset;

            // clear our hex buffer for new messages
            hex.clear();
        } else {
            // trailing line
            break;
        }
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


/** \brief Read content of fixed length.
 */
std::string Connection::body(const long length)
{
    std::string string;
    string.resize(length);
    ::read(sock, const_cast<char*>(&string[0]), length);

    return string;
}


/** \brief Read non-chunked content of unknown length.
 */
std::string Connection::read()
{
    // read from connection
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(BUFFER_SIZE));
    char *src = buffer + offset;
    while ((result = ::read(sock, src, BUFFER_SIZE))) {
        offset += result;
        buffer = static_cast<char*>(realloc(buffer, BUFFER_SIZE + offset));
        src = buffer + offset;
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


}   /* lattice */

#endif              // WIN32
