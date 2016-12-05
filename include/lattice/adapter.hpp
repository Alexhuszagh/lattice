//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket adapters.
 */

#pragma once

#include "adapter/posix.hpp"
#include "adapter/windows.hpp"


namespace lattice
{
// OBJECTS
// -------

// HTTP ADAPTERS
#ifdef _WIN32
    typedef Win32SocketAdapter HttpAdapter;
#else
    typedef PosixSocketAdapter HttpAdapter;
#endif

// HTTPS ADAPTERS
#ifdef HAVE_SSL
#   if defined(HAVE_OPENSSL)
        typedef OpenSslAdapter<HttpAdapter> SslAdapter;
#   elif defined(HAVE_LIBRESSL)
        typedef LibreSslAdapter<HttpAdapter> SslAdapter;
#   endif
#endif


}   /* lattice */
