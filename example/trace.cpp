//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP TRACE requests.
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    auto cache = lattice::create_dns_cache();
    lattice::url_t url = {"http://httpbin.org/"};
    lattice::timeout_t timeout(1000);
    auto response = lattice::Trace(url, timeout, cache);

    if (response.status() == 405) {
        std::cout << "Method not allowed returned, trace blocked\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status()  << std::endl;
    }

    return 0;
}
