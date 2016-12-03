//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief DNS caching.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
AddressInfo::AddressInfo()
{}


/** \brief Initializer list constructor.
 */
AddressInfo::AddressInfo(const addrinfo &info):
    family(info.ai_family),
    socket_type(info.ai_socktype),
    protocol(info.ai_protocol),
    address(*info.ai_addr),
    length(info.ai_addrlen)
{}


/** \brief Copy constructor.
 */
AddressInfo::AddressInfo(const AddressInfo &other):
    family(other.family),
    socket_type(other.socket_type),
    protocol(other.protocol),
    address(other.address),
    length(other.length)
{}

}   /* lattice */
