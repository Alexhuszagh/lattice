//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple asynchronous GET requests.
 */

#include <lattice.hpp>

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::parameters_t parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    auto cache = lattice::create_dns_cache();
    lattice::Url get = {"http://httpbin.org/get"};
    lattice::Url post = {"http://httpbin.org/post"};

    lattice::pool_t pool;
    lattice::timeout_t timeout(1000);
    pool.get(get, cache, timeout, parameters);
    pool.head(get, cache, timeout, parameters);
    pool.post(post, cache, timeout, parameters);

    while (pool) {
        auto response = pool.next(1);
        if (response && response.ok()) {
            std::cout << "Body:\n"
                      << "------------------\n"
                      << response.body()
                      << "------------------\n"
                      << "Encoding: " << response.encoding() << "\n"
                      << "------------------\n";
        } else if (response) {
            std::cout << "Response was not successful, error code: "
                      << response.status() << std::endl;
        }
    }

    return 0;
}
