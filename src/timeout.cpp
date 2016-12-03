//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Timeout for requests.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
Timeout::Timeout()
{}


/** \brief Initializer constructor.
 */
Timeout::Timeout(const std::chrono::milliseconds &timeout):
    timeout(timeout)
{}


/** \brief Initializer constructor.
 */
Timeout::Timeout(const long ms):
    timeout(std::chrono::milliseconds(ms))
{}


/** \brief Copy constructor.
 */
Timeout::Timeout(const Timeout &other):
    timeout(other.timeout)
{}


long Timeout::seconds() const
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    checkLimits(seconds);

    return static_cast<long>(seconds.count());
}


/** \brief Cast timer to milliseconds.
 */
long Timeout::milliseconds() const
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    checkLimits(ms);

    return static_cast<long>(ms.count());
}


/** \brief Cast timer to microseconds.
 */
long Timeout::microseconds() const
{
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    checkLimits(us);

    return static_cast<long>(us.count());
}


/** \brief Check if timeout set.
 */
Timeout::operator bool() const
{
    return bool(timeout.count());;
}

}   /* lattice */
