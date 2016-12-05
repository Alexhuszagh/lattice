//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP session.
 */

#pragma once

#include "adapter.hpp"
#include "certificate.hpp"
#include "connection.hpp"
#include "cookie.hpp"
#include "dns.hpp"
#include "header.hpp"
#include "method.hpp"
#include "parameter.hpp"
#include "redirect.hpp"
#include "response.hpp"
#include "timeout.hpp"
#include "url.hpp"
#include "util.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief HTTP session object.
 */
class Session
{
protected:
    Url url;
    Parameters parameters;
    Timeout timeout;
    Header header;
    Cookies cookies;
    Redirects redirects;
    CertificateFile certificate;
    Method method = static_cast<Method>(0);
    SslProtocol ssl = static_cast<SslProtocol>(0);
    DnsCache cache = nullptr;

    std::string request();

    template <typename Connection>
    Response exec(Connection &connection);

    template <typename Connection>
    void open(Connection &connection) const;

    template <typename Connection>
    void reset(Connection &connection,
        const Response &response);

public:
    Session();
    ~Session();

    void setMethod(const Method method);
    void setUrl(const Url &url);
    void setParameters(const Parameters &parameters);
    void setParameters(Parameters &&parameters);
    void setHeader(const Header &header);
    void setTimeout(const Timeout &timeout);
    void setCookies(const Cookies &cookies);
    void setRedirects(const Redirects &redirects);
    void setCertificateFile(const CertificateFile &certificate);
    void setSslProtocol(const SslProtocol ssl);
    void setCache(const DnsCache &cache);

    void setOption(const Method method);
    void setOption(const Url &url);
    void setOption(const Parameters &parameters);
    void setOption(Parameters &&parameters);
    void setOption(const Header &header);
    void setOption(const Timeout &timeout);
    void setOption(const Cookies &cookies);
    void setOption(const Redirects &redirects);
    void setOption(const CertificateFile &certificate);
    void setOption(const SslProtocol ssl);
    void setOption(const DnsCache &cache);

    Response exec();
};


// FUNCTIONS
// ---------


/** \brief Set option for HTTP session.
 */
template <typename T>
void setOption(Session& session, T&& t)
{
    session.setOption(FORWARD(t));
}


/** \brief Set options for HTTP session.
 */
template <
    typename T,
    typename... Ts
>
void setOption(Session& session,
    T&& t,
    Ts&&... ts)
{
    setOption(session, FORWARD(t));
    setOption(session, FORWARD(ts)...);
}


/** \brief Wrapper for HTTP DELETE request.
 */
template <typename... Ts>
Response Delete(Ts&&... ts)
{
    Session session;
    setOption(session, DELETE, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Get(Ts&&... ts)
{
    Session session;
    setOption(session, GET, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP HEAD request.
 */
template <typename... Ts>
Response Head(Ts&&... ts)
{
    Session session;
    setOption(session, HEAD, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP OPTIONS request.
 */
template <typename... Ts>
Response Options(Ts&&... ts)
{
    Session session;
    setOption(session, OPTIONS, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP PATCH request.
 */
template <typename... Ts>
Response Patch(Ts&&... ts)
{
    Session session;
    setOption(session, PATCH, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Post(Ts&&... ts)
{
    Session session;
    setOption(session, POST, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP PUT request.
 */
template <typename... Ts>
Response Put(Ts&&... ts)
{
    Session session;
    setOption(session, PUT, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP TRACE request.
 */
template <typename... Ts>
Response Trace(Ts&&... ts)
{
    Session session;
    setOption(session, TRACE, FORWARD(ts)...);

    return session.exec();
}


/** \brief Wrapper for HTTP CONNECT request.
 */
template <typename... Ts>
Response Connect(Ts&&... ts)
{
    Session session;
    setOption(session, CONNECT, FORWARD(ts)...);

    return session.exec();
}


// IMPLEMENTATION
// --------------


/** \brief Make request to server.
 *
 *  For API reasons, this must occur in the header.
 */
inline Response Session::exec()
{
    if (url.service() == "http") {
        Connection<HttpAdapter> connection;
        return exec(connection);
    } else if (url.service() == "https") {
        Connection<SslAdapter> connection;
        return exec(connection);
    }

    return Response();
}


/** \brief Execute request to server.
 */
template <typename Connection>
Response Session::exec(Connection &connection)
{
    open(connection);
    Response response;
    do {
        connection.send(request());
        response = Response(connection);
        if ((method = response.redirect(method)) != STOP) {
            reset(connection, response);
        } else {
            break;
        }
    } while (redirects--);

    return response;
}


/** \brief Open connection to server.
 */
template <typename Connection>
void Session::open(Connection &connection) const
{
    // set options
    if (!certificate.empty()) {
        connection.setCertificateFile(certificate);
    }
    if (FROM_ENUM(ssl)) {
        connection.setSslProtocol(ssl);
    }
    if (cache) {
        connection.setCache(cache);
    }

    // open and set timeout
    connection.open(url);
    if (timeout) {
        connection.setTimeout(timeout);
    }
}


/** \brief Reset parameters and connection to server.
 */
template <typename Connection>
void Session::reset(Connection &connection,
    const Response &response)
{
    // check if we need to reset connection
    bool reconnect = header.closeConnection();
    reconnect |= response.headers().closeConnection();

    Url newurl(response.headers().at("location").data());
    if (newurl.absolute()) {
        // reconnect if the service or host changes
        reconnect |= url.service() != newurl.service();
        reconnect |= url.host() != newurl.host();
        url = newurl;
    } else {
        url.setPath(newurl.path());
    }

    // reset connection
    if (reconnect) {
        connection.close();
        open(connection);
    }
}


}   /* lattice */
