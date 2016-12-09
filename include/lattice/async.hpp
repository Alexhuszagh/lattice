//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#pragma once

#include "request.hpp"
#include "response.hpp"
#include "util.hpp"

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
    void options(Ts&&... ts);

    template <typename... Ts>
    void patch(Ts&&... ts);

    template <typename... Ts>
    void post(Ts&&... ts);

    template <typename... Ts>
    void put(Ts&&... ts);

    template <typename... Ts>
    void trace(Ts&&... ts);

    std::vector<Response> perform();
};


// IMPLEMENTATION
// --------------


/** \brief Initialize thread with GET request.
 */
template <typename... Ts>
void Pool::get(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(GET);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with HEAD request.
 */
template <typename... Ts>
void Pool::head(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(HEAD);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with OPTIONS request.
 */
template <typename... Ts>
void Pool::options(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(OPTIONS);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with PATCH request.
 */
template <typename... Ts>
void Pool::patch(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(PATCH);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with POST request.
 */
template <typename... Ts>
void Pool::post(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(POST);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with PUT request.
 */
template <typename... Ts>
void Pool::put(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(PUT);
        return request.exec();
    }, std::move(request)));
}


/** \brief Initialize thread with TRACE request.
 */
template <typename... Ts>
void Pool::trace(Ts&&... ts)
{
    Request request;
    setOption(request, FORWARD(ts)...);

    futures.emplace_back(std::async([](Request &&request) {
        request.setMethod(TRACE);
        return request.exec();
    }, std::move(request)));
}


}   /* lattice */
