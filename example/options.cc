//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP OPTIONS request.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    url_t url = {"http://httpbin.org/"};
    timeout_t timeout(1000);
    auto response = Options(url, timeout);

    if (response.ok()) {
        std::cout << "Allow:\n"
                  << "------------------\n"
                  << response.headers().at("allow") << "\n"
                  << "------------------\n"
                  << "Encoding: " << response.encoding() << "\n"
                  << "------------------\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status() << std::endl;
    }

    return 0;
}
