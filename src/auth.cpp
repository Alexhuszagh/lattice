//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Authentification for requests.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
Authentication::Authentication()
{}


/** \brief Initializer list constructor.
 */
Authentication::Authentication(std::string &&username,
        std::string &&password):
    username(FORWARD(username)),
    password(FORWARD(password))
{}


/** \brief Copy constructor.
 */
Authentication::Authentication(const Authentication &other):
    username(other.username),
    password(other.password)
{}


/** \brief Get authorization string.
 */
const std::string Authentication::string() const noexcept
{
    return username + ":" + password;
}


/** \brief Check if basic auth set.
 */
Authentication::operator bool() const
{
    return !(username.empty() || password.empty());
}


}   /* lattice */
