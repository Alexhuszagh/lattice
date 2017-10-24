//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Windows socket adaptor.
 */

#pragma once

#ifdef _WIN32

#include <lattice/dns.h>
#include <lattice/method.h>
#include <lattice/ssl.h>
#include <lattice/timeout.h>
#include <lattice/url.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Adaptor for Win32 sockets.
 */
class win32_socket_adaptor_t
{
public:
    typedef win32_socket_adaptor_t self;

    win32_socket_adaptor_t();
    win32_socket_adaptor_t(const self&) = delete;
    self& operator=(const self&) = delete;
    ~win32_socket_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const std::string&);
    bool close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);

    // OPTIONS
    void set_reuse_address();
    void set_timeout(const timeout_t& timeout);
    void set_certificate_file(const certificate_file_t& certificate);
    void set_revocation_lists(const revocation_lists_t& revoke);
    void set_ssl_protocol(const ssl_protocol_t ssl);

    // DATA
    const SOCKET fd() const;

protected:
    SOCKET sock = INVALID_SOCKET;
};

LATTICE_END_NAMESPACE

#endif              // WIN32
