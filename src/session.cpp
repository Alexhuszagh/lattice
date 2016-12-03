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


/** \brief Set timeout for session.
 */
void Session::setTimeout(const Timeout &timeout)
{
    this->timeout = timeout;
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


/** \brief Set timeout for session.
 */
void Session::setOption(const Timeout &timeout)
{
    this->timeout = timeout;
}


/** \brief Get response to HTTP GET request.
 */
Response Session::get()
{
    // format request
    std::stringstream stream;
    stream << "GET " << url.path();
    if (parameters.size()) {
        stream << "?" << parameters;
    }
    stream << " HTTP/1.1\r\n"
           << "Host: " << url.host() << "\r\n"
           << "Connection: close\r\n"
           << "\r\n\r\n";

    Connection connection(url.host(), url.service());
    if (timeout) {
        connection.setTimeout(timeout);
    }
    connection.send(stream.str());

    return Response();
}


/** \brief Get response to HTTP POST request.
 */
Response Session::post()
{
    // TODO: now...
    return Response();
}

}   /* lattice */
