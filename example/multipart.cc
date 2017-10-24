//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP POST with file uploads.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    multipart_t multipart = {
        create_file("a.json"),
        create_file("b.txt"),
        create_file("c.xml"),
        create_buffer("d.csv", "A,B\nC,D"),
    };
    url_t url = {"http://httpbin.org/post"};
    timeout_t timeout(1000);
    auto response = Post(url, multipart, timeout);

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
