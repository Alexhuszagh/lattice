//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Filesystem and path utilities.
 */

#pragma once

#include <array>
#include <string>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Get the directory name for a path.
 */
std::string dirname(const std::string &path);

/** \brief Get the name without directory.
 */
std::string basename(const std::string &path);

/** \brief Split the filename and extension.
 */
std::array<std::string, 2> splitext(const std::string &path);


}   /* lattice */
