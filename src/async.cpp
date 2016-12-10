//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Asynchronous requests.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
Pool::Pool()
{}


/** \brief Destructor.
 */
Pool::~Pool()
{}


/** \brief Block and return all queries.
 */
std::vector<Response> Pool::perform()
{
    std::vector<Response> responses;
    responses.reserve(futures.size());

    for (auto &&future: futures) {
        if (future.valid()) {
            responses.emplace_back(future.get());
        }
    }

    return responses;
}


/** \brief Check if more items can be returns.
 */
Pool::operator bool() const
{
    return !futures.empty();
}


}   /* lattice */
