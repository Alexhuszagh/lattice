//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#pragma once

#include <sstream>
#include <stdexcept>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Warn user about user encryption methods without SSL/TLS support.
 */
void encryption_warning();

}   /* lattice */
