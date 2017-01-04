//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cookie support for persistent sessions.
 */

#include "lattice/cookie.hpp"
#include "lattice/encoding/url.hpp"

#include <sstream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Special case of type 1 cookies, with leading and trailing quotes.
 */
bool versionOneCookie(const std::string &cookie)
{
    if (!cookie.empty()) {
        return (cookie.front() == '"' && cookie.back() == '"');
    }
    return false;
}


/** \brief Encode cookies for request.
 */
std::string Cookies::encode() const
{
    std::stringstream stream;
    for (const auto &item: *this) {
        stream << urlencode(item.first) << "=";
        if (versionOneCookie(item.second)) {
            stream << item.second;
        } else {
            stream << urlencode(item.second);
        }
        stream << "; ";
    }

    return stream.str();
}


/** \brief Check if cookies are set.
 */
Cookies::operator bool() const
{
    return !empty();
}


}   /* lattice */
