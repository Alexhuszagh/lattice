//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Timeout for requests.
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <limits>
#include <stdexcept>

#if defined(min)
#   undef min
#endif
#if defined(max)
#   undef max
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief Timeout for a request object.
 */
class Timeout
{
protected:
    std::chrono::milliseconds timeout = std::chrono::milliseconds(0);

    template <typename Duration>
    void checkLimits(const Duration &duration) const;

public:
    Timeout() = default;
    Timeout(const Timeout &other) = default;
    Timeout & operator=(const Timeout&) = default;
    Timeout(Timeout&&) = default;
    Timeout & operator=(Timeout&&) = default;

    Timeout(const std::chrono::milliseconds &timeout);
    Timeout(const long ms);

    long seconds() const;
    long milliseconds() const;
    long microseconds() const;

    explicit operator bool() const;
};


// IMPLEMENTATION
// --------------


/** \brief Throw overflow or underflow if outside of numeric limits.
 */
template <typename Duration>
void Timeout::checkLimits(const Duration &duration) const
{
    if (duration.count() > std::numeric_limits<long>::max()) {
        throw std::overflow_error("Timeout value overflow.\n");
    }
    if (duration.count() < std::numeric_limits<long>::min()) {
        throw std::overflow_error("Timeout value underflow.\n");
    }
}

}   /* lattice */
