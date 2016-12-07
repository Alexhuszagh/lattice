//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief POSIX socket adapter.
 */

#ifndef _WIN32

#pragma once

#include "abstract.hpp"
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
class PosixSocketAdapter: public AbstractAdapter
{
protected:
    int sock = -1;

public:
    PosixSocketAdapter();
    PosixSocketAdapter(const PosixSocketAdapter &other) = delete;
    ~PosixSocketAdapter();

    // REQUESTS
    virtual bool open(const addrinfo &info,
        const std::string &/*host*/);
    virtual void close();
    virtual size_t write(const char *buf,
        size_t len);
    virtual size_t read(char *buf,
        size_t count);

    // OPTIONS
    virtual void setTimeout(const Timeout &timeout);
    virtual void setCertificateFile(const CertificateFile &certificate);
    virtual void setRevocationLists(const RevocationLists &revoke);
    virtual void setSslProtocol(const SslProtocol ssl);
    virtual void setVerifyPeer(const VerifyPeer &peer);

    // DATA
    const int fd() const;
};


}   /* lattice */

#endif              // POSIX
