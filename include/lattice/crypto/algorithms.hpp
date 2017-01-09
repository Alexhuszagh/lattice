//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cryptographic algorithms (**weak**) for requests.
 */

#pragma once


namespace lattice
{
// OBJECTS
// -------


/** \brief Various digest algorithms for digest authentication.
 */
enum DigestAlgorithm
{
    MD5_DIGEST          = 0,
    MD5_SESS_DIGEST     = 1,
    SHA1_DIGEST         = 2,
};


}   /* lattice */
