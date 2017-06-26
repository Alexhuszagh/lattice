//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Windows socket adapter.
 */

#pragma once

#ifdef _WIN32

#include "lattice/dns.hpp"
#include "lattice/method.hpp"
#include "lattice/ssl.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Adapter for Win32 sockets.
 */
class Win32SocketAdapter
{
protected:
    SOCKET sock = INVALID_SOCKET;

public:
    Win32SocketAdapter();
    Win32SocketAdapter(const Win32SocketAdapter &other) = delete;
    ~Win32SocketAdapter();

    // REQUESTS
    bool open(const addrinfo &info,
        const std::string &/*host*/);
    bool close();
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
    const SOCKET fd() const;
};


}   /* lattice */

#endif              // WIN32
