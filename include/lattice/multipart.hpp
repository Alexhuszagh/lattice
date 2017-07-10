//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Multipart requests (file uploads).
 */

#pragma once

#include <deque>
#include <memory>
#include <string>

namespace lattice
{
namespace detail
{
// TYPES
// -----

struct part_value_t;
struct file_value_t;
struct buffer_value_t;

typedef std::shared_ptr<part_value_t> part_ptr_t;
typedef std::shared_ptr<file_value_t> file_ptr_t;
typedef std::shared_ptr<buffer_value_t> buffer_ptr_t;

// FUNCTIONS
// ---------

/**
 *  \brief Get boundary value for MIME message.
 */
std::string get_boundary();

// OBJECTS
// -------


/**
 *  \brief Base class for a multipart item.
 */
struct part_value_t
{
    std::string filename;
    std::string content_type_;

    part_value_t() = default;
    part_value_t(const part_value_t &other) = default;
    part_value_t & operator=(const part_value_t&) = default;
    part_value_t(part_value_t&&) = default;
    part_value_t & operator=(part_value_t&&) = default;

    part_value_t(const std::string& filename, const std::string& content_type = "");
    part_value_t(std::string&& filename, std::string&& content_type = "");

    std::string basename() const;
    std::string name() const;
    const std::string & content_type() const;
    virtual std::string string() const;
};


/**
 *  \brief Data for a multipart message from file.
 */
struct file_value_t: part_value_t
{
    using part_value_t::part_value_t;

    std::string buffer() const;
    std::string string() const override;
};


/**
 *  \brief Data for a multipart message from buffer.
 */
struct buffer_value_t: part_value_t
{
    std::string buffer_;

    buffer_value_t() = default;
    buffer_value_t(const buffer_value_t &other) = default;
    buffer_value_t & operator=(const buffer_value_t&) = default;
    buffer_value_t(buffer_value_t&&) = default;
    buffer_value_t & operator=(buffer_value_t&&) = default;

    buffer_value_t(const std::string &filename,
        const std::string &buffer,
        const std::string &content_type = "");
    buffer_value_t(std::string &&filename,
        std::string &&buffer,
        std::string &&content_type);

    const std::string & buffer() const;
    std::string string() const override;
};

}   /* detail */


// HELPERS
// -------


template <typename... Ts>
detail::file_ptr_t create_file(Ts&&... ts)
{
    return std::make_shared<detail::file_value_t>(std::forward<Ts>(ts)...);
}


template <typename... Ts>
detail::buffer_ptr_t create_buffer(Ts&&... ts)
{
    return std::make_shared<detail::buffer_value_t>(std::forward<Ts>(ts)...);
}


// OBJECTS
// -------


/**
 *  \brief Data for a multipart
 */
class multipart_t: public std::deque<detail::part_ptr_t>
{
public:
    typedef std::deque<detail::part_ptr_t> base;
    using base::base;

    void add(const detail::part_ptr_t &part);
    void add(detail::part_ptr_t &&part);
    const std::string & boundary() const;
    std::string header() const;

    std::string string() const;
    explicit operator bool() const;

private:
    std::string boundary_ = detail::get_boundary();
};

}   /* lattice */
