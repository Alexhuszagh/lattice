//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Export strings to hex.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Hex, Int8T)
{
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    std::string hex = "0001020304050607";
    EXPECT_EQ(hex, lattice::HEX<uint8_t>(bytes));
}


TEST(Hex, Int32T)
{
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    std::string hex = "0302010007060504";
    EXPECT_EQ(hex, lattice::HEX<uint32_t>(bytes));
}
