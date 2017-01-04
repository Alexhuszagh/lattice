//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Timeout for requests.
 */

#include "lattice/timeout.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif


namespace lattice
{
// OBJECTS
// -------


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

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
