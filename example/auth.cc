//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with basic auth.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    authentication_t auth = {"user", "passwd"};
    url_t url = {"http://httpbin.org/basic-auth/user/passwd"};
    timeout_t timeout(1000);

    // set cookies
    auto response = Get(url, auth, timeout);
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
