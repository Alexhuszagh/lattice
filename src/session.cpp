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


/** \brief Set default options when making a request.
 */
void Session::setDefaultOptions()
{
    // emplace will not override user set options
    header.emplace("Host", url.host());
    header.emplace("Connection", "close");
    header.emplace("User-Agent", "lattice/" + VERSION);

    if (!cookies.empty()) {
        header.emplace("Cookie", cookies.encode());
    }
}


/** \brief Make request to server.
 */
Response Session::makeRequest(const std::string &data)
{
    // create connection and send data
    Connection connection(url.host(), url.service());
    if (timeout) {
        connection.setTimeout(timeout);
    }
    connection.send(data);

    return Response(connection.read());
}


/** \brief Null constructor.
 */
Session::Session()
{}


/** \brief Destructor.
 */
Session::~Session()
{}


/** \brief Set URL for session.
 */
void Session::setUrl(const Url &url)
{
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


/** \brief Set the DNS cache.
 *
 *  \warning The lifetime of the cache must outlive that of the session.
 */
void Session::setCache(DnsCache &cache)
{
    this->cache = &cache;
}


/** \brief Set URL for session.
 */
void Session::setOption(const Url &url)
{
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


/** \brief Set the DNS cache.
 */
void Session::setOption(DnsCache &cache)
{
    this->cache = &cache;
}


/** \brief Get response to HTTP GET request.
 */
Response Session::get()
{
    // format request
    setDefaultOptions();
    std::stringstream stream;
    stream << "GET " << url.path() << parameters.get() << " HTTP/1.1\r\n"
           << header
           << "\r\n\r\n";

    return makeRequest(stream.str());
}


/** \brief Get response to HTTP HEAD request.
 */
Response Session::head()
{
    // format request
    setDefaultOptions();
    std::stringstream stream;
    stream << "HEAD " << url.path() << parameters.get() << " HTTP/1.1\r\n"
           << header
           << "\r\n\r\n";

    return makeRequest(stream.str());
}


/** \brief Get response to HTTP POST request.
 */
Response Session::post()
{
    // format request
    setDefaultOptions();
    std::stringstream stream;
    stream << "POST " << url.path() << " HTTP/1.1\r\n"
           << header
           << parameters.post()
           << "\r\n\r\n";

    return makeRequest(stream.str());
}

}   /* lattice */
