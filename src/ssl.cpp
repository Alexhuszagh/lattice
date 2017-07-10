//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTPS options and validation.
 */

#include <lattice/ssl.hpp>

#include <pycpp/casemap.h>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4715)
#endif


namespace lattice
{
// OBJECTS
// -------


std::string certificate_file_t::suffix() const
{
    const size_t index = find_last_of('.');
    if (index != 0 || index != std::string::npos) {
        return substr(index+1);
    }
    return "";
}


certificate_format_t certificate_file_t::format() const
{
    auto data = ascii_tolower(suffix());
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


certificate_file_t::operator bool() const
{
    return !empty();
}


revocation_lists_t::operator bool() const
{
    return !empty();
}


verify_peer_t::verify_peer_t(const bool verify):
    verify(verify)
{}


verify_peer_t::operator bool() const
{
    return verify;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
