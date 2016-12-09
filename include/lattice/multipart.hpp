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
    std::string type;

    PartValue();
    PartValue(const std::string &filename,
        const std::string &type);
    PartValue(std::string &&filename,
        std::string &&type);
    PartValue(const PartValue &other);

    std::string basename() const;
    std::string name() const;
    const std::string & contentType() const;
    virtual std::string string() const;
};


/** \brief Data for a multipart message from file.
 */
struct FileValue: PartValue
{
    FileValue();
    FileValue(const std::string &filename,
        const std::string &type = "");
    FileValue(std::string &&filename,
        std::string &&type);
    FileValue(const FileValue &other);

    std::string buffer() const;
    std::string string() const override;
};


/** \brief Data for a multipart message from buffer.
 */
struct BufferValue: PartValue
{
    std::string contents;

    BufferValue();
    BufferValue(const std::string &filename,
        const std::string &contents,
        const std::string &contentType = "");
    BufferValue(std::string &&filename,
        std::string &&contents,
        std::string &&contentType);
    BufferValue(const BufferValue &other);

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
    return detail::FilePtr(new detail::FileValue(FORWARD(ts)...));
}


/** \brief Wrapper to create a file partition.
 */
template <typename... Ts>
detail::BufferPtr CreateBuffer(Ts&&... ts)
{
    return detail::BufferPtr(new detail::BufferValue(FORWARD(ts)...));
}


// OBJECTS
// -------


/** \brief Data for a multipart
 */
class Multipart: public std::vector<detail::PartPtr>
{
protected:
    typedef std::vector<detail::PartPtr> Base;
    std::string separator;

public:
    Multipart();
    Multipart(std::initializer_list<detail::PartPtr> &&list);
    Multipart(const Multipart &other);

    void add(const detail::PartPtr &part);
    void add(detail::PartPtr &&part);
    const std::string & boundary() const;
    std::string header() const;

    std::string string() const;
    explicit operator bool() const;
};


}   /* lattice */