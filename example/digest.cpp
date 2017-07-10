//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with digest auth.
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::digest_t digest = {"user", "pass"};
    lattice::url_t url = {"http://httpbin.org/digest-auth/auth/user/pass"};
    lattice::timeout_t timeout(1000);

    // set cookies
    auto response = lattice::Get(url, digest, timeout);
    if (response.ok()) {
        std::cout << "Body:\n"
                  << "------------------\n"
                  << response.body()
                  << "------------------\n"
                  << "Encoding: " << response.encoding() << "\n"
                  << "------------------\n";
    } else {
      std::cout << "Shit: "
                << response.status()  << std::endl;
    }

    return 0;
}
