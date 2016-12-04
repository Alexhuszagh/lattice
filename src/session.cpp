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


/** \brief Open connection to URL.
 */
void Session::openConnection(Connection &connection) const
{
    connection.open(url.host(), url.service());
    if (timeout) {
        connection.setTimeout(timeout);
    }
}


/** \brief Check if the connection needs to be reset.
 */
bool Session::resetConnection(const Response &response)
{
    auto old = url;
    resetUrl(response.headers().at("location"));
    if (old.host() != url.host()) {
        header["Host"] = url.host();
        return true;
    }

    return header.closeConnection() || response.headers().closeConnection();
}


/** \brief Get new URL, from a relative or absolute identifier.
 */
void Session::resetUrl(const std::string &string)
{
    if (!string.empty() && string.front() == '/') {
        // relative URL
        this->url = Url((url.service() + "://" + url.host() + string).data());
    } else {
        // absolute url
        this->url = Url(string.data());
    }
}


/** \brief Set default options when making a request.
 */
void Session::setDefaultHeaders()
{
    // emplace will not override user set options
    header.emplace("Host", url.host());
    header.emplace("User-Agent", "lattice/" + VERSION);

    if (!cookies.empty()) {
        header.emplace("Cookie", cookies.encode());
    }
}


/** \brief Make request to server.
 */
Response Session::makeRequest()
{
    // create connection and send data
    Connection connection;
    openConnection(connection);

    do {
        // send data and get response
        connection.send(requestData());
        Response response(connection);
        if ((method = response.redirect(method)) != STOP) {
            if (resetConnection(response)) {
                // remake the connection
                connection.close();
                openConnection(connection);
            }
        } else {
            return response;
        }
    } while (redirects--);

    return Response();
}


/** \brief Get data from request.
 */
std::string Session::requestData()
{
    setDefaultHeaders();

    std::stringstream stream;
    std::string name;

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
               << header
               << "\r\n"
               << parameters.post()
               << "\r\n\r\n";
    } else {
        stream << name << " " << url.path() << parameters.get()
               << " HTTP/1.1\r\n"
               << header
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


/** \brief Set maximum redirects for request.
 */
void Session::setRedirects(const Redirects &redirects)
{
    this->redirects = redirects;
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


/** \brief Set maximum redirects for request.
 */
void Session::setOption(const Redirects &redirects)
{
    this->redirects = redirects;
}


/** \brief Set the DNS cache.
 */
void Session::setOption(DnsCache &cache)
{
    this->cache = &cache;
}


/** \brief Get response to HTTP DELETE request.
 */
Response Session::delete_()
{
    method = Method::DELETE;

    return makeRequest();
}


/** \brief Get response to HTTP GET request.
 */
Response Session::get()
{
    method = Method::GET;

    return makeRequest();
}


/** \brief Get response to HTTP HEAD request.
 */
Response Session::head()
{
    method = Method::HEAD;

    return makeRequest();
}


/** \brief Get response to HTTP OPTIONS request.
 */
Response Session::options()
{
    method = Method::OPTIONS;

    return makeRequest();
}


/** \brief Get response to HTTP PATCH request.
 */
Response Session::patch()
{
    method = Method::PATCH;

    return makeRequest();
}


/** \brief Get response to HTTP POST request.
 */
Response Session::post()
{
    method = Method::POST;

    return makeRequest();
}


/** \brief Get response to HTTP PUT request.
 */
Response Session::put()
{
    method = Method::PUT;

    return makeRequest();
}


/** \brief Get response to HTTP TRACE request.
 */
Response Session::trace()
{
    method = Method::TRACE;

    return makeRequest();
}


/** \brief Get response to HTTP CONNECT request.
 */
Response Session::connect()
{
    method = Method::CONNECT;

    return makeRequest();
}

}   /* lattice */
