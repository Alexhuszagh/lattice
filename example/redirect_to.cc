//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Follow absolute redirects from GET request.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    parameters_t parameters = {
        {"url", "http://httpbin.org/get"},
    };
    url_t url = {"http://httpbin.org/redirect-to"};
    timeout_t timeout(1000);
    redirects_t redirects(5);
    auto response = Get(url, timeout, redirects, parameters);

    if (response.ok()) {
        std::cout << "Body:\n"
                  << "------------------\n"
                  << response.body()
                  << "------------------\n"
                  << "Encoding: " << response.encoding() << "\n"
                  << "------------------\n";
    } else {
        std::cout << "Response was not successful, error code: "
                  << response.status();
    }

    return 0;
}
