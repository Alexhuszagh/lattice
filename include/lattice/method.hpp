//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP methods.
 */

#pragma once

#ifdef DELETE
#   undef DELETE
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief HTTP request methods.
 */
enum Method
{
    STOP    = 0,
    GET     = 1,
    HEAD    = 2,
    POST    = 3,
    DELETE  = 4,
    OPTIONS = 5,
    PATCH   = 6,
    PUT     = 7,
    TRACE   = 8,
    CONNECT = 9,
};

}   /* lattice */
