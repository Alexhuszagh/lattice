//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Authentification for requests.
 */

#pragma once

#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Authorization string for requests.
 */
struct Authentication
{
    std::string username;
    std::string password;

    Authentication() = default;
    Authentication(const Authentication&) = default;
    Authentication & operator=(const Authentication&) = default;
    Authentication(Authentication&&) = default;
    Authentication & operator=(Authentication&&) = default;

    Authentication(std::string &&username,
        std::string &&password);
    Authentication(const std::string &username,
        const std::string &password);

    const std::string string() const noexcept;
    explicit operator bool() const;
};


}   /* lattice */
