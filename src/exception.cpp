//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
AddressError::AddressError(const std::string &host,
        const std::string &service):
    host(host),
    service(service)
{}


/** \brief Initializer list constructor.
 */
RequestError::RequestError(const int sent,
        const int total):
    sent(sent),
    total(total)
{}

}   /* lattice */
