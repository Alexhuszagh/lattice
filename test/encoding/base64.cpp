//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Base64 conversion unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(BaseN, Base64)
{
    std::string decoded("abcdef");
    std::string encoded("YWJjZGVm");

    EXPECT_EQ(encoded, lattice::BASE64_ENCODE(decoded));
    EXPECT_EQ(decoded, lattice::BASE64_DECODE(encoded));
}
