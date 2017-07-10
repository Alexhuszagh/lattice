//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include <lattice/url.hpp>


namespace lattice
{
// OBJECTS
// -------


/**
 *  \brief Proxy to tunnel communications to server.
 */
struct proxy_t: url_t
{
    using url_t::url_t;

    explicit operator bool() const;
};


}   /* lattice */
