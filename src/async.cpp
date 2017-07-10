//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#include <lattice/async.hpp>


namespace lattice
{
// OBJECTS
// -------


/** \brief Block and return all queries.
 */
ResponseList Pool::perform()
{
    ResponseList list;
    for (auto &&future: futures) {
        if (future.valid()) {
            list.emplace_back(future.get());
        }
    }

    return list;
}


/** \brief Check if more items can be returns.
 */
Pool::operator bool() const
{
    return !futures.empty();
}


}   /* lattice */
