//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include <lattice/proxy.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


proxy_t::operator bool() const
{
    return !empty();
}

LATTICE_END_NAMESPACE