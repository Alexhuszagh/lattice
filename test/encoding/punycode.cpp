//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Punycode conversion unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Punycode, Utf)
{
    std::string utf8 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
    std::string punycode = "rksmrgs-5wao1o";

    EXPECT_EQ(punycode, lattice::UTF8_TO_PUNYCODE(utf8));
}
