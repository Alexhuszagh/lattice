//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
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


/**
 *  \brief Tracks redirects from the original URL.
 */
struct redirects_t
{
    long count = 0;

    redirects_t() = default;
    redirects_t(const redirects_t &other) = default;
    redirects_t & operator=(const redirects_t&) = default;
    redirects_t(redirects_t&&) = default;
    redirects_t & operator=(redirects_t&&) = default;

    redirects_t(const long count);

    redirects_t & operator++();
    redirects_t operator++(int);
    redirects_t & operator--();
    redirects_t operator--(int);
    explicit operator bool() const;
};

}   /* lattice */
