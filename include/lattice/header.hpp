//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Custom HTTP header.
 */

#pragma once

#include <map>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/**
 *  \brief Case-insensitive std::less for ASCII.
 */
struct lowercase_less
{
    bool operator()(const std::string&, const std::string&) const noexcept;
};


/**
 *  \brief Custom headers for the request.
 */
struct Header: std::map<std::string, std::string, lowercase_less>
{
    typedef std::map<std::string, std::string, lowercase_less> base;
    using base::base;

    std::string string() const;
    bool accept() const;
    bool cookie() const;
    bool host() const;
    bool user_agent() const;
    bool authorization() const;
    bool wwwauthenticate() const;
    bool connection() const;
    bool close_connection() const;
    bool content_type() const;

    friend std::ostream & operator<<(std::ostream &os, const Header &header);
    explicit operator bool() const;
};

}   /* lattice */
