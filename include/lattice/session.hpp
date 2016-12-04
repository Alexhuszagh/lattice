//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP session.
 */

#pragma once

#include "cache.hpp"
#include "cookie.hpp"
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
    Method method;
    DnsCache *cache = nullptr;

    void openConnection(Connection &connection) const;
    bool resetConnection(const Response &response);
    void resetUrl(const std::string &string);
    void setDefaultHeaders();
    std::string requestData();
    Response makeRequest();

public:
    Session();
    ~Session();

    void setUrl(const Url &url);
    void setParameters(const Parameters &parameters);
    void setParameters(Parameters &&parameters);
    void setHeader(const Header &header);
    void setTimeout(const Timeout &timeout);
    void setCookies(const Cookies &cookies);
    void setRedirects(const Redirects &redirects);
    void setCache(DnsCache &cache);

    void setOption(const Url &url);
    void setOption(const Parameters &parameters);
    void setOption(Parameters &&parameters);
    void setOption(const Header &header);
    void setOption(const Timeout &timeout);
    void setOption(const Cookies &cookies);
    void setOption(const Redirects &redirects);
    void setOption(DnsCache &cache);

    Response delete_();
    Response get();
    Response head();
    Response options();
    Response patch();
    Response post();
    Response put();
    Response trace();
    Response connect();
};


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
    setOption(session, FORWARD(ts)...);

    return session.delete_();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Get(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.get();
}


/** \brief Wrapper for HTTP HEAD request.
 */
template <typename... Ts>
Response Head(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.head();
}


/** \brief Wrapper for HTTP OPTIONS request.
 */
template <typename... Ts>
Response Options(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.options();
}


/** \brief Wrapper for HTTP PATCH request.
 */
template <typename... Ts>
Response Patch(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.patch();
}


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Post(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.post();
}


/** \brief Wrapper for HTTP PUT request.
 */
template <typename... Ts>
Response Put(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.put();
}


/** \brief Wrapper for HTTP TRACE request.
 */
template <typename... Ts>
Response Trace(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.trace();
}


/** \brief Wrapper for HTTP CONNECT request.
 */
template <typename... Ts>
Response Connect(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.connect();
}


}   /* lattice */
