//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP/HTTPS request.
 */

#include "url.hpp"


namespace lattice
{
// OBJECTS
// -------


/** \brief Proxy to tunnel communications to server.
 */
class Proxy: public Url
{
public:
    using Url::Url;

    explicit operator bool() const;
};


}   /* lattice */
