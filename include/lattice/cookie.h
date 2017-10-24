//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cookie support for persistent sessions.
 */

#pragma once

#include <lattice/config.h>
#include <map>
#include <string>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Cookies to store user data for persistent sessions.
 */
struct cookies_t: std::map<std::string, std::string>
{
    typedef std::map<std::string, std::string> base;
    using base::base;

    std::string encode() const;
    explicit operator bool() const;
};

LATTICE_END_NAMESPACE
