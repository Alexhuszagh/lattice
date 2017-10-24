//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief timeout_t for requests.
 */

#include <lattice/timeout.h>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


timeout_t::timeout_t(const std::chrono::milliseconds &timeout):
    timeout(timeout)
{}


timeout_t::timeout_t(const long ms):
    timeout(std::chrono::milliseconds(ms))
{}


long timeout_t::seconds() const
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    check_limits(seconds);

    return static_cast<long>(seconds.count());
}


long timeout_t::milliseconds() const
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    check_limits(ms);

    return static_cast<long>(ms.count());
}


long timeout_t::microseconds() const
{
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    check_limits(us);

    return static_cast<long>(us.count());
}


timeout_t::operator bool() const
{
    return bool(timeout.count());;
}

LATTICE_END_NAMESPACE

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
