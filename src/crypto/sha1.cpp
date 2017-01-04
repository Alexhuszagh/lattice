//  :copyright: (c) Steve Reid <steve@edmweb.com>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: Public Domain.
/*
 *  \addtogroup Lattice
 *  \brief SHA1 hash function.
 */

#include "lattice/crypto/sha1.hpp"
#include "lattice/encoding/hex.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4267)
#endif


namespace lattice
{
// UNIONS
// ------

typedef union {
    unsigned char c[64];
    uint32_t l[16];
} CHAR64LONG16;

// FUNCTIONS
// ---------

#define ROL(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#if BYTE_ORDER == LITTLE_ENDIAN
#   define BLK0(i) (block->l[i] = (ROL(block->l[i],24)&0xFF00FF00) \
    |(ROL(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#   define BLK0(i) block->l[i]
#else
#   error "Endianness not defined!"
#endif

#define BLK(i) (block->l[i&15] = ROL(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/** \brief (R0+R1), R2, R3, R4 are the different operations used in SHA1.
 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+BLK0(i)+0x5A827999+ROL(v,5);w=ROL(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+BLK(i)+0x5A827999+ROL(v,5);w=ROL(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+BLK(i)+0x6ED9EBA1+ROL(v,5);w=ROL(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+BLK(i)+0x8F1BBCDC+ROL(v,5);w=ROL(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+BLK(i)+0xCA62C1D6+ROL(v,5);w=ROL(w,30);



/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1_Transform(uint32_t state[5],
    const unsigned char buffer[64])
{
    uint32_t a, b, c, d, e;
    CHAR64LONG16 block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, 64);

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // ROUND 1
    R0(a,b,c,d,e, 0);
    R0(e,a,b,c,d, 1);
    R0(d,e,a,b,c, 2);
    R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4);
    R0(a,b,c,d,e, 5);
    R0(e,a,b,c,d, 6);
    R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8);
    R0(b,c,d,e,a, 9);
    R0(a,b,c,d,e,10);
    R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12);
    R0(c,d,e,a,b,13);
    R0(b,c,d,e,a,14);
    R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16);
    R1(d,e,a,b,c,17);
    R1(c,d,e,a,b,18);
    R1(b,c,d,e,a,19);

    // ROUND 2
    R2(a,b,c,d,e,20);
    R2(e,a,b,c,d,21);
    R2(d,e,a,b,c,22);
    R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24);
    R2(a,b,c,d,e,25);
    R2(e,a,b,c,d,26);
    R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28);
    R2(b,c,d,e,a,29);
    R2(a,b,c,d,e,30);
    R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32);
    R2(c,d,e,a,b,33);
    R2(b,c,d,e,a,34);
    R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36);
    R2(d,e,a,b,c,37);
    R2(c,d,e,a,b,38);
    R2(b,c,d,e,a,39);

    // ROUND 3
    R3(a,b,c,d,e,40);
    R3(e,a,b,c,d,41);
    R3(d,e,a,b,c,42);
    R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44);
    R3(a,b,c,d,e,45);
    R3(e,a,b,c,d,46);
    R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48);
    R3(b,c,d,e,a,49);
    R3(a,b,c,d,e,50);
    R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52);
    R3(c,d,e,a,b,53);
    R3(b,c,d,e,a,54);
    R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56);
    R3(d,e,a,b,c,57);
    R3(c,d,e,a,b,58);
    R3(b,c,d,e,a,59);

    // ROUND 4
    R4(a,b,c,d,e,60);
    R4(e,a,b,c,d,61);
    R4(d,e,a,b,c,62);
    R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64);
    R4(a,b,c,d,e,65);
    R4(e,a,b,c,d,66);
    R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68);
    R4(b,c,d,e,a,69);
    R4(a,b,c,d,e,70);
    R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72);
    R4(c,d,e,a,b,73);
    R4(b,c,d,e,a,74);
    R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76);
    R4(d,e,a,b,c,77);
    R4(c,d,e,a,b,78);
    R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    /* Wipe variables */
    a = b = c = d = e = 0;
    memset(block, '\0', sizeof(block));
}


/** \brief Initialize SHA1 context.
 */
void SHA1_Init(SHA1_CTX *ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->count[0] = ctx->count[1] = 0;
}


/** \brief Update hash with data.
 */
void SHA1_Update(SHA1_CTX *ctx,
    const unsigned char* data,
    uint32_t len)
{
    uint32_t i, j;

    j = ctx->count[0];
    if ((ctx->count[0] += len << 3) < j)
        ctx->count[1]++;
    ctx->count[1] += (len>>29);
    j = (j >> 3) & 63;
    if ((j + len) > 63) {
        memcpy(&ctx->buffer[j], data, (i = 64-j));
        SHA1_Transform(ctx->state, ctx->buffer);
        for ( ; i + 63 < len; i += 64) {
            SHA1_Transform(ctx->state, &data[i]);
        }
        j = 0;
    }
    else i = 0;
    memcpy(&ctx->buffer[j], &data[i], len - i);
}


/** \brief Add padding and return the message digest.
 */
void SHA1_Final(unsigned char digest[20],
    SHA1_CTX *ctx)
{
    unsigned i;
    unsigned char finalcount[8];
    unsigned char c;

    for (i = 0; i < 8; i++) {
        finalcount[i] = (unsigned char)((ctx->count[(i >= 4 ? 0 : 1)]
         >> ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
    }

    c = 0200;
    SHA1_Update(ctx, &c, 1);
    while ((ctx->count[0] & 504) != 448) {
    c = 0000;
        SHA1_Update(ctx, &c, 1);
    }
    SHA1_Update(ctx, finalcount, 8);  /* Should cause a SHA1_Transform() */
    for (i = 0; i < 20; i++) {
        digest[i] = (unsigned char)
         ((ctx->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }

    /* Wipe variables */
    memset(ctx, '\0', sizeof(*ctx));
    memset(&finalcount, '\0', sizeof(finalcount));
}


/** \brief STL wrapper that calculates the SHA1 hash.
 */
std::string sha1Hash(const std::string &string)
{
    SHA1_CTX context;
    auto *src = reinterpret_cast<const unsigned char*>(string.data());
    uint32_t length = string.size();

    SHA1_Init(&context);
    while (length > 0) {
        const uint32_t shift = length > 512 ? 512 : length;
        SHA1_Update(&context, src, shift);
        length -= shift;
        src += shift;
    }

    char *digest = new char[SHA1_HASH_SIZE];
    SHA1_Final(reinterpret_cast<unsigned char*>(digest), &context);

    std::string output(digest, SHA1_HASH_SIZE);
    delete[] digest;

    return output;
}


/** \brief STL wrapper that creates an SHA1 hex digest.
 */
std::string sha1HexDigest(const std::string &string)
{
    return HEX<uint8_t>(sha1Hash(string));
}

// CLEANUP
// -------

#undef ROL
#undef BLK0
#undef BLK
#undef R0
#undef R1
#undef R2
#undef R3
#undef R4


}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
