//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief POSIX socket adapter.
 */

#ifndef _WIN32

#pragma once

#include <lattice/dns.hpp>
#include <lattice/ssl.hpp>
#include <lattice/timeout.hpp>
#include <lattice/url.hpp>

#include <netdb.h>


namespace lattice
{
// OBJECTS
// -------


/**
 *  \brief Adapter for POSIX sockets.
 */
class posix_socket_adaptor_t
{
protected:
    int sock = -1;

public:
    typedef posix_socket_adaptor_t self;

    posix_socket_adaptor_t();
    posix_socket_adaptor_t(const self&) = delete;
    self& operator=(const self&) = delete;
    ~posix_socket_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const std::string&);
    void close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);

    // OPTIONS
    void set_reuse_address();
    void set_timeout(const timeout_t& timeout);
    void set_certificate_file(const CertificateFile& certificate);
    void set_revocation_lists(const RevocationLists& revoke);
    void set_ssl_protocol(ssl_protocol_t ssl);

    // DATA
    const int fd() const;
};

}   /* lattice */

#endif              // POSIX
