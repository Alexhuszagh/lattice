//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#include <lattice/async.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


response_list_t pool_t::perform()
{
    response_list_t list;
    for (auto &&future: futures) {
        if (future.valid()) {
            list.emplace_back(future.get());
        }
    }

    return list;
}


pool_t::operator bool() const
{
    return !futures.empty();
}

LATTICE_END_NAMESPACE
