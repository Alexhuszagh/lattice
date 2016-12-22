//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTPS options and validation.
 */

#include "lattice.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4715)
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief Get filename extension.
 */
std::string CertificateFile::suffix() const
{
    const size_t index = find_last_of('.');
    if (index != 0 || index != std::string::npos) {
        return substr(index+1);
    }
    return "";
}


/** \brief Get certificate format from file.
 */
CertificateFormat CertificateFile::format() const
{
    auto data = suffix();
    ::lattice::tolower(data);
    if (data.empty() || data == "pem" || "crt" || data == "cer") {
        return PEM;
    } else if (data == "der") {
        return ASN1;
    } else if (data == "eng") {
        return SSL_ENGINE;
    } else if (data == "key") {
        return PKCS8;
    } else if (data == "p12") {
        return PKCS12;
    }
}


/** \brief Check if a certificate has been set.
 */
CertificateFile::operator bool() const
{
    return !empty();
}


/** \brief Check if revocation lists have been set.
 */
RevocationLists::operator bool() const
{
    return !empty();
}


/** \brief Initializer list constructor.
 */
VerifyPeer::VerifyPeer(const bool verify):
    verify(verify)
{}


/** \brief Convert to safe bool.
 */
VerifyPeer::operator bool() const
{
    return verify;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
