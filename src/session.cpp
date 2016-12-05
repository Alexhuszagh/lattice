//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Network session.
 */

#include "lattice.hpp"

#include <sstream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Get data from request.
 */
std::string Session::request()
{
    std::stringstream stream;
    std::string name;
    std::string headers = header.string();
    if (!header.host()) {
        // specify a default host
        headers += "Host: " + url.host() + "\r\n";
    }
    if (!header.userAgent()) {
        // specify a default user agent
        headers += "User-Agent: lattice/" + VERSION + "\r\n";
    }
    if (!cookies.empty()) {
        // add cookies to header
        headers += "Cookie: " + cookies.encode() + "\r\n";
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
Session::Session()
{}


/** \brief Destructor.
 */
Session::~Session()
{}


/** \brief Set HTTP method.
 */
void Session::setMethod(const Method method)
{
    this->method = method;
}


/** \brief Set URL for session.
 */
void Session::setUrl(const Url &url)
{
    if (url.relative()) {
        throw RelativeUrlError();
    }
    this->url = url;
}


/** \brief Set parameters for session.
 */
void Session::setParameters(const Parameters &parameters)
{
    this->parameters = parameters;
}


/** \brief Set parameters for session.
 */
void Session::setParameters(Parameters &&parameters)
{
    this->parameters = std::move(parameters);
}


/** \brief Set header for session.
 */
void Session::setHeader(const Header &header)
{
    this->header = header;
}


/** \brief Set timeout for session.
 */
void Session::setTimeout(const Timeout &timeout)
{
    this->timeout = timeout;
}


/** \brief Set cookies for session.
 */
void Session::setCookies(const Cookies &cookies)
{
    this->cookies = cookies;
}


/** \brief Set maximum redirects for request.
 */
void Session::setRedirects(const Redirects &redirects)
{
    this->redirects = redirects;
}


/** \brief Set certificate file for SSL encryption.
 */
void Session::setCertificateFile(const CertificateFile &certificate)
{
    this->certificate = certificate;
}


/** \brief Set protocol for SSL encryption.
 */
void Session::setSslProtocol(const SslProtocol ssl)
{
    this->ssl = ssl;
}


/** \brief Set the DNS cache.
 */
void Session::setCache(const DnsCache &cache)
{
    this->cache = cache;
}


/** \brief Set HTTP method.
 */
void Session::setOption(const Method method)
{
    this->method = method;
}


/** \brief Set URL for session.
 */
void Session::setOption(const Url &url)
{
    if (url.relative()) {
        throw RelativeUrlError();
    }
    this->url = url;
}


/** \brief Set parameters for session.
 */
void Session::setOption(const Parameters &parameters)
{
    this->parameters = parameters;
}


/** \brief Set parameters for session.
 */
void Session::setOption(Parameters &&parameters)
{
    this->parameters = std::move(parameters);
}


/** \brief Set header for session.
 */
void Session::setOption(const Header &header)
{
    this->header = header;
}


/** \brief Set timeout for session.
 */
void Session::setOption(const Timeout &timeout)
{
    this->timeout = timeout;
}


/** \brief Set cookies for session.
 */
void Session::setOption(const Cookies &cookies)
{
    this->cookies = cookies;
}


/** \brief Set maximum redirects for request.
 */
void Session::setOption(const Redirects &redirects)
{
    this->redirects = redirects;
}


/** \brief Set certificate file for SSL encryption.
 */
void Session::setOption(const CertificateFile &certificate)
{
    this->certificate = certificate;
}


/** \brief Set protocol for SSL encryption.
 */
void Session::setOption(const SslProtocol ssl)
{
    this->ssl = ssl;
}


/** \brief Set the DNS cache.
 */
void Session::setOption(const DnsCache &cache)
{
    this->cache = cache;
}

}   /* lattice */

