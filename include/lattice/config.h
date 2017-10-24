//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Configurations for Lattice.
 */

#pragma once

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4003)
#endif

// MACROS
// ------

#ifndef LATTICE_NAMESPACE
#   define LATTICE_NAMESPACE
#endif

#define LATTICE_DO_EXPAND(VAL)  VAL ## 1
#define LATTICE_EXPAND(VAL)     LATTICE_DO_EXPAND(VAL)


#if LATTICE_EXPAND(LATTICE_NAMESPACE) == 1      // NO NAMESPACE
#   define LATTICE_BEGIN_NAMESPACE
#   define LATTICE_END_NAMESPACE
#   define LATTICE_USING_NAMESPACE
#   undef LATTICE_HAVE_NAMESPACE
#else                                       // HAVE NAMESPACE
#   define LATTICE_BEGIN_NAMESPACE namespace LATTICE_NAMESPACE {
#   define LATTICE_END_NAMESPACE }
#   define LATTICE_USING_NAMESPACE using namespace LATTICE_NAMESPACE;
#   define LATTICE_HAVE_NAMESPACE
#endif

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
