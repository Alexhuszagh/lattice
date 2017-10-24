//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Timeout unittests.
 */

#include <lattice.h>
#include <gtest/gtest.h>

LATTICE_USING_NAMESPACE


// TESTS
// -----


TEST(timeout_t, timeout_t)
{
    timeout_t timeout(1000);
    EXPECT_EQ(timeout.seconds(), 1);
    EXPECT_EQ(timeout.milliseconds(), 1000);
    EXPECT_EQ(timeout.microseconds(), 1000000);
    EXPECT_TRUE(bool(timeout));

    timeout_t empty;
    EXPECT_FALSE(bool(empty));
}
