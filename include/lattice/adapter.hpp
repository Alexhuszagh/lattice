//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket adapters.
 */

#pragma once

#include "adapter/openssl.hpp"
#include "adapter/nossl.hpp"
#include "adapter/posix.hpp"
#include "adapter/windows.hpp"


namespace lattice
{
// MACROS
// ------


#if defined(HAVE_OPENSSL)
#   define HAVE_SSL
#endif

// OBJECTS
// -------

// HTTP ADAPTERS
#ifdef _WIN32
    typedef Win32SocketAdapter HttpAdapter;
#else
    typedef PosixSocketAdapter HttpAdapter;
#endif

// HTTPS ADAPTERS
#if defined(HAVE_SSL) && defined(HAVE_OPENSSL)
    typedef OpenSslAdapter<HttpAdapter> SslAdapter;
#else
    typedef NoSslAdapter<HttpAdapter> SslAdapter;
#endif


}   /* lattice */
