//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.

#include <lattice/util.hpp>

#include <iostream>


namespace lattice
{
// CONSTANTS
// ---------

std::mutex MUTEX;

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
