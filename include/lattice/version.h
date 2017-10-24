//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Lattice versioning.
 */

#pragma once

#include <lattice/config.h>
#include <string>

LATTICE_BEGIN_NAMESPACE

// CONSTANTS
// ---------

constexpr int MAJOR_VERSION = 0;
constexpr int MINOR_VERSION = 1;
constexpr int PATCH_VERSION = 0;
extern const std::string VERSION;

LATTICE_END_NAMESPACE