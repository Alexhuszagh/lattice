//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Base64 encoding and decoding lookup tables.
 */

#include "lattice.hpp"

#include <cmath>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4244)
#endif


namespace lattice
{
namespace base64
{
// CONSTANTS
// ---------

const size_t INPUT_INTERVAL = 3;
const size_t OUTPUT_INTERVAL = 4;
const char ENCODING[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char DECODING[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

// FUNCTIONS
// ---------


/** Calculate the exact number of non-padded output bytes from
 *  an input byte-count.
 */
size_t encodedByteCount(const size_t byteCount)
{
    return std::ceil(OUTPUT_INTERVAL * static_cast<double>(byteCount) / INPUT_INTERVAL);
}


/** Calculate upper bound onencoded message length, for buffer allocation
 *  to avoid reallocating.
 *
 *  The message size for each encoded message must be a multiple of 24,
 *  and each bit is transformed into 6-bit character groups to be
 *  transformed into the base64 dictionary (2^6 == 64).
 */
size_t encodedSize(const size_t messageLength)
{
    return ((4 * messageLength / 3) + 3) & ~3;
}


/** Calculate upper bound onencoded message length, for buffer allocation
 *  to avoid reallocating.
 */
size_t decodedSize(const size_t messageLength)
{
    return std::ceil(INPUT_INTERVAL * static_cast<double>(messageLength)) / OUTPUT_INTERVAL;
}

}   /* base64 */

// API
// ---


/** \brief String wrapper for base64::encode.
 */
std::string b64Encode(const std::string &other)
{
    std::string output;
    base64::encode(output, other.begin(), other.end());

    return output;
}


/** \brief String wrapper for base64::decode.
 */
std::string b64Decode(const std::string &other)
{
    std::string output;
    base64::decode(output, other.begin(), other.end());

    return output;
}


}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
