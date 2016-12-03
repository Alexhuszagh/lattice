//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP session.
 */

#pragma once

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

public:
    Session();
    ~Session();

    void setUrl(const Url &url);
    void setParameters(const Parameters &parameters);
    void setParameters(Parameters &&parameters);
    void setTimeout(const Timeout &timeout);

    void setOption(const Url &url);
    void setOption(const Parameters &parameters);
    void setOption(Parameters &&parameters);
    void setOption(const Timeout &timeout);

//    Response delete();
    Response get();
//    Response head();
//    Response options();
//    Response patch();
    Response post();
//    Response put();
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


/** \brief Wrapper for HTTP GET request.
 */
template <typename... Ts>
Response Get(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    return session.get();
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

}   /* lattice */
