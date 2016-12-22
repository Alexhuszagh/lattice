//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include "lattice.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
#   include "wincrypt.h"
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief Get name for HTTP request.
 */
std::string Request::methodName() const
{
    switch (method) {
        case GET:
            return "GET";
        case HEAD:
            return "HEAD";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        case OPTIONS:
            return "OPTIONS";
        case PATCH:
            return "PATCH";
        case PUT:
            return "PUT";
        case TRACE:
            return "TRACE";
        case CONNECT:
            return "CONNECT";
        default:
            throw std::out_of_range("HTTP request method unknown.\n");
    }
}


/** \brief Get headers for an initial HTTP request.
 */
std::stringstream Request::messageHeader() const
{
    std::stringstream data;
    data << header.string();
    if (!header.host() && url.absolute()) {
        // specify a default host
        data << "Host: " + url.host() + "\r\n";
    }
    if (!header.userAgent()) {
        // specify a default user agent
        data << "User-Agent: lattice/" + VERSION + "\r\n";
    }
    if (!header.connection()) {
        // Keep-Alive by default
        data << "Connection: keep-alive\r\n";
    }
    if (!header.accept()) {
        // accept everything by default
        data << "Accept: */*\r\n";
    }
    if (!header.cookie()) {
        // give a dummy cookie
        data << "Cookie: fake=fake_value\r\n";
    }
    if (!header.contentType() && isUnicode(parameters)) {
        // parameters must be UTF-8, are added to body
        data << "Content-Type: text/x-www-form-urlencoded; charset=utf-8\r\n";
    }

    return data;
}


/** \brief Get headers for a follow up response to an HTTP request.
 *
 *  Currently used only for digest authentication.
 */
std::stringstream Request::messageHeader(const Response &response) const
{
    auto data = messageHeader();
    if (digest) {
        try {
            auto string = response.headers().at("www-authenticate");
            DigestChallenge challenge(string);
            data << challenge.header(url, parameters, digest, response.body(), methodName());
        } catch(std::exception) {
        }
    }

    return data;
}


/** \brief Set HTTP method.
 */
void Request::setMethod(const Method method)
{
    this->method = method;
}


/** \brief Set URL for session.
 *
 *  \warning HTTP1.1 **requires** a host for the session, so the URL
 *  must be absolute.
 */
void Request::setUrl(const Url &url)
{
    this->url = url;
    if (this->url .relative()) {
        throw RelativeUrlError();
    }
}


/** \brief Set URL for session.
 */
void Request::setUrl(Url &&url)
{
    this->url = std::move(url);
    if (this->url .relative()) {
        throw RelativeUrlError();
    }
}


/** \brief Set parameters for session.
 */
void Request::setParameters(const Parameters &parameters)
{
    this->parameters = parameters;
}


/** \brief Set parameters for session.
 */
void Request::setParameters(Parameters &&parameters)
{
    this->parameters = std::move(parameters);
}


/** \brief Set header for session.
 */
void Request::setHeader(const Header &header)
{
    this->header = header;
}


/** \brief Set timeout for session.
 */
void Request::setTimeout(const Timeout &timeout)
{
    this->timeout = timeout;
}


/** \brief Set basic auth for session.
 */
void Request::setAuth(const Authentication &auth)
{
    header["Authorization"] = "Basic " + BASE64_ENCODE(auth.string());
}


/** \brief Set digest auth for session.
 */
void Request::setDigest(const Digest &digest)
{
    this->digest = digest;
}


/** \brief Set proxy for socket.
 */
void Request::setProxy(const Proxy &proxy)
{
    this->proxy = proxy;
}


/** \brief Set proxy for socket.
 */
void Request::setProxy(Proxy &&proxy)
{
    this->proxy = std::move(proxy);
}


/** \brief Set multipart message body.
 */
