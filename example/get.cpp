//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP GET request.
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::parameters_t parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    auto cache = lattice::create_dns_cache();
    lattice::url_t url = {"http://httpbin.org/get"};
    lattice::timeout_t timeout(1000);
    auto response = lattice::Get(url, parameters, timeout, cache);

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
