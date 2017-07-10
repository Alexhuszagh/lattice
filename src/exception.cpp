//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#include <lattice/util/exception.hpp>
#include <lattice/util/mutex.hpp>

#include <iostream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
AddressError::AddressError(const std::string &host,
        const std::string &service)
{
    std::stringstream stream;
    stream << "Unable to get address from getaddrinfo("
           << host << ", " << service << ").\n";

    message = stream.str();
}


/** \brief Initializer list constructor.
 */
RequestError::RequestError(const int sent,
        const int total)
{
    std::stringstream stream;
    stream << "Unable to get make request: sent "
           << sent << " of " << total << " bytes.\n";

    message = stream.str();
}


NetworkSchemeError::NetworkSchemeError(const std::string &service)
{
    std::stringstream stream;
    stream << "Network scheme \"" << service << "\" is not supported.\n";

    message = stream.str();
}


/** \brief Warn user about user encryption methods without SSL/TLS support.
 */
void encryptionWarning()
{
    std::lock_guard<std::mutex> lock(MUTEX);
    std::cerr << "Warning: HTTP requests do not support SSL/TLS."
              << "Do NOT send sensitive data without SSL/TLS."
              << std::endl;
}


}   /* lattice */