void Request::setMultipart(const Multipart &multipart)
{
    this->multipart = multipart;
    if (this->multipart) {
        header["Content-Type"] = this->multipart.header();
    }
}


/** \brief Set multipart message body.
 */
void Request::setMultipart(Multipart &&multipart)
{
    this->multipart = std::move(multipart);
    if (this->multipart) {
        header["Content-Type"] = this->multipart.header();
    }
}


/** \brief Set body for a POST request.
 */
void Request::setBody(const Body &body)
{
    this->parameters = static_cast<Parameters>(body);
    method = POST;
}


/** \brief Set body for a POST request.
 */
void Request::setBody(Body &&body)
{
    this->parameters = std::move(static_cast<Parameters>(body));
    method = POST;
}


/** \brief Set payload for a POST request.
 */
void Request::setPayload(const Payload &payload)
{
    this->parameters = static_cast<Parameters>(payload);
    method = POST;
}


/** \brief Set payload for a POST request.
 */
void Request::setPayload(Payload &&payload)
{
    this->parameters = std::move(static_cast<Parameters>(payload));
    method = POST;
}



/** \brief Set cookies for session.
 */
void Request::setCookies(const Cookies &cookies)
{
    header["Cookie"] = cookies.encode();
}


/** \brief Set maximum redirects for request.
 */
void Request::setRedirects(const Redirects &redirects)
{
    this->redirects = redirects;
}


/** \brief Set certificate file for SSL encryption.
 */
void Request::setCertificateFile(const CertificateFile &certificate)
{
    this->certificate = certificate;
}

/** \brief Set certificate file for SSL encryption.
 */
void Request::setCertificateFile(CertificateFile &&certificate)
{
    this->certificate = std::move(certificate);
}


/** \brief Set file to manually revoke certificates.
 */
void Request::setRevocationLists(const RevocationLists &revoke)
{
    this->revoke = revoke;
}


/** \brief Set file to manually revoke certificates.
 */
void Request::setRevocationLists(RevocationLists &&revoke)
{
    this->revoke = std::move(revoke);
}


/** \brief Set protocol for SSL encryption.
 */
void Request::setSslProtocol(const SslProtocol ssl)
{
    this->ssl = ssl;
}


/** \brief Change peer certificate validation.
 */
void Request::setVerifyPeer(const VerifyPeer &peer)
{
    this->verifypeer = verifypeer;
}


/** \brief Change peer certificate validation.
 */
void Request::setVerifyPeer(VerifyPeer &&peer)
{
    this->verifypeer = verifypeer;
}


/** \brief Set the DNS cache.
 */
void Request::setCache(const DnsCache &cache)
{
    this->cache = cache;
}


/** \brief Set HTTP method.
 */
void Request::setOption(const Method method)
{
    this->method = method;
}


/** \brief Set URL for session.
 */
void Request::setOption(const Url &url)
{
    setUrl(url);
}


/** \brief Set URL for session.
 */
void Request::setOption(Url &&url)
{
    setUrl(LATTICE_FWD(url));
}


/** \brief Set parameters for session.
 */
void Request::setOption(const Parameters &parameters)
{
    this->parameters = parameters;
}


/** \brief Set parameters for session.
 */
void Request::setOption(Parameters &&parameters)
{
    this->parameters = std::move(parameters);
}


/** \brief Set header for session.
 */
void Request::setOption(const Header &header)
{
    this->header = header;
}


/** \brief Set timeout for session.
 */
void Request::setOption(const Timeout &timeout)
{
    this->timeout = timeout;
}


/** \brief Set basic auth for session.
 */
void Request::setOption(const Authentication &auth)
{
    header["Authorization"] = "Basic " + BASE64_ENCODE(auth.string());
}


/** \brief Set digest auth for session.
 */
void Request::setOption(const Digest &digest)
{
    this->digest = digest;
}


/** \brief Set proxy for socket.
 */
void Request::setOption(const Proxy &proxy)
{
    this->proxy = proxy;
}


