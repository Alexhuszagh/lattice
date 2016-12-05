//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTPS certificate file.
 */

#pragma once

#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Holds path to a local certificate file.
 */
class CertificateFile: public std::string
{
protected:
    typedef std::string Base;

public:
    using Base::Base;
};

}   /* lattice */
