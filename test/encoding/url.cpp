//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief URL conversion unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Url, ToHex)
{
    EXPECT_EQ('f', lattice::toHex('/'));
}


TEST(Url, FromHex)
{
    EXPECT_EQ('\xF', lattice::fromHex('f'));
}


TEST(Url, UrlEncode)
{
    EXPECT_EQ("%3C%5ESpecial%3E", lattice::urlencode("<^Special>"));
}


TEST(Url, UrlDecode)
{
    EXPECT_EQ("<^Special>", lattice::urldecode("%3C%5ESpecial%3E"));
}
