//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Network request parameters.
 */

#include <lattice/parameter.h>
#include <pycpp/string/url.h>

LATTICE_BEGIN_NAMESPACE

// OBJECTS
// -------


parameter_t::parameter_t(std::string&& key, std::string&& value):
    key(std::forward<std::string>(key)),
    value(std::forward<std::string>(value))
{}


parameter_t::parameter_t(const std::string& key, const std::string& value):
    key(key),
    value(value)
{}


parameters_t::parameters_t(const std::initializer_list<parameter_t>& parameters)
{
    for (const auto &parameter: parameters) {
        add(parameter);
    }
}


parameters_t & parameters_t::add(const parameter_t &parameter)
{
    // add delimiter
    if (!empty()) {
        append("&");
    }

    // add escape values
    if (parameter.value.empty()) {
        append(PYCPP_NAMESPACE::url_encode(parameter.key));
    } else {
        append(PYCPP_NAMESPACE::url_encode(parameter.key) + "=" + PYCPP_NAMESPACE::url_encode(parameter.value));
    }

    return *this;
}


std::string parameters_t::get() const
{
    if (empty()) {
        return static_cast<const std::string&>(*this);
    }
    return "?" + static_cast<const std::string&>(*this);
}


const std::string& parameters_t::post() const
{
    return static_cast<const std::string&>(*this);
}


parameters_t::operator bool() const
{
    return !empty();
}

LATTICE_END_NAMESPACE
