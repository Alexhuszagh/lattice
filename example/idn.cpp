//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with internationalized domain names.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Url url = {"http://xn--rksmrgs-5wao1o.josefsson.org"};
    // TODO: need to custom convert to and from the IDN
    lattice::Timeout timeout(1000);
    auto response = lattice::Get(url, timeout);

    if (response.status() == 200) {
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
