//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief URL object.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Get service from  URL.
 */
std::string Url::service() const noexcept
{
    size_t index = find_first_of("://");
    if (index == std::string::npos) {
        return "http";
    }
    return substr(0, index);
}


/** \brief Get hostname from  URL.
 */
std::string Url::host() const noexcept
{
    size_t start = find_first_of("://");
    if (start == std::string::npos) {
        return substr(0, find_first_of('/'));
    }
    size_t end = find_first_of('/', start+4);
    return substr(start+3, end-start-3);
}


/** \brief Get hostname from  URL.
 */
std::string Url::path() const noexcept
{
    size_t start = find_first_of("://");
    if (start == std::string::npos) {
        return substr(find_first_of('/'));
    }
    return substr(find_first_of('/', start+4));
}


/** \brief Get directory component of URL.
 */
std::string Url::directory() const noexcept
{
    std::string data = path();

    return data.substr(0, data.find_last_of('/'));
}


/** \brief Get file component of URL.
 */
std::string Url::file() const noexcept
{
    std::string data = path();

    return data.substr(data.find_last_of('/'));
}

}   /* lattice */
