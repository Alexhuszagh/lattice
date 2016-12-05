//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#pragma once

#include "mutex.hpp"
#include "response.hpp"
#include "session.hpp"

#include <future>
#include <thread>
#include <vector>


namespace lattice
{
// OBJECTS
// -------


/** \brief Thread pool for asynchronous requests.
 *
 *  \warning The asynchronous approach is rudimentary, with a
 *  single thread per socket. This is not intended to replace
 *  a true, asynchronous library like Boost::asio or Casablanca.
 */
class Pool
{
protected:
    std::vector<std::future<Response>> futures;

public:
    Pool();
    Pool(const Pool &other) = delete;
    ~Pool();

    template <typename... Ts>
    void get(Ts&&... ts);

    template <typename... Ts>
    void head(Ts&&... ts);

    template <typename... Ts>
    void post(Ts&&... ts);

    std::vector<Response> perform();
};


// IMPLEMENTATION
// --------------


/** \brief Initialize thread with GET request.
 */
template <typename... Ts>
void Pool::get(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    futures.emplace_back(std::async([](Session &&session) {
        session.setMethod(GET);
        return session.exec();
    }, std::move(session)));
}


/** \brief Initialize thread with HEAD request.
 */
template <typename... Ts>
void Pool::head(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    futures.emplace_back(std::async([](Session &&session) {
        session.setMethod(HEAD);
        return session.exec();
    }, std::move(session)));
}


/** \brief Initialize thread with POST request.
 */
template <typename... Ts>
void Pool::post(Ts&&... ts)
{
    Session session;
    setOption(session, FORWARD(ts)...);

    futures.emplace_back(std::async([](Session &&session) {
        session.setMethod(POST);
        return session.exec();
    }, std::move(session)));
}


}   /* lattice */
