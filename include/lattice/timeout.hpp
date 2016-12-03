//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
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
    Timeout();
    Timeout(const std::chrono::milliseconds &timeout);
    Timeout(const long ms);
    Timeout(const Timeout &other);

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
