//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Abstract socket adapter.
 */

#pragma once

#include "lattice/dns.hpp"
#include "lattice/ssl.hpp"
#include "lattice/timeout.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Purely abstract socket adapter.
 */
struct AbstractAdapter
{
    // REQUESTS
    virtual bool open(const addrinfo &info,
        const std::string &host) = 0;
    virtual void close() = 0;
    virtual size_t write(const char *buf,
        size_t len) = 0;
    virtual size_t read(char *buf,
        size_t count) = 0;

    // OPTIONS
    virtual void setTimeout(const Timeout &timeout) = 0;
    virtual void setCertificateFile(const CertificateFile &certificate) = 0;
    virtual void setRevocationLists(const RevocationLists &revoke) = 0;
    virtual void setSslProtocol(const SslProtocol protocol) = 0;
    virtual void setVerifyPeer(const VerifyPeer &peer) = 0;
};


}   /* lattice */
