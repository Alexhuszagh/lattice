//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief String utilities.
 */

#pragma once

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Split string by delimiter.
 *
 *  Split string by delimiter, but allow escaped or quoted characters
 *  to be successfully included.
 *
 *  \param string           String to split
 *  \param quote            Quoting character
 *  \param escape           Escape character
 *  \param delimiters       Character delimiter
 */
std::vector<std::string> safesplit(const std::string &string,
    const char delimiter,
    const char quote,
    const char escape);

}   /* lattice */
