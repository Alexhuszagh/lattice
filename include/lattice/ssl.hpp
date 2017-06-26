//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTPS options and validation.
 */

#pragma once

#include <string>


namespace lattice
{
// OBJECTS
// -------


/** SSL encryption protocl.
 */
enum SslProtocol
{
    TLS     = 0,
    SSL_V23 = 1,
    TLS_V12 = 2,
    TLS_V11 = 3,
    TLS_V1  = 4,
    SSL_V3  = 5,
};


/** \brief Various file formats for the certificate chain.
 */
enum CertificateFormat
{
    PEM         = 1,
    ASN1        = 2,
    SSL_ENGINE  = 3,
    PKCS8       = 4,
    PKCS12      = 5,
};


/** \brief Holds path to certificate bundle.
 */
class CertificateFile: public std::string
{
protected:
    typedef std::string Base;

public:
    using Base::Base;

    std::string suffix() const;
    CertificateFormat format() const;

    explicit operator bool() const;
};


/** \brief Holds path to certificate revocation lists.
 */
class RevocationLists: public std::string
{
protected:
    typedef std::string Base;

public:
    using Base::Base;

    explicit operator bool() const;
};


/** \brief Verify peer for SSL/TLS connections.
 */
struct VerifyPeer
{
    bool verify = true;

    VerifyPeer() = default;
    VerifyPeer(const VerifyPeer &other) = default;
    VerifyPeer & operator=(const VerifyPeer&) = default;
    VerifyPeer(VerifyPeer&&) = default;
    VerifyPeer & operator=(VerifyPeer&&) = default;

    VerifyPeer(const bool verify);

    explicit operator bool() const;
};


}   /* lattice */
