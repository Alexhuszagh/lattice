//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP URL object.
 */

#pragma once

#include <initializer_list>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief URL class instance.
 */
class Url: public std::string
{
protected:
    typedef std::string Base;

public:
    Url();
    Url(const char *cstring);
    Url(const char *array,
        size_t size);
    Url(const std::string &string);
    Url(std::initializer_list<char> &&list);
    Url(const Url &url);

    // GETTERS
    std::string service() const noexcept;
    std::string host() const noexcept;
    std::string path() const noexcept;
    std::string directory() const noexcept;
    std::string file() const noexcept;

    // SETTERS
    void setService(const std::string &service);
    void setHost(const std::string &host);
    void setPath(const std::string &path);
    void setDirectory(const std::string &directory);
    void setFile(const std::string &file);

    // PROPERTIES
    bool relative() const noexcept;
    bool absolute() const noexcept;
};

}   /* lattice */
