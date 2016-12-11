//  :copyright: (c) 2011 by Ben Noordhuis <info@bnoordhuis.nl>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Punycode encoding/decoding.
 */

#pragma once

#include <cstdint>
#include <cstddef>


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief String data is Unicode.
 */
bool isUnicode(const std::string &string);

/** \brief Convert UTF-8 data to Punycode.
 */
std::string utf8ToPuny(const std::string &string);

/** \brief Convert UTF-16 data to Punycode.
 */
std::string utf16ToPuny(const std::string &string);

/** \brief Convert UTF-32 data to Punycode.
 */
std::string utf32ToPuny(const std::string &string);

// API
// ---

static const auto &UTF8_TO_PUNYCODE = utf8ToPuny;
static const auto &UTF16_TO_PUNYCODE = utf16ToPuny;
static const auto &UTF32_TO_PUNYCODE = utf32ToPuny;

}   /* lattice */
