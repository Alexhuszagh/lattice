//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP transfer encoding.
 */

#pragma once

#include "util.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Enumerations for acceptable transfer encodings.
 */
enum TransferEncoding
{
    CHUNKED     = 1,
    COMPRESS    = 2,
    DEFLATE     = 4,
    GZIP        = 8,
    IDENTITY    = 16,
};

FLAGS(TransferEncoding);


}   /* lattice */
