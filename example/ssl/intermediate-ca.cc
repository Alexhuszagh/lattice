//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief GET request to a certificate validated through an intermediate CA.
 */

#include <lattice.h>
#include <iostream>

LATTICE_USING_NAMESPACE


int main(int argc, char *argv[])
{
    url_t url = {"https://xn--rksmrgs-5wao1o.josefsson.org"};
    timeout_t timeout(1000);
    certificate_file_t certificate("/usr/lib/python3/dist-packages/websocket/cacert.pem");
    auto response = Get(url, timeout, certificate);

    if (response.status() == 200) {
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
