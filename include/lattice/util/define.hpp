//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Macro definitions.
 */

#pragma once

#include <utility>


namespace lattice
{
// MACROS
// ------

#define LATTICE_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

}   /* lattice */
