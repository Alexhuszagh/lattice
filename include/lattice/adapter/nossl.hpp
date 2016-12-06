//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Create no-opt adapter.
 */

#include "lattice/dns.hpp"
#include "lattice/exception.hpp"
#include "lattice/method.hpp"
#include "lattice/ssl.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief No-opt SSL socket adapter.
 */
template <typename HttpAdapter>
class NoSslAdapter
{
protected:
    typedef NoSslAdapter<HttpAdapter> This;

public:
    NoSslAdapter();
    NoSslAdapter(const This &other) = delete;
    ~NoSslAdapter();

    // REQUESTS
    bool open(const addrinfo &info);
    void close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);

    // OPTIONS
    void setTimeout(const Timeout &timeout);
    void setCertificateFile(const CertificateFile &certificate);
    void setSslProtocol(const SslProtocol protocol);
};


// IMPLEMENTATION
// --------------


/** \brief Null constructor.
 */
template <typename HttpAdapter>
NoSslAdapter<HttpAdapter>::NoSslAdapter()
{
    throw MissingSslError();
}


/** \brief Destructor.
 */
template <typename HttpAdapter>
NoSslAdapter<HttpAdapter>::~NoSslAdapter()
{
    throw MissingSslError();
}


/** \brief Open socket.
 */
template <typename HttpAdapter>
bool NoSslAdapter<HttpAdapter>::open(const addrinfo &info)
{
    throw MissingSslError();
    return false;
}


/** \brief Close socket.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::close()
{
    throw MissingSslError();
}


/** \brief Write data to socket.
 */
template <typename HttpAdapter>
size_t NoSslAdapter<HttpAdapter>::write(const char *buf,
    size_t len)
{
    throw MissingSslError();
    return 0;
}


/** \brief Read data from socket.
 */
template <typename HttpAdapter>
size_t NoSslAdapter<HttpAdapter>::read(char *buf,
    size_t count)
{
    throw MissingSslError();
    return 0;
}


/** \brief Set the max time for socket requests.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setTimeout(const Timeout &timeout)
{
    throw MissingSslError();
}


/** \brief Set the max time for socket requests.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setCertificateFile(const CertificateFile &certificate)
{
    throw MissingSslError();
}


/** \brief Set SSL protocol.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setSslProtocol(const SslProtocol protocol)
{
    throw MissingSslError();
}


}   /* lattice */
