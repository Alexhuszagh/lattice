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
    std::vector<lattice::Url> urls = {
        {"http://www.uniprot.org/uniprot/P46406.fasta"},
        {"http://www.uniprot.org/uniprot/P02769.fasta"},
    };

    lattice::Pool pool;
    lattice::Timeout timeout(1000);
    for (auto &&url: urls) {
        pool.get(url, timeout);
    }
    auto responses = pool.perform();
    // TODO: need to do something with this...

    return 0;
}
