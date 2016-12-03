//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Network request parameters.
 */

#include "lattice.hpp"


namespace lattice
{
// OBJECTS
// -------


/** Null constructor.
 */
Parameter::Parameter()
{}


/** \brief Initializer list initializer.
 */
Parameter::Parameter(std::string &&key,
        std::string &&value):
    key(FORWARD(key)),
    value(FORWARD(value))
{}


/** Copy constructor.
 */
Parameter::Parameter(const Parameter &other):
    key(other.key),
    value(other.value)
{}



/** Null constructor.
 */
Parameters::Parameters()
{}


/** \brief Initializer list initializer.
 */
Parameters::Parameters(const std::initializer_list<Parameter>& parameters)
{
    for (const auto &parameter: parameters) {
        add(parameter);
    }
}


/** \brief Add parameter.
 */
Parameters & Parameters::add(const Parameter &parameter)
{
    // add delimiter
    if (!empty()) {
        append("&");
    }

    // add escape values
    if (parameter.value.empty()) {
        append(encode(parameter.key));
    } else {
        append(encode(parameter.key) + "=" + encode(parameter.value));
    }

    return *this;
}

}   /* lattice */
