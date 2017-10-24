//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP transfer encoding.
 */

#pragma once

#include <lattice/config.h>
#include <pycpp/misc/enum.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


/** \brief Enumerations for acceptable transfer encodings.
 */
enum transfer_encoding_t: unsigned int
{
    CHUNKED     = 1,
    COMPRESS    = 2,
    DEFLATE     = 4,
    GZIP        = 8,
    IDENTITY    = 16,
};

enum_flag(transfer_encoding_t);

LATTICE_END_NAMESPACE
