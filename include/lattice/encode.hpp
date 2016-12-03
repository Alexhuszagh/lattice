//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: Boost, see BOOST.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief URL encode/decode data.
 */

#pragma once

#include <string>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Convert character to hex.
 *
 *  \param x            Character to convert
 *  \return             Hex-decoded character
 */
unsigned char toHex(unsigned char x);

/** \brief Convert character from hex.
 *
 *  \param x            Character to convert
 *  \return             Hex-decoded character
 */
unsigned char fromHex(unsigned char x);

/** \brief URL-encode string.
 *
 *  \param string       String to encode
 *  \return             URL-encoded string
 */
std::string encode(const std::string &string);

/** \brief URL-decode string.
 *
 *  \param string       String to encode
 *  \return             URL-decoded string
 */
std::string decode(const std::string &string);

}   /* lattice */
