//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Windows socket adapter.
 */

#ifdef _WIN32

#include "lattice.hpp"

#include <winsock2.h>
#include <windows.h>

#include <cstdlib>


namespace lattice
{
// CONSTANTS
// ---------

static WSADATA SOCK_DATA;
static bool WSA_INITIALIZED = false;

// OBJECTS
// -------


/** \brief Null constructor.
 *
 *  If not initialized, start the Winsock API and register cleanup.
 */
Win32SocketAdapter::Win32SocketAdapter()
{
    std::lock_guard<std::mutex> lock(MUTEX);
    if (!WSA_INITIALIZED) {
        if (WSAStartup(MAKEWORD(2,2), &SOCK_DATA) != 0){
            throw WinSockStartupError();
        }
        std::atexit([]() {
            WSACleanup();
        });
        WSA_INITIALIZED = true;
    }
}


/** \brief Destructor.
 */
Win32SocketAdapter::~Win32SocketAdapter()
{}


/** \brief Open socket.
 */
bool Win32SocketAdapter::open(const addrinfo &info,
    const std::string & /*host*/)
{
    sock = ::socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sock == INVALID_SOCKET) {
        return false;
    }
    setReuseAddress();
    if (!::connect(sock, info.ai_addr, info.ai_addrlen)) {
        return true;
    }

    ::closesocket(sock);
    return false;
}


/** \brief Close socket.
 */
bool Win32SocketAdapter::close()
{
    if (sock != INVALID_SOCKET) {
        ::closesocket(sock);
        sock = INVALID_SOCKET;
        return true;
    }
    return false;
}


/** \brief Write data to socket.
 */
size_t Win32SocketAdapter::write(const char *buf,
    size_t len)
{
    return ::send(sock, buf, len, 0);
}


/** \brief Read data from socket.
 */
size_t Win32SocketAdapter::read(char *buf,
    size_t count)
{
    return ::recv(sock, buf, count, 0);
}


/** \brief Allow socket address reuse.
 */
void Win32SocketAdapter::setReuseAddress()
{
    int reuse = 1;
    char *option = reinterpret_cast<char*>(&reuse);
    int size = sizeof(reuse);
    if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, option, size)) {
        throw SocketOptionError();
    }
}


/** \brief Set the max time for socket requests.
 */
void Win32SocketAdapter::setTimeout(const Timeout &timeout)
{
    // create timeout
    int ms = timeout.milliseconds();
    char *option = reinterpret_cast<char*>(&ms);
    int size = sizeof(ms);

    // set options
    if (::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, option, size)) {
        throw SocketOptionError();
    }
    if (::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, option, size)) {
        throw SocketOptionError();
    }
}


/** \brief Associate a certificate file with the socket.
 */
void Win32SocketAdapter::setCertificateFile(const CertificateFile &certificate)
{
    encryptionWarning();
}


/** \brief Set file to manually revoke certificates.
 */
void Win32SocketAdapter::setRevocationLists(const RevocationLists &revoke)
{
    encryptionWarning();
}


/** \brief Set SSL protocol.
 */
void Win32SocketAdapter::setSslProtocol(const SslProtocol ssl)
{
    encryptionWarning();
}


/** \brief Change peer certificate validation (noop).
 */
void Win32SocketAdapter::setVerifyPeer(const VerifyPeer &peer)
{}


/** \brief Get socket descriptor.
 */
const SOCKET Win32SocketAdapter::fd() const
{
    return sock;
}


}   /* lattice */

#endif
