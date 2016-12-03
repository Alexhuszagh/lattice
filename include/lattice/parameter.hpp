//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP request parameters.
 */

#pragma once

#include <initializer_list>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Request parameter.
 */
struct Parameter
{
    Parameter();
    Parameter(std::string &&key,
        std::string &&value);
    Parameter(const Parameter &other);

    std::string key;
    std::string value;
};


/** \brief HTTP parameters object.
 */
class Parameters: public std::string
{
public:
    Parameters();
    Parameters(const std::initializer_list<Parameter>& parameters);

    Parameters & add(const Parameter &parameter);
};

}   /* lattice */
