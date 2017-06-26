//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with cookies.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Parameters cookies = {
        {"name", "value"},
    };
    auto cache = lattice::CreateDnsCache();
    lattice::Url url = {"http://httpbin.org/cookies/set"};
    lattice::Timeout timeout(1000);
    lattice::Redirects redirects(5);

    // set cookies
    auto response = lattice::Get(url, cookies, timeout, redirects, cache);
    if (response.ok()) {
        // delete the cookies
        url = {"http://httpbin.org/cookies/delete"};
        cookies = {{"name", ""}};
        response = lattice::Get(url, cookies, timeout, redirects, cache);
        if (response.ok()) {
            std::cout << "Successfully set and deleted cookies" << std::endl;
        } else {
            std::cout << "Could not delete the set cookies, error code: "
                      << response.status()  << std::endl;
        }
    } else {
      std::cout << "Could not set the cookies, error code: "
                << response.status()  << std::endl;
    }

    return 0;
}
