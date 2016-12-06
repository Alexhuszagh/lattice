//  :copyright: (c) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief OpenSSL socket adapter.
 */

#ifdef HAVE_OPENSSL

#include "lattice/dns.hpp"
#include "lattice/exception.hpp"
#include "lattice/method.hpp"
#include "lattice/mutex.hpp"
#include "lattice/ssl.hpp"
#include "lattice/timeout.hpp"
#include "lattice/url.hpp"

#include <openssl/asn1.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

// LEGACY
#ifndef TLS_client_method
#   define TLS_client_method SSLv23_client_method
#endif
#ifndef ASN1_STRING_get0_data
#   define ASN1_STRING_get0_data(x) ASN1_STRING_data(x)
#endif


namespace lattice
{
// MACROS
// ------

#define CHARP(charp) reinterpret_cast<char*>(charp)
#define CONST_CHARP(charp) reinterpret_cast<const char*>(charp)
#define SIZE_T(number) static_cast<size_t>(number)
#define GET_ASN1_STRING(check) CONST_CHARP(ASN1_STRING_get0_data(check->d.ia5))
#define GET_ASN1_LENGTH(check) SIZE_T(ASN1_STRING_length(check->d.ia5))

// CONSTANTS
// ---------

const char * const PREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!SRP:!PSK:!CAMELLIA:!RC4:!MD5:!DSS";
static bool SSL_INITIALIZED = false;
static X509_STORE *STORE = nullptr;

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
    RevocationLists revoke;
    SslProtocol protocol = TLS;
    VerifyPeer verifypeer;

    SSL_CTX *ctx = nullptr;
    SSL *ssl = nullptr;

    static void initialize();
    static void cleanup();
    void setContext();
    void setCertificate();
    void setRevoke();
    void setVerify(const std::string &host);
    void sslConnect();

public:
    OpenSslAdapter();
    OpenSslAdapter(const This &other) = delete;
    ~OpenSslAdapter();

    // REQUESTS
    bool open(const addrinfo &info,
        const std::string &host);
    void close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);

    // OPTIONS
    void setTimeout(const Timeout &timeout);
    void setCertificateFile(const CertificateFile &certificate);
    void setRevocationLists(const RevocationLists &revoke);
    void setSslProtocol(const SslProtocol protocol);
    void setVerifyPeer(const VerifyPeer &peer);
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


/** \brief Cleanup OpenSSL (noop).
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::cleanup()
{}


/** \brief Set the SSL connection context.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setContext()
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
    if (!ctx) {
        throw SSlContextError();
    }
    SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
}


/** \brief Set certificate file for the store.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setCertificate()
{
    int ok = 1;
    const char *data = certificate.data();
    switch (certificate.format()) {
        case PEM:
            ok = SSL_CTX_use_certificate_chain_file(ctx, data);
            break;
        case ASN1:
            ok = SSL_CTX_use_certificate_file(ctx, data, SSL_FILETYPE_ASN1);
            break;
        case SSL_ENGINE:
            /* fallthrough */
        case PKCS8:
            /* fallthrough */
        case PKCS12:
            /* fallthrough */
        default:
            /* don't recognize certificates, fail */
            ok = -1;
    }

    if (ok != 1) {
        throw CertificateLoadingError();
    }
}


/** \brief Set revocation lists for the store.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setRevoke()
{
    if (revoke.empty()) {
        return;
    }

    // get our store and lookup, and load the store
    X509_STORE *store = SSL_CTX_get_cert_store(ctx);
    X509_LOOKUP *lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
    int format = X509_FILETYPE_PEM;
    if (!lookup || !(X509_load_crl_file(lookup, revoke.data(), format))) {
        throw CertificateLoadingError();
    } else {
        int flags = X509_V_FLAG_CRL_CHECK | X509_V_FLAG_CRL_CHECK_ALL;
        X509_STORE_set_flags(store, flags);
    }
}


/** \brief Verify untrusted certificate with certificate bundle.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setVerify(const std::string &host)
{
    // set verification context
    int mode = verifypeer ? SSL_VERIFY_PEER : SSL_VERIFY_NONE;
    SSL_CTX_set_verify(ctx, mode, nullptr);
    if (!verifypeer) {
        return;
    }

     // hostname verification
    X509_VERIFY_PARAM *param = SSL_get0_param(ssl);
    X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
    X509_VERIFY_PARAM_set1_host(param, host.data(), 0);

    // set flags to avoid issues with legacy certificates
    X509_STORE *store = SSL_CTX_get_cert_store(ctx);
    X509_STORE_set_flags(store, X509_V_FLAG_TRUSTED_FIRST);

    // set preferred ciphers
    SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);

    // initalize content with bundle
    if (!certificate.empty()) {
        if (!SSL_CTX_load_verify_locations(ctx, certificate.data(), nullptr)) {
            throw CertificateLoadingError();
        }
    } else {
        SSL_CTX_set_default_verify_paths(ctx);
    }
}


/** \brief Connect to the remote host via SSL connect.
 *
 *  Process read and write descriptors.
 *
 *  \warning Errors can be thrown for **no** reason, skip the null
 *  errors, including system errors without an error log, and throw
 *  handshake errors otherwise.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::sslConnect()
{
    while (SSL_connect(ssl) == -1) {
        fd_set desriptors;
        FD_ZERO(&desriptors);
        FD_SET(adapter.fd(), &desriptors);

        switch (SSL_get_error(ssl, -1)) {
            case SSL_ERROR_NONE:
                /* fallthrough */
            case SSL_ERROR_ZERO_RETURN:
                /* no error */
                return;
            case SSL_ERROR_WANT_READ:
                select(adapter.fd() + 1, &desriptors, NULL, NULL, NULL);
                break;
            case SSL_ERROR_WANT_WRITE:
                select(adapter.fd() + 1, NULL, &desriptors, NULL, NULL);
                break;
            case SSL_ERROR_SYSCALL:
                /* unknown error */
                if (!ERR_get_error()) {
                    // no error
                    return;
                }
            default:
                throw SslHandshakeError();
        }
    }
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
        std::atexit(cleanup);
        SSL_INITIALIZED = true;
    }
}


/** \brief Open socket.
 */
template <typename HttpAdapter>
bool OpenSslAdapter<HttpAdapter>::open(const addrinfo &info,
    const std::string &host)
{
    setContext();
    ssl = SSL_new(ctx);
    setVerify(host);
    if (!certificate.empty()) {
        setCertificate();
    }
    if (!revoke.empty()) {
        setRevoke();
    }

    // open socket and create SSL
    SSL_set_connect_state(ssl);
    adapter.open(info, host);
    SSL_set_fd(ssl, adapter.fd());
    sslConnect();

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


/** \brief Set file to validate certificates.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setCertificateFile(const CertificateFile &certificate)
{
    this->certificate = certificate;
}


/** \brief Set file to manually revoke certificates.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setRevocationLists(const RevocationLists &revoke)
{
    this->revoke = revoke;
}


/** \brief Set SSL protocol.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setSslProtocol(const SslProtocol protocol)
{
    this->protocol = protocol;
}


/** \brief Change peer certificate validation.
 *
 *  \warning Does not disable URL validation.
 */
template <typename HttpAdapter>
void OpenSslAdapter<HttpAdapter>::setVerifyPeer(const VerifyPeer &peer)
{
    this->verifypeer = verifypeer;
}


}   /* lattice */

#endif
