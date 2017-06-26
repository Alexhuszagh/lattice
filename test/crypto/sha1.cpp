//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief SHA1 algorithm unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Sha1, Sha1)
{
    std::string bytes = "012345";
    std::string digest = {
        -3, -8, -68, 88, 20, 83, 111, 102, 1, 40, -124,
        -31, 70, -88, -120, 122, 68, 112, -102, 86
    };

    EXPECT_EQ(digest, lattice::SHA1(bytes));
}


TEST(Sha1, Sha1Hex)
{
    std::string bytes = "012345";
    std::string digest = "FDF8BC5814536F66012884E146A8887A44709A56";
    EXPECT_EQ(digest, lattice::SHA1_HEX(bytes));
}
