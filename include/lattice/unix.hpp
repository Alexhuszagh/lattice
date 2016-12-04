//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Unix networking protocols.
 */

#pragma once

#ifndef _WIN32

#include "cache.hpp"
#include "exception.hpp"
#include "timeout.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iterator>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Error in in getting an address from the host/service.
 */
class UnixAddressError: public AddressError
{
public:
    using AddressError::AddressError;

    /** \brief Display message from address lookup error.
     */
    virtual const char * what() const throw()
    {
        std::stringstream stream;
        stream << "Unable to get address from getaddrinfo("
               << host.data() << ", " << service.data() << ")\n";

        return stream.str().data();
    }
};


/** \brief Iterator over addresses.
 */
class AddressIterator: public std::iterator<
        std::forward_iterator_tag,
        addrinfo*
    >
{
protected:
    typedef addrinfo value_type;

    value_type *value = nullptr;

public:
    AddressIterator(value_type *value = nullptr);
    AddressIterator(const AddressIterator &other);

    const value_type & operator*() const;
    const value_type * operator->() const;
    AddressIterator & operator++();
    AddressIterator operator++(int);
    bool operator==(const AddressIterator& other) const;
    bool operator!=(const AddressIterator& other) const;
};


/** \brief Address for a server host.
 *
 *  \param host             Base url of host address, "example.com"
 *  \param service          Service for connection, "http"
 */
class Address
{
protected:
    addrinfo *info = nullptr;

public:
    Address();
    Address(const std::string &host,
        const std::string &service = "http");
    Address(const Address &other) = delete;
    ~Address();

    void open(const std::string &host,
        const std::string &service = "http");
    void close();

    // ITERATORS
    AddressIterator begin() const;
    AddressIterator end() const;
};


/** \brief Server connection.
 *
 *  Establish connection to server and make request.
 */
class Connection
{
protected:
    Address address;
    int sock = 1;

    bool openAddress(const AddressInfo &info);

public:
    Connection();
    Connection(const std::string &host,
        const std::string &service = "http",
        DnsCache *cache = nullptr);
    Connection(const Connection &other) = delete;
    ~Connection();

    // REQUESTS
    void open(const std::string &host,
        const std::string &service = "http",
        DnsCache *cache = nullptr);
    void close();
    void setTimeout(const Timeout &timeout);
    void setTimeout(const Timeout &timeout,
        const bool input);
    void send(const std::string &request);

    // RESPONSE
    std::string headers();
    std::string chunked();
    std::string body(const long length);
    std::string read();
};

}   /* lattice */

#endif              // WIN32
