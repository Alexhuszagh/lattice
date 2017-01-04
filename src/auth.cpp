//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Authentification for requests.
 */

#include "lattice/auth.hpp"
#include "lattice/util/define.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Initializer list nove constructor.
 */
Authentication::Authentication(std::string &&username,
        std::string &&password):
    username(LATTICE_FWD(username)),
    password(LATTICE_FWD(password))
{}


/** \brief Initializer list copy constructor.
 */
Authentication::Authentication(const std::string &username,
        const std::string &password):
    username(username),
    password(password)
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
