//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Weakly encrypted authentication for requests.
 *
 *  \warning Digest authentication, although popular, is insecure
 *  and should be considered deprecated and broken beyond repair.
 */

#pragma once

#include "auth.hpp"
#include "crypto.hpp"
#include "parameter.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>


namespace lattice
{
// OBJECTS
// -------


/** \brief Case-insensitive hash for ASCII.
 */
struct CaseInsensitiveHash
{
    size_t operator()(std::string string) const;
};


/** \brief Authorization string for requests.
 */
struct Digest: public Authentication
{
    using Authentication::Authentication;
};


/** \brief Quality of protection directive.
 */
class QualityOfProtection: public std::vector<std::string>
{
protected:
    typedef std::vector<std::string> Base;

public:
    using Base::Base;
    QualityOfProtection(const std::string &qop);

    bool auth() const;
    bool authInt() const;
};


/** \brief Authenticate challenge sent from the server.
 *
 *  \format
 *      Digest nonce="42148a112dd92b7e5b6ac4769c2a6693", opaque="35fa82343c10f5a83c7d9b8bb29d8518", realm="me@kennethreitz.com", qop=auth
 */
class DigestChallenge: public std::unordered_map<
        std::string,
        std::string,
        CaseInsensitiveHash
    >
{
protected:
    typedef std::unordered_map<
        std::string,
        std::string,
        CaseInsensitiveHash
    > Base;

    uint64_t nonceCounter = 0;
    std::string clientNonce;

public:
    using Base::Base;
    DigestChallenge(const std::string &string);

    // DATA
    const std::string & realm() const;
    const std::string & nonce() const;
    const std::string & cnonce();
    std::string nc() const;
    DigestAlgorithm algorithm() const;
    QualityOfProtection qop() const;
    std::string header(const Url &url,
        const Parameters &parameters,
        const Digest &digest,
        const std::string &body,
        const std::string &method);
};


}   /* lattice */
