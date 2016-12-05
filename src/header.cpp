//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Custom HTTP header.
 */

#include "lattice.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Case-insensitive comparison for ASCII.
 */
bool CaseInsensitiveCompare::operator()(const std::string &left,
    const std::string &right) const noexcept
{
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end(), [](unsigned char l, unsigned char r) {
            return std::tolower(l) < std::tolower(r);
    });
}


/** \brief Print headers to string.
 */
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


/** \brief Check if the header specifies a host.
 */
bool Header::host() const
{
    return find("host") != end();
}


/** \brief Check if the header specifies a User-Agent.
 */
bool Header::userAgent() const
{
    return find("user-agent") != end();
}


/** \brief Check if the connection should be closed.
 */
bool Header::closeConnection() const
{
    auto it = find("connection");
    if (it != end()) {
        return it->second == "close";
    }
    return false;
}


/** \brief Print headers to stream.
 */
std::ostream & operator<<(std::ostream &os,
    const Header &header)
{
    os << header.string();

    return os;
}


}   /* lattice */
