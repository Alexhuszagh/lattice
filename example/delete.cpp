//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP DELETE requests.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Parameters parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    auto cache = lattice::CreateDnsCache();
    lattice::Url url = {"http://httpbin.org/delete"};
    lattice::Timeout timeout(1000);
    auto response = lattice::Delete(url, parameters, timeout, cache);

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
