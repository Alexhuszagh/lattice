//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket adaptors.
 */

#pragma once

#include <lattice/config.h>
#include <lattice/adaptor/openssl.h>
#include <lattice/adaptor/nossl.h>
#include <lattice/adaptor/posix.h>
#include <lattice/adaptor/windows.h>

LATTICE_BEGIN_NAMESPACE

// MACROS
// ------

#if defined(LATTICE_HAVE_OPENSSL)
#   define HAVE_SSL
#endif

// OBJECTS
// -------

// HTTP ADAPTERS
#ifdef _WIN32
    typedef win32_socket_adaptor_t http_adaptor_t;
#else
    typedef posix_socket_adaptor_t http_adaptor_t;
#endif

// HTTPS ADAPTERS
#if defined(HAVE_SSL) && defined(LATTICE_HAVE_OPENSSL)
    typedef open_ssl_adaptor_t<http_adaptor_t> ssl_adaptor_t;
#else
    typedef no_ssl_adaptor_t<http_adaptor_t> ssl_adaptor_t;
#endif

LATTICE_END_NAMESPACE
