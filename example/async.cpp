//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple asynchronous GET requests.
 */

#include "lattice.hpp"

#include <iostream>
#include <vector>


int main(int argc, char *argv[])
{
    lattice::Parameters parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    lattice::DnsCache cache;
    lattice::Url get = {"http://httpbin.org/get"};
    lattice::Url post = {"http://httpbin.org/post"};

    lattice::Pool pool;
    lattice::Timeout timeout(1000);
    pool.get(get, cache, timeout, parameters);
    pool.head(get, cache, timeout, parameters);
    pool.post(post, cache, timeout, parameters);

    auto responses = pool.perform();
    for (const auto &response: responses) {
        if (response.status() == 200) {
            std::cout << "Body:\n"
                      << "------------------\n"
                      << response.body()
                      << "------------------\n"
                      << "Encoding: " << response.encoding() << "\n"
                      << "------------------\n";
        } else {
            std::cout << "Response was not successful, error code: "
                      << response.status() << std::endl;
        }
    }

    return 0;
}
