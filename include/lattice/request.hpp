//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#pragma once

#include "adapter.hpp"
#include "auth.hpp"
#include "connection.hpp"
#include "cookie.hpp"
#include "digest.hpp"
#include "dns.hpp"
#include "header.hpp"
#include "method.hpp"
#include "multipart.hpp"
#include "parameter.hpp"
#include "proxy.hpp"
#include "redirect.hpp"
#include "response.hpp"
#include "ssl.hpp"
#include "timeout.hpp"
#include "url.hpp"
#include "util.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief HTTP request object.
 */
class Request
{
protected:
    Url url;
    Parameters parameters;
    Header header;
    Digest digest;
    Multipart multipart;
    Proxy proxy;
    Timeout timeout;
    Redirects redirects;
    CertificateFile certificate;
    RevocationLists revoke;
    Method method = static_cast<Method>(0);
    SslProtocol ssl = static_cast<SslProtocol>(0);
    VerifyPeer verifypeer;
    DnsCache cache = nullptr;

    std::stringstream messageHeader() const;
    std::stringstream messageHeader(const Response &response) const;

public:
    Request();
    ~Request();

    // EXPLICIT OPTIONS
    void setMethod(const Method method);
    void setUrl(const Url &url);
    void setUrl(Url &&url);
    void setParameters(const Parameters &parameters);
    void setParameters(Parameters &&parameters);
    void setHeader(const Header &header);
    void setTimeout(const Timeout &timeout);
    void setAuth(const Authentication &auth);
    void setDigest(const Digest &digest);
    void setMultipart(const Multipart &multipart);
    void setMultipart(Multipart &&multipart);
    void setProxy(const Proxy &proxy);
    void setProxy(Proxy &&proxy);
    void setBody(const Body &body);
    void setBody(Body &&body);
    void setPayload(const Payload &payload);
    void setPayload(Payload &&payload);
    void setCookies(const Cookies &cookies);
    void setRedirects(const Redirects &redirects);
    void setCertificateFile(const CertificateFile &certificate);
    void setCertificateFile(CertificateFile &&certificate);
    void setRevocationLists(const RevocationLists &revoke);
    void setRevocationLists(RevocationLists &&revoke);
    void setSslProtocol(const SslProtocol ssl);
    void setVerifyPeer(const VerifyPeer &peer);
    void setVerifyPeer(VerifyPeer &&peer);
    void setCache(const DnsCache &cache);

    // FORWARDING OPTIONS
    void setOption(const Method method);
    void setOption(const Url &url);
    void setOption(Url &&url);
    void setOption(const Parameters &parameters);
    void setOption(Parameters &&parameters);
    void setOption(const Header &header);
    void setOption(const Timeout &timeout);
    void setOption(const Authentication &auth);
    void setOption(const Digest &digest);
    void setOption(const Multipart &multipart);
    void setOption(Multipart &&multipart);
    void setOption(const Proxy &proxy);
    void setOption(Proxy &&proxy);
    void setOption(const Body &body);
    void setOption(Body &&body);
    void setOption(const Payload &payload);
    void setOption(Payload &&payload);
    void setOption(const Cookies &cookies);
    void setOption(const Redirects &redirects);
    void setOption(const CertificateFile &certificate);
    void setOption(CertificateFile &&certificate);
    void setOption(const RevocationLists &revoke);
    void setOption(RevocationLists &&revoke);
    void setOption(const SslProtocol ssl);
    void setOption(const VerifyPeer &peer);
    void setOption(VerifyPeer &&peer);
    void setOption(const DnsCache &cache);

    // ACCESS
    const Method getMethod() const;
    const Url & getUrl() const;
    const Parameters & getParameters() const;
    const Header & getHeader() const;
    const Timeout & getTimeout() const;
    const Digest & getDigest() const;
    const Redirects & getRedirects() const;
    const CertificateFile & getCertificateFile() const;
    const RevocationLists & getRevocationLists() const;
    const SslProtocol getSslProtocol() const;
    const VerifyPeer getVerifyPeer() const;
    const DnsCache getDnsCache() const;

    // CONNECTIONS
    template <typename... Ts>
    std::string message(Ts&&... ts) const;
    std::string methodName() const;

    Response exec();

    template <typename Connection>
    void open(Connection &connection) const;

    template <typename Connection>
    void reset(Connection &connection,
        const Response &response);

    template <typename Connection>
    Response exec(Connection &connection);
};


