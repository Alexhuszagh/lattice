//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief String utilities.
 */

#pragma once

#include <algorithm>
#include <istream>
#include <string>
#include <type_traits>
#include <vector>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Check if string starts with substring.
 */
bool startswith(const std::string &string,
    const std::string &substring);

/** \brief Modified getline which trims carriage returns.
 */
std::istream & getline(std::istream &stream,
    std::string &string);

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

/** \brief Split string by a character delimiter(s).
 *
 *  \param string           String to split
 *  \param delimiters       Character delimiters to split on
 */
std::vector<std::string> split(const std::string &string,
    const char *delimiters);

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

/** \brief Join string from begin and end iterator pairs.
 *
 *  \param items            Vector of items to join
 *  \param delimiter        Iterator at last element to add
 */
std::string join(const std::vector<std::string> &items,
    const std::string &delimiter);

/** \brief Transform the string to lower-case.
 */
void tolower(std::string &string);

/** \brief Replace substring in string.
 *
 *  Replace up to `max` instances of `sub` with`repl` in the target
 *  string. By default, max is set to -1, or infinite occurences.
 *
 *  \param string           String to replace (in-place)
 *  \param sub              Characters to remove with repl in string
 *  \param repl             Characters to add in place of sub in string
 *  \param max              Maximum number of occurences to replace
 */
void replace(std::string &string,
    const std::string &sub,
    const std::string &repl,
    int max = -1);

/** Escape printable, non-Unicode, non-alphanumeric characters.
 *
 *  Escapes all ASCII non-alphanumeric characters, assuming
 *  UTF-8 source encoding.
 */
std::string escapeUrl(const std::string src);


}   /* lattice */
