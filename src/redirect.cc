//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Follow redirects.
 */

#include <lattice/redirect.h>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


redirects_t::redirects_t(const long count):
    count(count)
{}


redirects_t & redirects_t::operator++()
{
    ++count;

    return *this;
}


redirects_t redirects_t::operator++(int)
{
    redirects_t copy(*this);
    operator++();

    return copy;
}


redirects_t & redirects_t::operator--()
{
    --count;

    return *this;
}


redirects_t redirects_t::operator--(int)
{
    redirects_t copy(*this);
    operator--();

    return copy;
}


redirects_t::operator bool() const
{
    return bool(count);
}

LATTICE_END_NAMESPACE

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
