//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP TRACE requests.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    auto cache = create_dns_cache();
    url_t url = {"http://httpbin.org/"};
    timeout_t timeout(1000);
    auto response = Trace(url, timeout, cache);

    if (response.status() == 405) {
        std::cout << "Method not allowed returned, trace blocked\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status()  << std::endl;
    }

    return 0;
}
