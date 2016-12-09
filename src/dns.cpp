//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Domain name lookups and address caching.
 */

#include "lattice.hpp"

#include <cstdlib>

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
Address::Address(const addrinfo &info):
    family(info.ai_family),
    socket_type(info.ai_socktype),
    protocol(info.ai_protocol),
    address(*info.ai_addr),
    length(info.ai_addrlen)
{}


/** \brief Copy constructor.
 */
Address::Address(const Address &other):
    family(other.family),
    socket_type(other.socket_type),
    protocol(other.protocol),
    address(other.address),
    length(other.length)
{}


/** \brief Convert to addrinfo for connection.
 */
Address::operator addrinfo() const
{
    addrinfo info;
    info.ai_family = family;
    info.ai_socktype = socket_type;
    info.ai_protocol = protocol;
    info.ai_addr = const_cast<sockaddr*>(&address);
    info.ai_addrlen = length;

    return info;
}


/** \brief Null constructor.
 */
DnsLookup::DnsLookup()
{}


/** \brief Initializer list constructor.
 */
DnsLookup::DnsLookup(const std::string &host,
    const std::string &service)
{
    // initialize our hints
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // get our host and port
    std::string nodeString, portString;
    const char *node = host.data();
    const char *port = service.data();
    const size_t index = host.find(":");
    if (index != std::string::npos) {
        nodeString = host.substr(0, index);
        portString = host.substr(index+1);
        node = nodeString.data();
        port = portString.data();
    }

    if (getaddrinfo(node, port, &hints, &result)) {
        throw AddressError(host, service);
    }

    info = result;
}


/** \brief Destructor.
 */
DnsLookup::~DnsLookup()
{
    if (info) {
        freeaddrinfo(info);
        info = nullptr;
    }
}


/** \brief Get iterator at first address.
 */
AddressIterator DnsLookup::begin() const
{
    return AddressIterator(info);
}


/** \brief Get iterator past end of addresses.
 */
AddressIterator DnsLookup::end() const
{
    return AddressIterator(nullptr);
}


}   /* lattice */
