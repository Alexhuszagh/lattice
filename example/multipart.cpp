//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP POST with file uploads.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Multipart multipart;
    multipart.emplace_back(lattice::CreateFile("a.json"));
    multipart.emplace_back(lattice::CreateFile("b.txt"));
    multipart.emplace_back(lattice::CreateFile("c.xml"));
    lattice::Url url = {"http://httpbin.org/post"};
    lattice::Timeout timeout(1000);
    auto response = lattice::Post(url, multipart, timeout);

    if (response.ok()) {
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

    return 0;
}
