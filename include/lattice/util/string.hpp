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

/** \brief Trim characters from left-end of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
void ltrim(std::string &string,
    const std::string &characters = " \n\r\t");

/** \brief Trim characters from right-end of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
void rtrim(std::string &string,
    const std::string &characters = " \n\r\t");

/** \brief Trim characters from both ends of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
void trim(std::string &string,
    const std::string &characters = " \n\r\t");

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
