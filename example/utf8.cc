//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with internationalized domain names.
 */

#include <lattice.h>
#include <cassert>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    // "http://räksmörgås.josefsson.org/"
    url_t url = {
        104, 116, 116, 112, 58, 47, 47, 114, -61, -92,
        107, 115, 109, -61, -74, 114, 103, -61, -91,
        115, 46, 106, 111, 115, 101, 102, 115, 115, 111,
        110, 46, 111, 114, 103, 47
    };
    timeout_t timeout(1000);
    auto response = Get(url, timeout);

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
