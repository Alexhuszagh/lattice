//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP POST with file uploads.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Multipart multipart = {
        lattice::CreateFile("a.json"),
        lattice::CreateFile("b.txt"),
        lattice::CreateFile("c.xml"),
        lattice::CreateBuffer("d.csv", "A,B\nC,D"),
    };
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
