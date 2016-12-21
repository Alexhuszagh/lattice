//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Timeout unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Timeout, Timeout)
{
    lattice::Timeout timeout(1000);
    EXPECT_EQ(timeout.seconds(), 1);
    EXPECT_EQ(timeout.milliseconds(), 1000);
    EXPECT_EQ(timeout.microseconds(), 1000000);
    EXPECT_TRUE(bool(timeout));

    lattice::Timeout empty;
    EXPECT_FALSE(bool(empty));
}
