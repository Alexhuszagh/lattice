//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP DELETE requests.
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
    lattice::Url url = {"http://httpbin.org/delete"};
    lattice::timeout_t timeout(1000);
    auto response = lattice::Delete(url, parameters, timeout, cache);

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
