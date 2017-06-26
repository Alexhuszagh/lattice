//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with basic auth.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Authentication auth = {"user", "passwd"};
    lattice::Url url = {"http://httpbin.org/basic-auth/user/passwd"};
    lattice::Timeout timeout(1000);

    // set cookies
    auto response = lattice::Get(url, auth, timeout);
    if (response.ok()) {
        std::cout << "Body:\n"
                  << "------------------\n"
                  << response.body()
                  << "------------------\n"
                  << "Encoding: " << response.encoding() << "\n"
                  << "------------------\n";
    } else {
      std::cout << "Could not authenticate, error code: "
                << response.status()  << std::endl;
    }

    return 0;
}
