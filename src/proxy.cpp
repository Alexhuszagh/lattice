//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include <lattice/proxy.hpp>


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

}   /* lattice */
