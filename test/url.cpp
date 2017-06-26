//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup LatticeTests
 *  \brief Url unittests.
 */

#include "lattice.hpp"

#include <gtest/gtest.h>


// TESTS
// -----


TEST(Url, Constructors)
{
    lattice::Url url;
    url = lattice::Url("http://example.com");
    url = lattice::Url("http://example.com", 18);
    url = lattice::Url(std::string("http://example.com"));
    url = lattice::Url {104, 116, 116, 112, 58, 47, 47, 101, 120, 97, 109, 112, 108, 101, 46, 99, 111, 109};
}


TEST(Url, Getters)
{
    lattice::Url url("http://example.com/path/to/file");
    EXPECT_EQ("http", url.service());
    EXPECT_EQ("example.com", url.host());
    EXPECT_EQ("/path/to/file", url.path());
    EXPECT_EQ("path/to", url.directory());
    EXPECT_EQ("file", url.file());
}


TEST(Url, Setters)
{
    lattice::Url url("http://example.com/path/to/file");

    // SERVICE
    url.setService("https");
    EXPECT_EQ("https://example.com/path/to/file", url);

    // HOST
    url.setHost("example.net");
    EXPECT_EQ("https://example.net/path/to/file", url);

    // PATH
    url.setPath("/new/path/to/file");
    EXPECT_EQ("https://example.net/new/path/to/file", url);

    // DIRECTORY
    url.setDirectory("new/directory");
    EXPECT_EQ("https://example.net/new/directory/file", url);

    // FILE
    url.setFile("document");
    EXPECT_EQ("https://example.net/new/directory/document", url);
}


TEST(Url, Properties)
{
    lattice::Url url("http://example.com/path/to/file");
    EXPECT_FALSE(url.relative());
    EXPECT_TRUE(url.absolute());

    url = lattice::Url("/path/to/file");
    EXPECT_TRUE(url.relative());
    EXPECT_FALSE(url.absolute());
}


TEST(Url, Unicode)
{
    // "http://räksmörgås.josefsson.org/"
    lattice::Url url = {
        104, 116, 116, 112, 58, 47, 47, 114, -61, -92,
        107, 115, 109, -61, -74, 114, 103, -61, -91,
        115, 46, 106, 111, 115, 101, 102, 115, 115, 111,
        110, 46, 111, 114, 103, 47
    };

    EXPECT_EQ("http", url.service());
    EXPECT_EQ("xn--rksmrgs-5wao1o.josefsson.org", url.host());
    EXPECT_EQ("/", url.path());
    EXPECT_EQ("", url.directory());
    EXPECT_EQ("", url.file());
}
