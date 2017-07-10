//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with a UTF-16 domain name.
 *
 *  Lattice provides 6 functions to convert between Unicode encodings:
 *  `UTF8_TO_UTF16`, `UTF8_TO_UTF32`, `UTF16_TO_UTF8`,
 *  `UTF8_TO_UTF32`, `UTF32_TO_UTF8`, `UTF32_TO_UTF16`.
 *
 *  Lattice expects all arguments passed to it are UTF-8 encoded.
 */

#include <lattice.hpp>

#include <pycpp/unicode.h>
#include <cassert>
#include <iostream>


int main(int argc, char *argv[])
{
    // "http://räksmörgås.josefsson.org/"
    #if BYTE_ORDER == LITTLE_ENDIAN
        lattice::url_t url = utf16_to_utf8(std::string({
            104, 0, 116, 0, 116, 0, 112, 0, 58, 0, 47, 0, 47, 0, 120, 0,
            110, 0, 45, 0, 45, 0, 114, 0, 107, 0, 115, 0, 109, 0, 114, 0,
            103, 0, 115, 0, 45, 0, 53, 0, 119, 0, 97, 0, 111, 0, 49, 0,
            111, 0, 46, 0, 106, 0, 111, 0, 115, 0, 101, 0, 102, 0, 115,
            0, 115, 0, 111, 0, 110, 0, 46, 0, 111, 0, 114, 0, 103, 0,
            47, 0
        }));
    #else
        lattice::url_t url = utf16_to_utf8(std::string({
            0, 104, 0, 116, 0, 116, 0, 112, 0, 58, 0, 47, 0, 47, 0, 120,
            0, 110, 0, 45, 0, 45, 0, 114, 0, 107, 0, 115, 0, 109, 0, 114,
            0, 103, 0, 115, 0, 45, 0, 53, 0, 119, 0, 97, 0, 111, 0, 49
            0, 111, 0, 46, 0, 106, 0, 111, 0, 115, 0, 101, 0, 102, 0,
            115, 0, 115, 0, 111, 0, 110, 0, 46, 0, 111, 0, 114, 0, 103,
            0, 47
        }));
    #endif


    lattice::timeout_t timeout(1000);
    auto response = lattice::Get(url, timeout);

    if (response.ok()) {
        std::cout << "Body:\n"
                  << "------------------\n"
                  << response.body()
                  << "------------------\n"
                  << "Encoding: " << response.encoding() << "\n"
                  << "------------------\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status()  << std::endl;
    }

    return 0;
}
