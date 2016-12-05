//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief OpenSSL socket adapter.
 */

#ifdef HAVE_OPENSSL

#include "lattice/certificate.hpp"
#include "lattice/dns.hpp"
#include "lattice/exception.hpp"
#include "lattice/method.hpp"
#include "lattice/mutex.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"

#include <openssl/ssl.h>

// LEGACY
#ifndef TLS_client_method
#   define TLS_client_method SSLv23_client_method
#endif


namespace lattice
{
// CONSTANTS
// ---------

static bool SSL_INITIALIZED = false;

// OBJECTS
// -------


/** \brief Socket adapter for OpenSSL.
 */
template <typename HttpAdapter>
class OpenSslAdapter
{
protected:
    typedef OpenSslAdapter<HttpAdapter> This;
    HttpAdapter adapter;
    CertificateFile certificate;
    SslProtocol protocol = TLS;

    SSL_CTX *ctx = nullptr;
    SSL *ssl = nullptr;

    void initialize();

public:
    OpenSslAdapter();
    OpenSslAdapter(const This &other) = delete;
    ~OpenSslAdapter();

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


/** \brief Initialize OpenSSL.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::initialize()
{
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
}


/** \brief Destructor.
 */
template <typename HttpAdapter>
OpenSslAdapter<HttpAdapter>::~OpenSslAdapter()
{
    close();
}


/** \brief Null constructor.
 *
 *  If not initialized, initialize SSL.
 */
template <typename HttpAdapter>
OpenSslAdapter<HttpAdapter>::OpenSslAdapter()
{
    std::lock_guard<std::mutex> lock(MUTEX);
    if (!SSL_INITIALIZED) {
        initialize();
        SSL_INITIALIZED = true;
    }
}


/** \brief Open socket.
 */
template <typename HttpAdapter>
bool OpenSslAdapter<HttpAdapter>::open(const addrinfo &info)
{
    // initialize SSL methods
    switch (protocol) {
        case SSL_V23:
            ctx = SSL_CTX_new(SSLv23_client_method());
            break;
        case TLS_V12:
            ctx = SSL_CTX_new(TLSv1_2_client_method());
            break;
        case TLS_V11:
            ctx = SSL_CTX_new(TLSv1_1_client_method());
            break;
        case TLS_V1:
            ctx = SSL_CTX_new(TLSv1_client_method());
            break;
        case SSL_V3:
            ctx = SSL_CTX_new(SSLv3_client_method());
            break;
        case TLS:
            /* fallthrough */
        default:
            ctx = SSL_CTX_new(TLS_client_method());
            break;
    }
    SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
    if (!certificate.empty()) {
        const char *data = certificate.data();
        SSL_CTX_use_certificate_file(ctx, data, SSL_FILETYPE_PEM);
        SSL_CTX_use_PrivateKey_file(ctx, data, SSL_FILETYPE_PEM);
    }
    ssl = SSL_new(ctx);

    // open socket and create SSL
    adapter.open(info);
    SSL_set_fd(ssl, adapter.fd());
    if (SSL_connect(ssl) != 1) {
        throw SslHandshakeError();
    }

    return true;
}


/** \brief Close socket.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::close()
{
    if (ssl) {
        SSL_shutdown(ssl);
        adapter.close();
        SSL_free(ssl);
        ssl = nullptr;
    }
    if (ctx) {
        SSL_CTX_free(ctx);
        ctx = nullptr;
    }
}


/** \brief Write data to socket.
 */
template <typename HttpAdapter>
size_t OpenSslAdapter<HttpAdapter>::write(const char *buf,
    size_t len)
{
    return SSL_write(ssl, buf, len);
}


/** \brief Read data from socket.
 */
template <typename HttpAdapter>
size_t OpenSslAdapter<HttpAdapter>::read(char *buf,
    size_t count)
{
    return SSL_read(ssl, buf, count);
}


/** \brief Set the max time for socket requests.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setTimeout(const Timeout &timeout)
{
    adapter.setTimeout(timeout);
}


/** \brief Set the max time for socket requests.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setCertificateFile(const CertificateFile &certificate)
{
    this->certificate = certificate;
}


/** \brief Set SSL protocol.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setSslProtocol(const SslProtocol protocol)
{
    this->protocol = protocol;
}


}   /* lattice */

#endif