/** \brief Set proxy for socket.
 */
void Request::setOption(Proxy &&proxy)
{
    this->proxy = std::move(proxy);
}


/** \brief Set multipart message body.
 */
void Request::setOption(const Multipart &multipart)
{
    setMultipart(multipart);
}


/** \brief Set multipart message body.
 */
void Request::setOption(Multipart &&multipart)
{
    setMultipart(LATTICE_FWD(multipart));
}


/** \brief Set body for a POST request.
 */
void Request::setOption(const Body &body)
{
    setBody(body);
}


/** \brief Set body for a POST request.
 */
void Request::setOption(Body &&body)
{
    setBody(LATTICE_FWD(body));
}


/** \brief Set payload for a POST request.
 */
void Request::setOption(const Payload &payload)
{
    setPayload(payload);
}


/** \brief Set payload for a POST request.
 */
void Request::setOption(Payload &&payload)
{
    setPayload(LATTICE_FWD(payload));
}


/** \brief Set cookies for session.
 */
void Request::setOption(const Cookies &cookies)
{
    header["Cookie"] = cookies.encode();
}


/** \brief Set maximum redirects for request.
 */
void Request::setOption(const Redirects &redirects)
{
    this->redirects = redirects;
}


/** \brief Set certificate file for SSL encryption.
 */
void Request::setOption(const CertificateFile &certificate)
{
    this->certificate = certificate;
}


/** \brief Set certificate file for SSL encryption.
 */
void Request::setOption(CertificateFile &&certificate)
{
    this->certificate = std::move(certificate);
}


/** \brief Set file to manually revoke certificates.
 */
void Request::setOption(const RevocationLists &revoke)
{
    this->revoke = revoke;
}


/** \brief Set file to manually revoke certificates.
 */
void Request::setOption(RevocationLists &&revoke)
{
    this->revoke = std::move(revoke);
}


/** \brief Set protocol for SSL encryption.
 */
void Request::setOption(const SslProtocol ssl)
{
    this->ssl = ssl;
}


/** \brief Change peer certificate validation.
 */
void Request::setOption(const VerifyPeer &peer)
{
    this->verifypeer = verifypeer;
}


/** \brief Change peer certificate validation.
 */
void Request::setOption(VerifyPeer &&peer)
{
    this->verifypeer = verifypeer;
}


/** \brief Set the DNS cache.
 */
void Request::setOption(const DnsCache &cache)
{
    this->cache = cache;
}


/** \brief Get HTTP method.
 */
const Method Request::getMethod() const
{
    return method;
}


/** \brief Get URL for session.
 */
const Url & Request::getUrl() const
{
    return url;
}


/** \brief Set parameters for session.
 */
const Parameters & Request::getParameters() const
{
    return parameters;
}


/** \brief Set header for session.
 */
const Header & Request::getHeader() const
{
    return header;
}


/** \brief Get timeout for session.
 */
const Timeout & Request::getTimeout() const
{
    return timeout;
}


/** \brief Get digest auth for session.
 */
const Digest & Request::getDigest() const
{
    return digest;
}


/** \brief Set maximum redirects for request.
 */
const Redirects & Request::getRedirects() const
{
    return redirects;
}


/** \brief Set certificate file for SSL encryption.
 */
const CertificateFile & Request::getCertificateFile() const
{
    return certificate;
}


/** \brief Set file to manually revoke certificates.
 */
const RevocationLists & Request::getRevocationLists() const
{
    return revoke;
}


/** \brief Set protocol for SSL encryption.
 */
const SslProtocol Request::getSslProtocol() const
{
    return ssl;
}


/** \brief Get peer certificate validation.
 */
const VerifyPeer Request::getVerifyPeer() const
{
    return verifypeer;
}


/** \brief Get the DNS cache.
 */
const DnsCache Request::getDnsCache() const
{
    return cache;
}



}   /* lattice */

