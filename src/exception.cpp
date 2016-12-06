//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#include "lattice.hpp"

#include <iostream>


namespace lattice
{
// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
AddressError::AddressError(const std::string &host,
        const std::string &service):
    host(host),
    service(service)
{}


/** \brief Initializer list constructor.
 */
RequestError::RequestError(const int sent,
        const int total):
    sent(sent),
    total(total)
{}


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
