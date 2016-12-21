//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP transfer encoding.
 */

#pragma once

#include "util.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif


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

LATTICE_FLAGS(TransferEncoding);


}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
