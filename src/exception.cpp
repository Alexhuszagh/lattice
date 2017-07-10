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


void encryption_warning()
{
    std::lock_guard<std::mutex> lock(MUTEX);
    std::cerr << "Warning: HTTP requests do not support SSL/TLS."
              << "Do NOT send sensitive data without SSL/TLS."
              << std::endl;
}

}   /* lattice */
