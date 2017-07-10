//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Custom HTTP header.
 */

#include <lattice/header.hpp>
#include <pycpp/casemap.h>

#include <algorithm>
#include <cctype>
#include <sstream>


namespace lattice
{
// OBJECTS
// -------


bool lowercase_less::operator()(const std::string &lhs, const std::string &rhs) const noexcept
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](unsigned char l, unsigned char r) {
            return ascii_tolower(l) < ascii_tolower(r);
    });
}


std::string Header::string() const
{
    std::stringstream stream;
    for (const auto &pair: *this) {
        if (pair.second.empty()) {
            stream << pair.first << ";\r\n";
        } else {
            stream << pair.first << ": " << pair.second << "\r\n";
        }
    }

    return stream.str();
}


bool Header::accept() const
{
    return find("accept") != end();
}


bool Header::cookie() const
{
    return find("cookie") != end();
}


bool Header::host() const
{
    return find("host") != end();
}


bool Header::authorization() const
{
    return find("authorization") != end();
}


bool Header::wwwauthenticate() const
{
    return find("www-authenticate") != end();
}


bool Header::user_agent() const
{
    return find("user-agent") != end();
}


bool Header::close_connection() const
{
    auto it = find("connection");
    if (it != end()) {
        return it->second == "close";
    }
    return false;
}


bool Header::connection() const
{
    return find("connection") != end();
}


bool Header::content_type() const
{
    return find("content-Type") != end();
}


std::ostream & operator<<(std::ostream &os,
    const Header &header)
{
    os << header.string();

    return os;
}


Header::operator bool() const
{
    return !empty();
}

}   /* lattice */
