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

#include <cstring>
#include <iostream>         // TODO: remove


namespace lattice
{
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


/** \brief Null constructor.
 */
Connection::Connection()
{}


/** \brief Initializer list constructor.
 */
Connection::Connection(const std::string &host,
        const std::string &service)
{
    open(host, service);
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
    const std::string &service)
{
    address.open(host, service);
    for (const auto &item: address) {
        sock = ::socket(item.ai_family, item.ai_socktype, item.ai_protocol);
        if (sock < 0) {
            continue;
        }
        if (::connect(sock, item.ai_addr, item.ai_addrlen) >= 0) {
            return;
        }
        ::close(sock);
    }

    // no suitable addresses found
    throw ConnectionError();
}


/** \brief Close active connection and address lookup.
 */
void Connection::close()
{
    address.close();
    ::close(sock);
    sock = -1;
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

    // TODO: this needs to be a decent buffer length...
    // TODO: need to then read from it...
    // TODO: remove....
    // 8192 for the buffer size....
    char cur;
    while (read(sock, &cur, 1) > 0 ) {
        std::cout << cur;
    }
}

}   /* lattice */

#endif              // WIN32
