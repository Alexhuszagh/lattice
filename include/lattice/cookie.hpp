//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cookie support for persistent sessions.
 */

#pragma once

#include <map>
#include <sstream>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Cookies to store user data for persistent sessions.
 */
class Cookies: public std::map<std::string, std::string>
{
protected:
    typedef std::map<std::string, std::string> Base;

public:
    using Base::Base;

    std::string encode() const;
};


}   /* lattice */
