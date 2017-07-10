//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Connect to server with a certificate signed by a root CA.
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::parameters_t parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    lattice::url_t url = {"https://httpbin.org/get"};
    lattice::timeout_t timeout(1000);
    lattice::certificate_file_t certificate("client.crt");
    auto response = lattice::Get(url, timeout, certificate);

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
