//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Check if proxy has been set.
 */
Proxy::operator bool() const
{
    return !empty();
}


/** \brief Convert proxy to URL for connection.
 */
Proxy::operator Url() const
{
    return Url(static_cast<Url>(*this));
}


}   /* lattice */
