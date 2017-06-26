//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief POSIX socket adapter.
 */

#ifndef _WIN32

#pragma once

#include "lattice/dns.hpp"
#include "lattice/ssl.hpp"
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
    bool open(const addrinfo &info,
        const std::string &/*host*/);
    void close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);

    // OPTIONS
    void setReuseAddress();
    void setTimeout(const Timeout &timeout);
    void setCertificateFile(const CertificateFile &certificate);
    void setRevocationLists(const RevocationLists &revoke);
    void setSslProtocol(const SslProtocol ssl);

    // DATA
    const int fd() const;
};


}   /* lattice */

#endif              // POSIX
