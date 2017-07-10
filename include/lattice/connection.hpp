//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket connection.
 */

#pragma once

#include <lattice/adapter.hpp>
#include <lattice/dns.hpp>
#include <lattice/method.hpp>
#include <lattice/ssl.hpp>
#include <lattice/timeout.hpp>
#include <lattice/util.hpp>

#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4267)
#endif


namespace lattice
{
// CONSTANTS
// ---------

static constexpr size_t BUFFER_SIZE = 8092;

// FUNCTION
// --------


/**
 *  \brief Open connection without a cache.
 */
template <typename Adapter>
void open_connection(Adapter& adapter, const std::string& host, const std::string& service)
{
    // perform DNS lookup
    for (auto &&info: dns_lookup_t(host, service)) {
        if (adapter.open(info, host)) {
            return;
        }
    }

    // no suitable addresses found
    throw std::runtime_error("Unable to establish a connection.");
}


/**
 *  \brief Open connection with DNS cache.
 */
template <typename Adapter>
void open_connection(Adapter& adapter,
    const std::string& host,
    const std::string& service,
    address_cache_t& cache)
{
    // try cached results
    typename address_cache_t::iterator it;
    if ((it = cache.find(host)) != cache.end()) {
        if (adapter.open(addrinfo(it->second), host)) {
            return;
        }
    }

    // perform DNS lookup
    for (auto &&info: dns_lookup_t(host, service)) {
        if (adapter.open(info, host)) {
            cache.emplace(host, info);
            return;
        }
    }

    throw std::runtime_error("Unable to establish a connection.");
}


// OBJECTS
// -------


/**
 *  \brief Socket connection.
 *
 *  Establish and maintain connection over socket.
 */
template <typename Adapter>
class connection_t
{
protected:
    Adapter adapter;
    dns_cache_t cache = nullptr;

    long readn(char *dst, long bytes);

public:
    connection_t();
    connection_t(const connection_t&) = delete;
    connection_t & operator=(const connection_t&) = delete;
    connection_t(connection_t&&) = default;
    connection_t & operator=(connection_t&&) = default;
    ~connection_t();

    // REQUESTS
    void open(const Url& url);
    void close();
    void write(const std::string& data);
    void set_cache(const dns_cache_t& cache);

    // RESPONSE
    std::string headers();
    std::string chunked();
    std::string body(const long length);
    std::string read();

    // OPTIONAL
    template <typename T = Adapter>
    typename std::enable_if<(has_set_timeout<T>::value), void>::type
    set_timeout(const timeout_t& timeout);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_timeout<T>::value), void>::type
    set_timeout(const timeout_t& timeout);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_certificate_file<T>::value), void>::type
    set_certificate_file(const CertificateFile& certificate);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_certificate_file<T>::value), void>::type
    set_certificate_file(const CertificateFile& certificate);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_revocation_lists<T>::value), void>::type
    set_revocation_lists(const RevocationLists& revoke);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_revocation_lists<T>::value), void>::type
    set_revocation_lists(const RevocationLists& revoke);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_ssl_protocol<T>::value), void>::type
    set_ssl_protocol(ssl_protocol_t ssl);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_ssl_protocol<T>::value), void>::type
    set_ssl_protocol(ssl_protocol_t ssl);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_verify_peer<T>::value), void>::type
    set_verify_peer(const verify_peer_t& peer);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_verify_peer<T>::value), void>::type
    set_verify_peer(const verify_peer_t& peer);
};


// IMPLEMENTATION
// --------------


/**
 *  Sockets guarantee at least 1 byte will be read, while valid, but do
 *  not guarantee N-bytes will be successfully read. Read until all
 *  data have been extracted.
 */
template <typename Adapter>
long connection_t<Adapter>::readn(char *dst, long bytes)
{
    long count = 0;
    while (bytes) {
        long read = adapter.read(dst, bytes);
        if (!read) {
            return count;
        }
        bytes -= read;
        dst += read;
        count += read;
    }

    return count;
}


template <typename Adapter>
connection_t<Adapter>::connection_t()
{}


template <typename Adapter>
connection_t<Adapter>::~connection_t()
{
    close();
}


template <typename Adapter>
void connection_t<Adapter>::open(const Url& url)
{
    if (cache) {
        open_connection(adapter, url.host(), url.service(), *cache);
    } else {
        open_connection(adapter, url.host(), url.service());
    }
}


