//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief System random unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Random, SysRandom)
{
    // cryptographic random, should be completely unique
    auto reference = lattice::sysrandom(8);
    for (int i = 0; i < 200; ++i) {
        EXPECT_NE(lattice::sysrandom(8), reference);
    }
}


TEST(Random, PseudoRandom)
{
    // all are seeded the same, should produce same output
    auto reference = lattice::pseudorandom(8);
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(lattice::pseudorandom(8), reference);
    }
}
