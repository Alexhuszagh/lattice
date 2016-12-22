//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Follow redirects.
 */

#pragma once


namespace lattice
{
// OBJECTS
// -------


/** \brief Tracks redirects from the original URL.
 */
struct Redirects
{
    long count = 0;

    Redirects() = default;
    Redirects(const Redirects &other) = default;
    Redirects & operator=(const Redirects&) = default;
    Redirects(Redirects&&) = default;
    Redirects & operator=(Redirects&&) = default;

    Redirects(const long count);

    Redirects & operator++();
    Redirects operator++(int);
    Redirects & operator--();
    Redirects operator--(int);
    explicit operator bool() const;
};

}   /* lattice */
