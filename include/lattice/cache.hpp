//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief DNS caching.
 */

#pragma once

#ifdef _WIN32
#   include <ws2tcpip.h>
#else
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netdb.h>
#endif

#include <string>
#include <unordered_map>


namespace lattice
{
// OBJECTS
// -------


/** \brief Host address information.
 */
struct AddressInfo
{
    int family;
    int socket_type;
    int protocol;
    sockaddr address;
    size_t length;

    AddressInfo();
    AddressInfo(const addrinfo &info);
    AddressInfo(const AddressInfo &other);
};


/** \brief Cache for DNS lookups.
 */
class DnsCache: public std::unordered_multimap<std::string, AddressInfo>
{
protected:
    typedef std::unordered_multimap<std::string, AddressInfo> Base;

public:
    using Base::Base;
};


}   /* lattice */