// FUNCTIONS
// ---------


/** \brief Set option for HTTP request.
 */
template <typename T>
void setOption(Request& request, T&& t)
{
    request.setOption(FORWARD(t));
}


/** \brief Set options for HTTP request.
 */
template <
    typename T,
    typename... Ts
>
void setOption(Request& request,
    T &&t,
    Ts&&... ts)
{
    setOption(request, FORWARD(t));
    setOption(request, FORWARD(ts)...);
}


/** \brief Wrapper for HTTP DELETE request.
 */
template <typename... Ts>
Response Delete(Ts&&... ts)
{
    Request request;
    setOption(request, DELETE, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Get(Ts&&... ts)
{
    Request request;
    setOption(request, GET, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP HEAD request.
 */
template <typename... Ts>
Response Head(Ts&&... ts)
{
    Request request;
    setOption(request, HEAD, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP OPTIONS request.
 */
template <typename... Ts>
Response Options(Ts&&... ts)
{
    Request request;
    setOption(request, OPTIONS, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP PATCH request.
 */
template <typename... Ts>
Response Patch(Ts&&... ts)
{
    Request request;
    setOption(request, PATCH, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Post(Ts&&... ts)
{
    Request request;
    setOption(request, POST, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP PUT request.
 */
template <typename... Ts>
Response Put(Ts&&... ts)
{
    Request request;
    setOption(request, PUT, FORWARD(ts)...);

    return request.exec();
}


/** \brief Wrapper for HTTP TRACE request.
 */
template <typename... Ts>
Response Trace(Ts&&... ts)
{
    Request request;
    setOption(request, TRACE, FORWARD(ts)...);

    return request.exec();
}


// IMPLEMENTATION
// --------------


/** \brief Get message for request.
 */
template <typename... Ts>
std::string Request::message(Ts&&... ts) const
{
    std::stringstream stream;

    // get our formatted body
    std::string body;
    if (method == POST && parameters) {
        body += parameters;
    } else if (multipart) {
        body += multipart.string();
    }

    // get formatted headers
    auto headers = messageHeader(FORWARD(ts)...);
    if (!body.empty()) {
        headers << "Content-Length: " << body.size() << "\r\n";
    }

    // get first line
    if (method == POST) {
        stream << methodName() << " " << url.path()
               << " HTTP/1.1\r\n"
               << headers.str()
               << "\r\n" << body;
    } else {
        stream << methodName() << " " << url.path() << parameters.get()
               << " HTTP/1.1\r\n"
               << headers.str()
               << "\r\n" << body;
    }

    // end message with double CRLF
    stream << "\r\n";
    if (!body.empty()) {
        stream << "\r\n";
    }

    return stream.str();
}


/** \brief Make request to server.
 *
 *  To avoid compiling external libraries into lattice, misuse inline
 *  to keep this in the header.
 */
inline Response Request::exec()
{
    auto service = url.service();
    if (service == "http") {
        HttpConnection connection;
        return exec(connection);
    } else if (service == "https") {
        HttpsConnection connection;
        return exec(connection);
    } else {
        throw NetworkSchemeError(service);
    }

    return Response();
}


/** \brief Execute request to server.
 */
template <typename Connection>
Response Request::exec(Connection &connection)
{
    open(connection);
    Response response;
    do {
        connection.write(message());
        response = Response(connection);
        if (response.unauthorized() && digest) {
            // using digest authentication
            connection.write(message(response));
            response = Response(connection);
            return response;
        } else if ((method = response.redirect(method)) != STOP) {
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
void Request::open(Connection &connection) const
{
    // set options
    connection.setVerifyPeer(verifypeer);
    if (certificate) {
        connection.setCertificateFile(certificate);
    }
    if (revoke) {
        connection.setRevocationLists(revoke);
    }
    if (FROM_ENUM(ssl)) {
        connection.setSslProtocol(ssl);
    }
    if (cache) {
        connection.setCache(cache);
    }

    // open and set timeout
    if (!proxy) {
        connection.open(url);
    } else {
        connection.open(Url(proxy));
    }
    if (timeout) {
        connection.setTimeout(timeout);
    }
}


/** \brief Reset parameters and connection to server.
 */
template <typename Connection>
void Request::reset(Connection &connection,
    const Response &response)
{
    // check if we need to reset connection
    bool reconnect = header.closeConnection();
    reconnect |= response.headers().closeConnection();

    Url newurl(response.headers().at("location"));
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
