//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#pragma once

#include <sstream>
#include <stdexcept>


namespace lattice
{
// OBJECTS
// -------


/** \brief Unable to start the WinSock API
 */
class WinSockStartupError
{
    /** \brief Display message from Windows socket startup error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to startup the Windows socket API.\n";
    }
};


/** \brief Error in in getting an address from the host/service.
 */
class AddressError: public std::exception
{
protected:
    std::string host;
    std::string service;

public:
    AddressError(const std::string &host,
        const std::string &service);

    /** \brief Display message from address lookup error.
     */
    virtual const char * what() const throw()
    {
        std::stringstream stream;
        stream << "Unable to get address from getaddrinfo("
               << host.data() << ", " << service.data() << ").\n";

        return stream.str().data();
    }
};


/** \brief Error in establishing a connection.
 */
struct ConnectionError: public std::exception
{
    /** \brief Display message from connection error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to establish connection.\n";
    }
};


/** \brief Error sending a request to the server.
 */
class RequestError: public std::exception
{
protected:
    int sent;
    int total;

public:
    RequestError(const int sent,
        const int total);

    /** \brief Display number of bytes successfully sent.
     */
    virtual const char * what() const throw()
    {
        std::stringstream stream;
        stream << "Unable to get make request: sent "
               << sent << " of " << total << " bytes.\n";

        return stream.str().data();
    }
};


/** \brief Error in settings socket option.
 */
struct SocketOptionError: public std::exception
{
    /** \brief Display message from socket option error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to set socket option via setsockopt().\n";
    }
};


/** \brief Relative URL set for main connection.
 */
class RelativeUrlError: public std::exception
{
    /** \brief Display message from relative URL error.
     */
    virtual const char * what() const throw()
    {
        return "Relative URL set: host and service unknown.\n";
    }
};


/** \brief Error when SSL handshake fails.
 */
class SslHandshakeError: public std::exception
{
    /** \brief Display message from SSL handshake error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to establish SSL handshake.\n";
    }
};


/** \brief Error when HTTPS connection is requested but no library used.
 */
class MissingSslError: public std::exception
{
    /** \brief Display message from SSL error.
     */
    virtual const char * what() const throw()
    {
        return "HTTPS connection requested, but not SSL library found.\n";
    }
};


/** \brief Error creating SSL context.
 */
class SSlContextError: public std::exception
{
    /** \brief Display message from SSL error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to initialize SSL context.\n";
    }
};



/** \brief Error loading certificates from file.
 */
class CertificateLoadingError: public std::exception
{
    /** \brief Display message from SSL error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to load certificates from file.\n";
    }
};


/** \brief Warn user about user encryption methods without SSL/TLS support.
 */
void encryptionWarning();


}   /* lattice */
