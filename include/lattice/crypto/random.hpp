//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Random number generators.
 */

#include <string>


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief Get random bytes for cryptographic applications.
 */
std::string sysrandom(const size_t size);

/** \brief Get psuedo-random bytes for general purposes.
 */
std::string pseudorandom(const size_t size);

}   /* lattice */
