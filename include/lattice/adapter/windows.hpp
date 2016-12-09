//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Windows socket adapter.
 */

#pragma once

#ifdef _WIN32

#include "abstract.hpp"
#include "lattice/dns.hpp"
#include "lattice/method.hpp"
#include "lattice/ssl.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"

#include <ws2tcpip.h>


namespace lattice
{
// OBJECTS
// -------


/** \brief Adapter for Win32 sockets.
 */
class Win32SocketAdapter: public AbstractAdapter
{
protected:
    SOCKET sock = INVALID_SOCKET;

public:
    Win32SocketAdapter();
    Win32SocketAdapter(const Win32SocketAdapter &other) = delete;
    ~Win32SocketAdapter();

    // REQUESTS
    virtual bool open(const addrinfo &info,
        const std::string &/*host*/);
    virtual bool close();
    virtual size_t write(const char *buf,
        size_t len);
    virtual size_t read(char *buf,
        size_t count);

    // OPTIONS
    virtual void setReuseAddress();
    virtual void setTimeout(const Timeout &timeout);
    virtual void setCertificateFile(const CertificateFile &certificate);
    virtual void setRevocationLists(const RevocationLists &revoke);
    virtual void setSslProtocol(const SslProtocol ssl);
    virtual void setVerifyPeer(const VerifyPeer &peer);

    // DATA
    const SOCKET fd() const;
};


}   /* lattice */

#endif              // WIN32
