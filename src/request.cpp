//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include "lattice.hpp"

#include <sstream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Get raw bytestream from request.
 */
std::string Request::bytes()
{
    std::stringstream stream;
    std::string name;
    std::string headers = header.string();
    if (!header.host() && !url.empty() && url.absolute()) {
        // specify a default host
        headers += "Host: " + url.host() + "\r\n";
    }
    if (!header.userAgent()) {
        // specify a default user agent
        headers += "User-Agent: lattice/" + VERSION + "\r\n";
    }
    if (isUnicode(parameters) && method == POST) {
        // parameters must be UTF-8, are added to body
        headers += "Content-Type: text/plain; charset=utf-8\r\n";
    }

    switch (method) {
        case GET:
            name = "GET";
            break;
        case HEAD:
            name = "HEAD";
            break;
        case POST:
            name = "POST";
            break;
        case DELETE:
            name = "DELETE";
            break;
        case OPTIONS:
            name = "OPTIONS";
            break;
        case PATCH:
            name = "PATCH";
            break;
        case PUT:
            name = "PUT";
            break;
        case TRACE:
            name = "TRACE";
            break;
        case CONNECT:
            name = "CONNECT";
            break;
    }

    if (method == POST) {
        stream << name << " " << url.path() << " HTTP/1.1\r\n"
               << headers
               << "\r\n"
               << parameters.post()
               << "\r\n\r\n";
    } else {
        stream << name << " " << url.path() << parameters.get()
               << " HTTP/1.1\r\n"
               << headers
               << "\r\n\r\n";
    }

    return stream.str();
}


/** \brief Null constructor.
 */
Request::Request()
{}


/** \brief Destructor.
 */
Request::~Request()
{}


/** \brief Set HTTP method.
 */
void Request::setMethod(const Method method)
{
    this->method = method;
}


/** \brief Set URL for session.
 */
void Request::setUrl(const Url &url)
{
    if (url.relative()) {
        throw RelativeUrlError();
    }
    this->url = url;
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
    header["Authorization"] = "Basic " + b64Encode(auth.string());
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


/** \brief Set file to manually revoke certificates.
 */
void Request::setRevocationLists(const RevocationLists &revoke)
{
    this->revoke = revoke;
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
 *
 *  \warning HTTP1.1 **requires** a host for the session, so the URL
 *  must be absolute.
 */
void Request::setOption(const Url &url)
{
    if (url.relative()) {
        throw RelativeUrlError();
    }
    this->url = url;
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
    header["Authorization"] = "Basic " + b64Encode(auth.string());
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


/** \brief Set file to manually revoke certificates.
 */
void Request::setOption(const RevocationLists &revoke)
{
    this->revoke = revoke;
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

}   /* lattice */

