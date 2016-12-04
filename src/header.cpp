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
    for (const auto &pair: header) {
        if (pair.second.empty()) {
            os << pair.first << ";\r\n";
        } else {
            os << pair.first << ": " << pair.second << "\r\n";
        }
    }

    return os;
}


}   /* lattice */
