//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Follow redirects.
 */

#include <lattice/redirect.hpp>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
redirects_t::redirects_t(const long count):
    count(count)
{}


/** Pre-increment operator.
 */
redirects_t & redirects_t::operator++()
{
    ++count;

    return *this;
}


/** Post-increment operator.
 */
redirects_t redirects_t::operator++(int)
{
    redirects_t copy(*this);
    operator++();

    return copy;
}


/** Pre-decrement operator.
 */
redirects_t & redirects_t::operator--()
{
    --count;

    return *this;
}


/** Post-decrement operator.
 */
redirects_t redirects_t::operator--(int)
{
    redirects_t copy(*this);
    operator--();

    return copy;
}


/** \brief Explicit conversion to bool.
 */
redirects_t::operator bool() const
{
    return bool(count);
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
