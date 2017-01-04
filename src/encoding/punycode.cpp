//  :copyright: (c) 2011 by Ben Noordhuis <info@bnoordhuis.nl>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Punycode encoding/decoding.
 */

#include "lattice/encoding/punycode.hpp"
#include "lattice/encoding/unicode.hpp"

#include <algorithm>
#include <cstdint>
#include <stdexcept>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4244 4267)
#endif


namespace lattice
{
namespace punycode
{
// FUNCTIONS
// ---------


/** \brief Bias adaption function.
 */
uint32_t adaptBias(uint32_t delta,
    const uint32_t points,
    const bool first)
{
    // variables
    constexpr uint32_t base = 36;
    constexpr uint32_t damp = 700;
    constexpr uint32_t skew = 38;
    constexpr uint32_t tmin = 1;
    constexpr uint32_t tmax = 26;

    // initialize
    delta /= first ? damp : 2;
    delta += delta / points;

    /* while delta > 455: delta /= 35 */
    uint32_t k;
    for (k = 0; delta > ((base - tmin) * tmax) / 2; k += base) {
        delta /= (base - tmin);
    }

    return k + (((base - tmin + 1) * delta) / (delta + skew));
}


/** \brief Get threshold, t(j) from RFC3492.
 */
const uint32_t threshold(const uint32_t k,
    const uint32_t bias)
{
    // limits
    constexpr uint32_t tmin = 1;
    constexpr uint32_t tmax = 26;

    if (k <= bias) {
        return tmin;
    } else if (k >= bias + tmax) {
        return tmax;
    } else {
        return k - bias;
    }
}


/** \brief Encode single byte to Punycode.
 */
static uint8_t encodeByte(uint8_t c)
{
    // limits
    constexpr uint32_t base = 36;
    constexpr uint32_t tmin = 1;
    constexpr uint32_t range = base - tmin;

    if (c < 0 || c > range) {
        throw std::out_of_range("Character to encode is out of range\n");
    }
    if (c > 25) {
        return c + 22;                      // '0'..'9'
    }
    return c + 'a';                         // 'a'..'z'
}


/** \brief Decode single byte from Punycode.
 */
uint32_t decodeByte(uint8_t c)
{
    if ((c >= 48) && (c <= 57)) {
        return 22 + (c - '0');              // '0'..'9'
    } else if ((c >= 97) && (c <= 122)) {
        return c - 'a';                     // 'a'..'z'
    } else if ((c >= 65) && (c <= 90)) {
        return c - 'A';                     // 'A'..'Z'
    }
    throw std::out_of_range("Character to decode is out of range\n");
}


/** \brief Encode single UTF-32 code point to Punycode.
 */
template <typename Iter8>
void encodeCharacter(const uint32_t bias,
    const uint32_t delta,
    Iter8 &begin,
    Iter8 end)
{
    // limits
    constexpr uint32_t base = 36;

    // encode
    uint32_t t;
    uint32_t k = base;
    uint32_t q = delta;

    while (begin < end) {
        t = threshold(k, bias);

        if (q < t) {
            break;
        }

        *begin++ = encodeByte(t + (q - t) % (base - t));
        q = (q - t) / (base - t);
        k += base;
    }

    if (begin < end) {
        *begin++ = encodeByte(q);
    }
}


/** \brief Encode UTF-32 array to Punycode.
 */
template <typename Iter32, typename Iter8>
size_t encode(Iter32 srcBegin,
    Iter32 srcEnd,
    Iter8 dstBegin,
    Iter8 dstEnd)
{
    // limits
    constexpr uint32_t asciiEnd = 128;

    // encode basic
    auto src = srcBegin;
    auto dst = dstBegin;
    while (src < srcEnd) {
        const uint32_t c = *src++;
        if (c < asciiEnd) {
            *dst++ = c;
        }
    }

    // get number processed
    const uint32_t basic = dst - dstBegin;
    uint32_t h = basic;
    if (basic) {
        *dst++ = '-';
    }

    // process non-basic characters
    uint32_t n = asciiEnd;
    uint32_t bias = 72;
    uint32_t delta = 0;
    const size_t srclen = srcEnd - srcBegin;
    size_t m;

    for (; h < srclen && dst < dstEnd; n++, delta++) {
        // find smallest non-basic code point
        auto src = srcBegin;
        for (m = SIZE_MAX; src < srcEnd; ++src) {
            const uint32_t c = *src;
            if (c >= n && c < m) {
                m = c;
            }
        }

        // check overflow
        if ((m - n) > (SIZE_MAX - delta) / (h + 1)) {
            throw std::overflow_error("Overflow detected in encoding.\n");
        }

        delta += (m - n) * (h + 1);
        n = m;

        // add values
        for (auto src = srcBegin; src < srcEnd; ++src) {
            const uint32_t c = *src;
            if (c < n) {
                if (++delta == 0) {
                    throw std::overflow_error("Punycode encoding overflow.\n");
                }
            } else if (c == n) {
                encodeCharacter(bias, delta, dst, dstEnd);
                bias = adaptBias(delta, h + 1, h == basic);
                delta = 0;
                ++h;
            }
        }
    }

    return dst - dstBegin;
}

}   /* punycode */


/** \brief Convert UTF-8 data to Punycode.
 */
std::string utf8ToPuny(const std::string &string)
{
    return UTF32_TO_PUNYCODE(UTF8_TO_UTF32(string));
}


/** \brief Convert UTF-16 data to Punycode.
 */
std::string utf16ToPuny(const std::string &string)
{
    return UTF32_TO_PUNYCODE(UTF16_TO_UTF32(string));
}


/** \brief Convert UTF-32 data to Punycode.
 */
std::string utf32ToPuny(const std::string &string)
{
    // arguments
    const size_t srclen = string.size() / 4;
    const size_t dstlen = srclen * 6;
    auto *src = reinterpret_cast<const uint32_t*>(string.data());
    auto *srcEnd = src + srclen;
    auto *dst = reinterpret_cast<uint8_t*>(malloc(dstlen));
    auto *dstEnd = dst + dstlen;

    size_t out = punycode::encode(src, srcEnd, dst, dstEnd);
    std::string output(reinterpret_cast<const char*>(dst), out);
    free(dst);

    return output;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
