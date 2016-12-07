//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
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

    Authentication();
    Authentication(std::string &&username,
        std::string &&password);
    Authentication(const Authentication &other);

    const std::string string() const noexcept;
    explicit operator bool() const;
};


}   /* lattice */
