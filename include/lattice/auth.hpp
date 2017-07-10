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
struct authentication_t
{
    std::string username;
    std::string password;

    authentication_t() = default;
    authentication_t(const authentication_t&) = default;
    authentication_t & operator=(const authentication_t&) = default;
    authentication_t(authentication_t&&) = default;
    authentication_t & operator=(authentication_t&&) = default;

    authentication_t(std::string &&username, std::string &&password);
    authentication_t(const std::string &username, const std::string &password);

    const std::string string() const noexcept;
    explicit operator bool() const;
};

}   /* lattice */
