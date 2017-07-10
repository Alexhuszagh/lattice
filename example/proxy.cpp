//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief GET request through an HTTP proxy.
 *
 *  This example requires [Node.js][1] and [http-proxy][2], and the
 *  proxy server can be initialized with:
 *
 *  \code
 *      node node/forwarding-proxy.js
 *
 *  \warning Make sure to open port 8008 and 9008 on your firewall.
 *
 *  [1] https://nodejs.org/
 *  [2] https://github.com/nodejitsu/node-http-proxy
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::proxy_t proxy = {"http://127.0.0.1:8008/"};
    lattice::url_t url = {"http://httpbin.org/get"};
    lattice::timeout_t timeout(1000);

    auto response = lattice::Get(url, proxy, timeout);
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
