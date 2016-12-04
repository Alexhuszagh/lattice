//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP HEAD request.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Url url = {"http://httpbin.org/get"};
    lattice::Timeout timeout(1000);
    auto response = lattice::Head(url, timeout);

    if (response.status() == 200) {
        std::cout << "Server:\n"
                  << "-------\n"
                  << response.headers().at("server") << "\n"
                  << "-------\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status()  << std::endl;
    }

    return 0;
}
