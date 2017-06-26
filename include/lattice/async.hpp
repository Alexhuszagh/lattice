//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#pragma once

#include "request.hpp"
#include "response.hpp"
#include "util.hpp"

#include <chrono>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>


namespace lattice
{
// OBJECTS
// -------

typedef std::vector<Response> Responses;


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
    Pool() = default;
    Pool(const Pool &other) = delete;
    Pool & operator=(const Pool&) = delete;
    Pool(Pool&&) = default;
    Pool & operator=(Pool&&) = default;

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

    Responses perform();

    template <typename Duration>
    typename std::enable_if<std::is_integral<Duration>::value, Response>::type
    next(const Duration seconds = 1);

    template <typename Duration>
    typename std::enable_if<IsDerived<std::chrono::duration, Duration>::value, Response>::type
    next(const Duration &duration = std::chrono::seconds(1));

    explicit operator bool() const;
};


// IMPLEMENTATION
// --------------


/** \brief Initialize thread with GET request.
 */
template <typename... Ts>
void Pool::get(Ts&&... ts)
{
    Request request;
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
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
    setOption(request, LATTICE_FWD(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](Request &&request) {
        request.setMethod(TRACE);
        return request.exec();
    }, std::move(request)));
}


/** \brief Block until next query is ready (using a seconds overload).
 */
template <typename Duration>
typename std::enable_if<std::is_integral<Duration>::value, Response>::type
Pool::next(const Duration seconds)
{
    return next(std::chrono::seconds(seconds));
}


/** \brief Block until next query is ready.
 */
template <typename Duration>
typename std::enable_if<IsDerived<std::chrono::duration, Duration>::value, Response>::type
Pool::next(const Duration &duration)
{
    // set our starting timepoint
    typedef std::chrono::high_resolution_clock Clock;

    auto now = Clock::now();
    auto stop = now + duration;

    auto it = futures.begin();
    while (!futures.empty() && now < stop) {
        auto status = it->wait_for(std::chrono::milliseconds(50));
        if (status == std::future_status::ready) {
            auto response = it->get();
            futures.erase(it);
            return response;
        }

        // increment our conditions
        now = Clock::now();
        ++it;
        if (it == futures.end()) {
            it = futures.begin();
        }
    }

    return Response();
}


}   /* lattice */
