//  :copyright: (c) Steve Reid <steve@edmweb.com>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: Public Domain.
/*
 *  \addtogroup Lattice
 *  \brief SHA1 hash function.
 */

#pragma once

#include <string>


namespace lattice
{
// CONSTANTS
// ---------

constexpr size_t SHA1_HASH_SIZE = 20;

// OBJECTS
// -------

/** \brief SHA1 context.
 */
struct SHA1_CTX
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
};

// FUNCTIONS
// ---------

/** \brief Initialize SHA1 context.
 */
void SHA1_Init(SHA1_CTX *ctx);

/** \brief Update hash with data.
 */
void SHA1_Update(SHA1_CTX *ctx,
    const unsigned char* data,
    uint32_t len);

/** \brief Add padding and return the message digest.
 */
void SHA1_Final(unsigned char digest[20],
    SHA1_CTX *ctx);

/** \brief STL wrapper that calculates the SHA1 hash.
 */
std::string sha1Hash(const std::string &string);

/** \brief STL wrapper that creates an SHA1 hex digest.
 */
std::string sha1HexDigest(const std::string &string);

// API
// ---

static const auto &SHA1 = sha1Hash;
static const auto &SHA1_HEX = sha1HexDigest;

}   /* lattice */