template <typename Adapter>
void connection_t<Adapter>::close()
{
    adapter.close();
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_timeout<T>::value), void>::type
connection_t<Adapter>::set_timeout(const timeout_t& timeout)
{
    adapter.set_timeout(timeout);
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_timeout<T>::value), void>::type
connection_t<Adapter>::set_timeout(const timeout_t& timeout)
{}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_certificate_file<T>::value), void>::type
connection_t<Adapter>::set_certificate_file(const CertificateFile& certificate)
{
    adapter.set_certificate_file(certificate);
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_certificate_file<T>::value), void>::type
connection_t<Adapter>::set_certificate_file(const CertificateFile& certificate)
{}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_revocation_lists<T>::value), void>::type
connection_t<Adapter>::set_revocation_lists(const RevocationLists& revoke)
{
    adapter.set_revocation_lists(revoke);
}


/**
 *  \brief Set file to manually revoke certificates (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_revocation_lists<T>::value), void>::type
connection_t<Adapter>::set_revocation_lists(const RevocationLists& revoke)
{}


/**
 *  \brief Set SSL protocol.
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_ssl_protocol<T>::value), void>::type
connection_t<Adapter>::set_ssl_protocol(ssl_protocol_t ssl)
{
    adapter.set_ssl_protocol(ssl);
}


/**
 *  \brief Set SSL protocol (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_ssl_protocol<T>::value), void>::type
connection_t<Adapter>::set_ssl_protocol(ssl_protocol_t ssl)
{}


/**
 *  \brief Change peer certificate validation.
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_verify_peer<T>::value), void>::type
connection_t<Adapter>::set_verify_peer(const verify_peer_t& peer)
{
    adapter.set_verify_peer(peer);
}


/**
 *  \brief Change peer certificate validation (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_verify_peer<T>::value), void>::type
connection_t<Adapter>::set_verify_peer(const verify_peer_t& peer)
{}


/**
 *  \brief Set DNS cache.
 */
template <typename Adapter>
void connection_t<Adapter>::set_cache(const dns_cache_t& cache)
{
    this->cache = cache;
}


/**
 *  \brief Send data through socket.
 */
template <typename Adapter>
void connection_t<Adapter>::write(const std::string& data)
{
    int sent = adapter.write(data.data(), data.size());
    if (sent != static_cast<int>(data.size())) {
        throw std::runtime_error("Unable to make request, sent " + std::to_string(sent) + " bytes.");
    }
}


/**
 *  \brief Read headers data from server.
 *
 *  Slowly read from buffer until a double carriage return is found.
 */
template <typename Adapter>
std::string connection_t<Adapter>::headers()
{
    std::string string;
    int result;
    char src;
    while ((result = adapter.read(&src, 1))) {
        string += src;
        size_t size = string.size() - 4;
        if (size > 0 && src == '\n' && string.substr(size) == "\r\n\r\n") {
            break;
        }
    }

    return string;
}


/**
 *  \brief Read chunked transfer encoding.
 *
 *  Each message is prefixed with a single line denoting how
 *  long the message is, in hex.
 */
template <typename Adapter>
std::string connection_t<Adapter>::chunked()
{
    // initialize alloc
    std::string hex;
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(offset));
    char byte, *src = buffer + offset;

    while ((result = adapter.read(&byte, 1))) {
        if (!(byte == '\r' || byte == '\n')) {
            hex += byte;
        } else if (hex == "0") {
            // end of file
            break;
        } else if (hex.size()) {
            // get carriage return
            result = adapter.read(&byte, 1);

            // read bytes
            long bytes = std::strtoul(hex.data(), nullptr, 16);
            buffer = static_cast<char*>(realloc(buffer, bytes + offset));
            long read = readn(buffer + offset, bytes);
            offset += read;
            if (read != bytes) {
                break;
            }

            // clear our hex buffer for new messages
            hex.clear();
        }
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


/**
 *  \brief Read non-chunked content of fixed length.
 */
template <typename Adapter>
std::string connection_t<Adapter>::body(const long length)
{
    std::string string;
    if (length > 0) {
        string.resize(length);
        adapter.read(const_cast<char*>(&string[0]), length);
    } else if (length) {
        throw std::runtime_error("Asked to read negative bytes.");
    }

    return string;
}


/**
 *  \brief Read non-chunked content of unknown length.
 */
template <typename Adapter>
std::string connection_t<Adapter>::read()
{
    // read from connection
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(BUFFER_SIZE));
    char *src = buffer + offset;
    while ((result = adapter.read(src, BUFFER_SIZE))) {
        offset += result;
        buffer = static_cast<char*>(realloc(buffer, BUFFER_SIZE + offset));
        src = buffer + offset;
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


// TYPES
// -----

typedef connection_t<http_adaptor_t> http_connection_t;
typedef connection_t<ssl_adaptor_t> https_connection_t;

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
