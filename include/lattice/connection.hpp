//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket connection.
 */

#pragma once

#include "adapter.hpp"
#include "certificate.hpp"
#include "dns.hpp"
#include "exception.hpp"
#include "method.hpp"
#include "timeout.hpp"

#include <cstdlib>
#include <cstring>


namespace lattice
{
// CONSTANTS
// ---------

static const size_t BUFFER_SIZE = 8092;


// FUNCTION
// --------


/** \brief Open connection without a cache.
 */
template <typename Adapter>
void openConnection(Adapter &adapter,
    const std::string &host,
    const std::string &service)
{
    // perform DNS lookup
    for (auto &&info: DnsLookup(host, service)) {
        if (adapter.open(info)) {
            return;
        }
    }

    // no suitable addresses found
    throw ConnectionError();
}


/** \brief Open connection with DNS cache.
 */
template <typename Adapter>
void openConnection(Adapter &adapter,
    const std::string &host,
    const std::string &service,
    Cache &cache)
{
    // try cached results
    Cache::iterator it;
    if ((it = cache.find(host)) != cache.end()) {
        if (adapter.open(addrinfo(it->second))) {
            return;
        }
    }

    // perform DNS lookup
    for (auto &&info: DnsLookup(host, service)) {
        if (adapter.open(info)) {
            cache.emplace(host, info);
            return;
        }
    }

    throw ConnectionError();
}


// OBJECTS
// -------


/** \brief Socket connection.
 *
 *  Establish and maintain connection over socket.
 */
template <typename Adapter>
class Connection
{
protected:
    Adapter adapter;
    DnsCache cache = nullptr;

public:
    Connection();
    Connection(const Connection &other) = delete;
    ~Connection();

    // REQUESTS
    void open(const Url &url);
    void close();
    void setTimeout(const Timeout &timeout);
    void setCertificateFile(const CertificateFile &certificate);
    void setSslProtocol(const SslProtocol ssl);
    void setCache(const DnsCache &cache);
    void send(const std::string &data);

    // RESPONSE
    std::string headers();
    std::string chunked();
    std::string body(const long length);
    std::string read();
};


// IMPLEMENTATION
// --------------


/** \brief Null constructor.
 */
template <typename Adapter>
Connection<Adapter>::Connection()
{}


/** \brief Destructor.
 */
template <typename Adapter>
Connection<Adapter>::~Connection()
{
    close();
}


/** \brief Open connection.
 */
template <typename Adapter>
void Connection<Adapter>::open(const Url &url)
{
    if (cache) {
        openConnection(adapter, url.host(), url.service(), *cache);
    } else {
        openConnection(adapter, url.host(), url.service());
    }
}


/** \brief Close connection.
 */
template <typename Adapter>
void Connection<Adapter>::close()
{
    adapter.close();
}


/** \brief Set timeout for socket requests.
 */
template <typename Adapter>
void Connection<Adapter>::setTimeout(const Timeout &timeout)
{
    adapter.setTimeout(timeout);
}


/** \brief Set certificate file for SSL/TLS.
 */
template <typename Adapter>
void Connection<Adapter>::setCertificateFile(const CertificateFile &certificate)
{
    adapter.setCertificateFile(certificate);
}


/** \brief Set SSL protocol.
 */
template <typename Adapter>
void Connection<Adapter>::setSslProtocol(const SslProtocol ssl)
{
    adapter.setSslProtocol(ssl);
}


/** \brief Set DNS cache.
 */
template <typename Adapter>
void Connection<Adapter>::setCache(const DnsCache &cache)
{
    this->cache = cache;
}


/** \brief Send data through socket.
 */
template <typename Adapter>
void Connection<Adapter>::send(const std::string &data)
{
    int sent = adapter.write(data.data(), data.size());
    if (sent != static_cast<int>(data.size())) {
        throw RequestError(sent, data.size());
    }
}


/** \brief Read headers data from server.
 *
 *  Slowly read from buffer until a double carriage return is found.
 */
template <typename Adapter>
std::string Connection<Adapter>::headers()
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


/** \brief Read chunked transfer encoding.
 *
 *  Each message is prefixed with a single line denoting how
 *  long the message is, in hex.
 */
template <typename Adapter>
std::string Connection<Adapter>::chunked()
{
    // initialize alloc
    std::string hex;
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(offset));
    char byte, *src = buffer + offset;

    while ((result = adapter.read(&byte, 1))) {
        if (!(byte == '\r' || byte == '\n')) {
            hex += byte;
        } else if (hex.size()) {
            // get carriage return
            result = adapter.read(&byte, 1);

            // read our bytes
            long bytes = std::strtoul(hex.data(), nullptr, 16);
            buffer = static_cast<char*>(realloc(buffer, bytes + offset));
            offset += adapter.read(buffer, bytes);
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


/** \brief Read non-chunked content of fixed length.
 */
template <typename Adapter>
std::string Connection<Adapter>::body(const long length)
{
    std::string string;
    string.resize(length);
    adapter.read(const_cast<char*>(&string[0]), length);

    return string;
}


/** \brief Read non-chunked content of unknown length.
 */
template <typename Adapter>
std::string Connection<Adapter>::read()
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


}   /* lattice */
