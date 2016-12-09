//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief POSIX socket adapter.
 */

#ifndef _WIN32

#include "lattice.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
PosixSocketAdapter::PosixSocketAdapter()
{}


/** \brief Destructor.
 */
PosixSocketAdapter::~PosixSocketAdapter()
{}


/** \brief Open socket.
 */
bool PosixSocketAdapter::open(const addrinfo &info,
    const std::string & /*host*/)
{
    sock = ::socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sock < 0) {
        return false;
    }
    setReuseAddress();
    if (::connect(sock, info.ai_addr, info.ai_addrlen) >= 0) {
        return true;
    }

    ::close(sock);
    return false;
}


/** \brief Close socket.
 */
void PosixSocketAdapter::close()
{
    if (sock >= 0) {
        ::close(sock);
        sock = -1;
    }
}


/** \brief Write data to socket.
 */
size_t PosixSocketAdapter::write(const char *buf,
    size_t len)
{
    return ::send(sock, buf, len, 0);
}


/** \brief Read data from socket.
 */
size_t PosixSocketAdapter::read(char *buf,
    size_t count)
{
    return ::recv(sock, buf, count, 0);
}


/** \brief Allow socket address reuse.
 */
void PosixSocketAdapter::setReuseAddress()
{
    int reuse = 1;
    char *option = reinterpret_cast<char*>(&reuse);
    int size = sizeof(reuse);
    if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, option, size)) {
        throw SocketOptionError();
    }

    #ifdef SO_REUSEPORT
        // use reuseport if available
        if (::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, option, size)) {
            throw SocketOptionError();
        }
    #endif
}


/** \brief Set the max time for socket requests.
 *
 *  \warning Microseconds are not set since it causes stoichastic
 *  errors.
 */
void PosixSocketAdapter::setTimeout(const Timeout &timeout)
{
    // create timeout, do not set microseconds as it causes errors
    struct timeval value;
    value.tv_sec = timeout.seconds();
    value.tv_usec = 0;

    // set options
    char *option = reinterpret_cast<char*>(&value);
    socklen_t size = sizeof(timeval);
    if (::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, option, size)) {
        throw SocketOptionError();
    }
    if (::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, option, size)) {
        throw SocketOptionError();
    }
}


/** \brief Associate a certificate file with the socket.
 */
void PosixSocketAdapter::setCertificateFile(const CertificateFile &certificate)
{
    encryptionWarning();
}


/** \brief Set file to manually revoke certificates.
 */
void PosixSocketAdapter::setRevocationLists(const RevocationLists &revoke)
{
    encryptionWarning();
}


/** \brief Set SSL protocol.
 */
void PosixSocketAdapter::setSslProtocol(const SslProtocol ssl)
{
    encryptionWarning();
}


/** \brief Change peer certificate validation (noop).
 */
void PosixSocketAdapter::setVerifyPeer(const VerifyPeer &peer)
{}


/** \brief Get socket descriptor.
 */
const int PosixSocketAdapter::fd() const
{
    return sock;
}


}   /* lattice */

#endif
