//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Create no-opt adapter.
 */

#include "abstract.hpp"
#include "lattice/exception.hpp"
#include "lattice/ssl.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief No-opt SSL socket adapter.
 */
template <typename HttpAdapter>
class NoSslAdapter: public AbstractAdapter
{
protected:
    typedef NoSslAdapter<HttpAdapter> This;

public:
    NoSslAdapter();
    NoSslAdapter(const This &other) = delete;
    ~NoSslAdapter();

    // REQUESTS
    virtual bool open(const addrinfo &info,
        const std::string &host);
    virtual void close();
    virtual size_t write(const char *buf,
        size_t len);
    virtual size_t read(char *buf,
        size_t count);

    // OPTIONS
    virtual void setReuseAddress();
    virtual void setTimeout(const Timeout &timeout);
    virtual void setCertificateFile(const CertificateFile &certificate);
    virtual void setRevocationLists(const RevocationLists &revoke);
    virtual void setSslProtocol(const SslProtocol protocol);
    virtual void setVerifyPeer(const VerifyPeer &peer);
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
bool NoSslAdapter<HttpAdapter>::open(const addrinfo &info,
    const std::string &host)
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


/** \brief Allow socket address reuse.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setReuseAddress()
{
    throw MissingSslError();
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


/** \brief Set file to manually revoke certificates.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setRevocationLists(const RevocationLists &revoke)
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


/** \brief Change peer certificate validation.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::setVerifyPeer(const VerifyPeer &peer)
{
    throw MissingSslError();
}


}   /* lattice */
