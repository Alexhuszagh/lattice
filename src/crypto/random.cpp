//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Cryptographic random number generators.
 */

#include "lattice.hpp"

#include <cstring>
#include <ctime>
#include <fstream>
#include <random>

#if defined(_WIN32)
#   include "wincrypt.h"
#endif


namespace lattice
{
// FUNCTIONS
// ---------


/** The following section uses cryptographic random number generators.
 *  Although std::random_device was designed in the C++ standard library
 *  for this reason, if hardware sources are insufficient, it allows
 *  a possibly deterministic fallback, which is true for compilers
 *  such a MinGW. For these reasons, native APIs and /dev/urandom
 *  are used for truly random bytes.
 */

#ifdef _WIN32

     /** \brief Wrapper that acquires the context for the crypt library.
      *
      * If the keyset has not been set, it also initializes that.
      */
    bool acquireContext(HCRYPTPROV *ctx)
    {
        if (!CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, 0)) {
            return CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, CRYPT_NEWKEYSET);
        }
        return true;
    }

    /** \brief Generate cryptograhically random bytes on Windows.
     */
    std::string sysrandom(const size_t size)
    {
        HCRYPTPROV ctx;
        if (!acquireContext(&ctx)) {
            throw WindowsCrypt32Error();
        }

        BYTE *buffer = new BYTE[size];
        if(!CryptGenRandom(ctx, size, buffer)) {
            throw WindowsCrypt32Error();
        }

        if (!CryptReleaseContext(ctx, 0)) {
            throw WindowsCrypt32Error();
        }

        std::string output(reinterpret_cast<char*>(buffer), size);
        delete[] buffer;

        return output;
    }

#else                   // POSIX

    /** \brief Generate cryptograhically random bytes on POSIX-systems.
     *
     *  Open and read from `/dev/urandom`, **not** `/dev/random`.
     *
     *  [reference] http://www.2uo.de/myths-about-urandom/
     */
    std::string sysrandom(const size_t size)
    {
        std::ifstream stream("/dev/urandom", std::ios_base::binary | std::ios_base::in);
        char *buffer = new char[size];
        stream.read(buffer, size);

        std::string output(buffer, size);
        delete[] buffer;

        return output;
    }

#endif                  // WIN32


/** \brief Get psuedo-random bytes for general purposes.
 *
 *  Uses the 32-bitt Mersenne Twister algorithm.
 *
 *  \warning This is **not** suitable for cryptographic purposes.
 */
std::string pseudorandom(const size_t size)
{
    std::mt19937 generator;
    generator.seed(time(nullptr));

    char *buffer = new char[size];
    char *ptr = buffer;
    size_t counter = size / sizeof(uint32_t);
    while (counter--) {
        const uint32_t value = generator();
        memmove(ptr, &value, sizeof(value));
        ptr += sizeof(value);
    }

    std::string output(buffer, size);
    delete[] buffer;

    return output;
}



}   /* lattice */
