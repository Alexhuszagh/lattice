//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Timeout for requests.
 */

#pragma once

#include <lattice/config.h>
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

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Timeout for a request object.
 */
class timeout_t
{
public:
    timeout_t() = default;
    timeout_t(const timeout_t &other) = default;
    timeout_t & operator=(const timeout_t&) = default;
    timeout_t(timeout_t&&) = default;
    timeout_t & operator=(timeout_t&&) = default;

    timeout_t(const std::chrono::milliseconds &timeout);
    timeout_t(const long ms);

    long seconds() const;
    long milliseconds() const;
    long microseconds() const;

    explicit operator bool() const;

protected:
    std::chrono::milliseconds timeout = std::chrono::milliseconds(0);

    template <typename Duration>
    void check_limits(const Duration &duration) const;
};


// IMPLEMENTATION
// --------------


/**
 *  \brief Throw overflow or underflow if outside of numeric limits.
 */
template <typename Duration>
void timeout_t::check_limits(const Duration &duration) const
{
    if (duration.count() > std::numeric_limits<long>::max()) {
        throw std::overflow_error("Timeout value overflow.\n");
    }
    if (duration.count() < std::numeric_limits<long>::min()) {
        throw std::overflow_error("Timeout value underflow.\n");
    }
}

LATTICE_END_NAMESPACE
