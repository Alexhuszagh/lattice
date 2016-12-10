//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Weakly encrypted authentication for requests.
 */

#include "lattice.hpp"

#include <algorithm>
#include <iomanip>



namespace lattice
{
// OBJECTS
// -------


/** \brief Hash lower-case version of string.
 */
size_t CaseInsensitiveHash::operator()(std::string string) const
{
    ::lattice::tolower(string);
    return std::hash<std::string>()(string);
}


/** \brief Initialize from a quality of protection code.
 */
QualityOfProtection::QualityOfProtection(const std::string &qop):
    std::vector<std::string>(split(qop, ","))
{
    for (auto &item: *this) {
        trim(item);
    }
}


/** \brief Check if the QOP's directive includes "auth".
 */
bool QualityOfProtection::auth() const
{
    return std::any_of(begin(), end(), [](const value_type &data) {
        return data == "auth";
    });
}


/** \brief Check if the QOP's directive includes "auth-int".
 */
bool QualityOfProtection::authInt() const
{
    return std::any_of(begin(), end(), [](const value_type &data) {
        return data == "auth-int";
    });
}


/** \brief Check if any quality of protection codes have been set.
 */
QualityOfProtection::operator bool() const
{
    return !empty();
}


/** \brief Initialize digest from challenge string.
 */
DigestChallenge::DigestChallenge(const std::string &string)
{
    auto data = safesplit(string.substr(7), ',', '"', '\\');
    for (auto &value: data) {
        trim(value);
        const char *begin = value.data();
        const char *ptr = strchr(begin, '=');
        if (ptr) {
            std::string first(begin, ptr);
            std::string second(ptr+1);

            // remove quotes
            if (second.size() && second.front() == '"') {
                second.erase(0, 1);
            }
            if (second.size() && second.back() == '"') {
                second.erase(second.size() - 1);
            }
            emplace(first, second);
        }
    }
}


/** \brief Get realm for challenge.
 */
const std::string & DigestChallenge::realm() const
{
    return at("realm");
}


/** \brief Get nonce for challenge.
 */
const std::string & DigestChallenge::nonce() const
{
    return at("nonce");
}


/** \brief Get client nonce for challenge.
 */
const std::string & DigestChallenge::cnonce()
{
    if (clientNonce.empty()) {
        clientNonce = HEX<uint32_t>(sysrandom(8));
    }
    return clientNonce;
}


/** \brief Get nonce counter (request number).
 */
std::string DigestChallenge::nc() const
{
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(8) << std::hex << nonceCounter;

    return stream.str();
}


/** \brief Get hashing algorithm for challenge.
 */
DigestAlgorithm DigestChallenge::algorithm() const
{
    auto it = find("algorithm");
    if (it == end()) {
        return MD5_DIGEST;
    }

    std::string data = it->second;
    ::lattice::tolower(data);

    if (data == "md5") {
        return MD5_DIGEST;
    } else if (data == "md5-sess") {
        return MD5_SESS_DIGEST;
    } else if (data == "sha") {
        return SHA1_DIGEST;
    }
    throw UnknownDigestAlgorithm();
}


/** \brief Get quality of protection directives.
 */
QualityOfProtection DigestChallenge::qop() const
{
    auto it = find("qop");
    if (it != end()) {
        return QualityOfProtection(it->second);
    }
    return QualityOfProtection{};
}


/** \brief Create header from digest auth and challenge.
 */
std::string DigestChallenge::header(const Url &url,
    const Parameters &parameters,
    const Digest &digest,
    const std::string &body,
    const std::string &method)
{
    // get string to hash
    auto quality = qop();
    auto path = url.path() + parameters.get();
    std::string a1 = digest.username + ":" + realm() + ":" + digest.password;
    std::string a2 = method + ":" + path;
    if (quality.authInt()) {
        a2 += ":" + body;
    }

    // get our initial hash values
    auto algo = algorithm();
    auto hasher = (algo == SHA1_DIGEST) ? SHA1_HEX : MD5_HEX;
    std::string ha1 = hasher(a1);
    std::string ha2 = hasher(a2);

    // MD5 session digests also hash the nonce
    if (algo == MD5_SESS_DIGEST) {
        ha1 = hasher(ha1 + ":" + nonce() + ":" + cnonce());
    }

    // create the hex digest
    ++nonceCounter;
    std::string response;
    if (quality.empty()) {
        response = hasher(ha1 + ":" + nonce() + ":" + ha2);
    } else {
        response = hasher(ha1 + ":" + nonce() + ":" + nc() + ":" + cnonce() + ":auth:" + ha2);
    }

    // create our header
    std::ostringstream header;
    header << "Authorization: Digest " << "username=\"" + digest.username
           << "\", realm=\"" << realm()
           << "\", nonce=\"" << nonce()
           << "\", uri=\"" << path
           << "\", response=\"" << response << "\"";

    // optional arguments
    if (quality) {
        header << ", qop=\"" << join(quality, ",")
               << "\", nc=" << nc()
               << ", cnonce=\"" << cnonce() << "\"";
    }
    if (find("opaque") != end()) {
        header << ", opaque=\"" << at("opaque") << "\"";
    }
    if (find("algorithm") != end()) {
        header << ", algorithm=\"" << at("algorithm") << "\"";
    }

    header << "\r\n";

    return header.str();
}


/** \brief Check if a challenge has been set.
 */
DigestChallenge::operator bool() const
{
    return !empty();
}


}   /* lattice */
