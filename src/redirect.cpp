//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Follow redirects.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
Redirects::Redirects()
{}


/** \brief Initializer list constructor.
 */
Redirects::Redirects(const long count):
    count(count)
{}


/** \brief Copy constructor.
 */
Redirects::Redirects(const Redirects &other):
    count(other.count)
{}


/** Pre-increment operator.
 */
Redirects & Redirects::operator++()
{
    ++count;

    return *this;
}


/** Post-increment operator.
 */
Redirects Redirects::operator++(int)
{
    Redirects copy(*this);
    operator++();

    return copy;
}


/** Pre-decrement operator.
 */
Redirects & Redirects::operator--()
{
    --count;

    return *this;
}


/** Post-decrement operator.
 */
Redirects Redirects::operator--(int)
{
    Redirects copy(*this);
    operator--();

    return copy;
}


/** \brief Explicit conversion to bool.
 */
Redirects::operator bool() const
{
    return bool(count);
}



}   /* lattice */
