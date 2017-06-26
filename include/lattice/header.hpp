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


/** \brief Case-insensitive comparison for ASCII.
 */
struct CaseInsensitiveCompare
{
    bool operator()(const std::string &left,
        const std::string &right) const noexcept;
};


/** \brief Custom headers for the request.
 */
class Header: public std::map<std::string, std::string, CaseInsensitiveCompare>
{
protected:
    typedef std::map<std::string, std::string, CaseInsensitiveCompare> Base;

public:
    using Base::Base;

    std::string string() const;
    bool accept() const;
    bool cookie() const;
    bool host() const;
    bool userAgent() const;
    bool authorization() const;
    bool wwwauthenticate() const;
    bool connection() const;
    bool closeConnection() const;
    bool contentType() const;

    friend std::ostream & operator<<(std::ostream &os,
        const Header &header);
    explicit operator bool() const;
};


}   /* lattice */
