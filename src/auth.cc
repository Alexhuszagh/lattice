//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Authentification for requests.
 */

#include <lattice/auth.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


authentication_t::authentication_t(std::string &&username, std::string &&password):
    username(std::forward<std::string>(username)),
    password(std::forward<std::string>(password))
{}


authentication_t::authentication_t(const std::string &username, const std::string &password):
    username(username),
    password(password)
{}


const std::string authentication_t::string() const noexcept
{
    return username + ":" + password;
}


authentication_t::operator bool() const
{
    return !(username.empty() || password.empty());
}

LATTICE_END_NAMESPACE
