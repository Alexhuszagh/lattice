//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP request parameters.
 */

#pragma once

#include <itl/string.hpp>


namespace lattice
{
// OBJECTS
// -------


/** \brief Request parameter.
 */
struct Parameter
{
    std::string key;
    std::string value;

    Parameter() = default;
    Parameter(const Parameter &other) = default;
    Parameter & operator=(const Parameter&) = default;
    Parameter(Parameter&&) = default;
    Parameter & operator=(Parameter&&) = default;

    Parameter(std::string &&key,
        std::string &&value);
    Parameter(const std::string &key,
        const std::string &value);
};


/** \brief HTTP parameters object.
 */
class Parameters: public itl::string
{
protected:
    typedef itl::string Base;

public:
    Parameters() = default;
    Parameters(const Parameters &other) = default;
    Parameters & operator=(const Parameters&) = default;
    Parameters(Parameters&&) = default;
    Parameters & operator=(Parameters&&) = default;

    using Base::Base;
    Parameters(const std::initializer_list<Parameter>& parameters);

    Parameters & add(const Parameter &parameter);

    std::string get() const;
    const std::string & post() const;
    explicit operator bool() const;
};


/** \brief HTTP parameters object for a POST request.
 */
struct Body: Parameters
{
    using Parameters::Parameters;
};


/** \brief HTTP parameters object for a POST request.
 */
struct Payload: Parameters
{
    using Parameters::Parameters;
};


}   /* lattice */
