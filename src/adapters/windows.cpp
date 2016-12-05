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
#include <iostream>


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
bool Win32SocketAdapter::open(const addrinfo &info)
{
    sock = ::socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sock == INVALID_SOCKET) {
        return false;
    }
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
    std::lock_guard<std::mutex> lock(MUTEX);
    std::cerr << "Warning: HTTP requests do not support certificates."
              << "Do NOT send sensitive data without SSL/TLS."
              << std::endl;
}


/** \brief Set SSL protocol.
 */
void Win32SocketAdapter::setSslProtocol(const SslProtocol ssl)
{
    std::lock_guard<std::mutex> lock(MUTEX);
    std::cerr << "Warning: HTTP requests do not support SSL/TLS."
              << "Do NOT send sensitive data without SSL/TLS."
              << std::endl;
}


/** \brief Get socket descriptor.
 */
const SOCKET Win32SocketAdapter::fd() const
{
    return sock;
}


}   /* lattice */

#endif
