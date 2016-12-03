//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP GET request.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Parameters parameters = {
        {"sort", "score"},
        {"desc", ""},
        {"fil", ""},
        {"force", "no"},
        {"format", "tab"},
        {"query", "fasta:P46406"},
        {"reviewed", "yes"},
        {"columns", "id,entry name,genes(PREFERRED),sequence"},
    };

    lattice::Url url = {"http://www.uniprot.org/uniprot/"};
    lattice::Timeout timeout(1000);
    auto response = lattice::Get(url, parameters, timeout);

    // TODO: this is good-ish
    //std::cout << parameters << std::endl;

    // TODO: need to do the URL

    return 0;
}
