//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief POSIX socket adapter.
 */

#ifndef _WIN32

#pragma once

#include "lattice/certificate.hpp"
#include "lattice/dns.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"

#include <netdb.h>


namespace lattice
{
// OBJECTS
// -------


/** \brief Adapter for POSIX sockets.
 */
class PosixSocketAdapter
{
protected:
    int sock = -1;

public:
    PosixSocketAdapter();
    PosixSocketAdapter(const PosixSocketAdapter &other) = delete;
    ~PosixSocketAdapter();

    // REQUESTS
    bool open(const addrinfo &info);
    bool close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);

    // OPTIONS
    void setTimeout(const Timeout &timeout);
    void setCertificateFile(const CertificateFile &certificate);
};


}   /* lattice */

#endif              // POSIX
