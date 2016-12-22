//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Multipart requests (file uploads).
 */

#pragma once

#include "util.hpp"

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>


namespace lattice
{
namespace detail
{
// TYPES
// -----

struct PartValue;
struct FileValue;
struct BufferValue;

typedef std::shared_ptr<PartValue> PartPtr;
typedef std::shared_ptr<FileValue> FilePtr;
typedef std::shared_ptr<BufferValue> BufferPtr;

// OBJECTS
// -------


/** \brief Base class for a multipart item.
 */
struct PartValue
{
    std::string filename;
    std::string contentType_;

    PartValue() = default;
    PartValue(const PartValue &other) = default;
    PartValue & operator=(const PartValue&) = default;
    PartValue(PartValue&&) = default;
    PartValue & operator=(PartValue&&) = default;

    PartValue(const std::string &filename,
        const std::string &contentType = "");
    PartValue(std::string &&filename,
        std::string &&contentType = "");

    std::string basename() const;
    std::string name() const;
    const std::string & contentType() const;
    virtual std::string string() const;
};


/** \brief Data for a multipart message from file.
 */
struct FileValue: PartValue
{
    using PartValue::PartValue;

    std::string buffer() const;
    std::string string() const override;
};


/** \brief Data for a multipart message from buffer.
 */
struct BufferValue: PartValue
{
    std::string buffer_;

    BufferValue() = default;
    BufferValue(const BufferValue &other) = default;
    BufferValue & operator=(const BufferValue&) = default;
    BufferValue(BufferValue&&) = default;
    BufferValue & operator=(BufferValue&&) = default;

    BufferValue(const std::string &filename,
        const std::string &buffer,
        const std::string &contentType = "");
    BufferValue(std::string &&filename,
        std::string &&buffer,
        std::string &&contentType);

    const std::string & buffer() const;
    std::string string() const override;
};

}   /* detail */


// HELPERS
// -------


/** \brief Wrapper to create a file partition.
 */
template <typename... Ts>
detail::FilePtr CreateFile(Ts&&... ts)
{
    return detail::FilePtr(new detail::FileValue(LATTICE_FWD(ts)...));
}


/** \brief Wrapper to create a file partition.
 */
template <typename... Ts>
detail::BufferPtr CreateBuffer(Ts&&... ts)
{
    return detail::BufferPtr(new detail::BufferValue(LATTICE_FWD(ts)...));
}


// OBJECTS
// -------


/** \brief Data for a multipart
 */
class Multipart: public std::vector<detail::PartPtr>
{
protected:
    typedef std::vector<detail::PartPtr> Base;
    std::string boundary_ = SHA1_HEX(pseudorandom(8));

public:
    using Base::Base;

    void add(const detail::PartPtr &part);
    void add(detail::PartPtr &&part);
    const std::string & boundary() const;
    std::string header() const;

    std::string string() const;
    explicit operator bool() const;
};


}   /* lattice */
