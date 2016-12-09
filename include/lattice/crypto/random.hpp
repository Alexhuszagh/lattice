//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cryptographic random number generators.
 */

#include <string>


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief Get random bytes to seed number generators.
 */
std::string sysrandom(const size_t size);

}   /* lattice */
