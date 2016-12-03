//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple HTTP POST request.
 */

#include "lattice.hpp"

#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Parameters parameters = {
        {"name1", "value1"},
        {"name2", "value2"},
    };

    lattice::Url url = {"http://w3schools.com/test/demo_form.asp"};
    lattice::Timeout timeout(1000);
    auto response = lattice::Post(url, parameters, timeout);

    // TODO: this is good-ish
    //std::cout << parameters << std::endl;

    // TODO: need to do the URL

    return 0;
}
