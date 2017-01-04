//  :copyright: (c) 2001 Alexander Peslyak <solar@openwall.com>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: Public Domain.
/*
 *  \addtogroup Lattice
 *  \brief MD5 hash function.
 *
 *  [reference] http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 *  This software was written by Alexander Peslyak in 2001.  No copyright is
 *  claimed, and the software is hereby placed in the public domain.
 *  In case this attempt to disclaim copyright and place the software in the
 *  public domain is deemed null and void, then the software is
 *  Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 *  general public under the following terms:
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted.
 *
 *  There's ABSOLUTELY NO WARRANTY, express or implied.
 */

#include <cstdint>
#include <string>


namespace lattice
{
// CONSTANTS
// ---------

constexpr size_t MD5_HASH_SIZE = 16;

// OBJECTS
// -------

/** \brief 32-bit MD5 integer.
 */
typedef uint32_t MD5_u32plus;

/** \brief MD5 context.
 */
struct MD5_CTX
{
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    unsigned char buffer[64];
    uint32_t block[MD5_HASH_SIZE];
};

// FUNCTIONS
// ---------

/** \brief Initialize MD5 context.
 */
void MD5_Init(MD5_CTX *ctx);

/** \brief Update hash with data.
 */
void MD5_Update(MD5_CTX *ctx,
    const void *data,
    unsigned long size);

/** \brief Add padding and return the message digest.
 */
void MD5_Final(unsigned char *result,
    MD5_CTX *ctx);

/** \brief STL wrapper that calculates the MD5 hash.
 */
std::string md5Hash(const std::string &string);

/** \brief STL wrapper that creates an MD5 hex digest.
 */
std::string md5HexDigest(const std::string &string);

// API
// ---

static const auto &MD5 = md5Hash;
static const auto &MD5_HEX = md5HexDigest;

}   /* lattice */
