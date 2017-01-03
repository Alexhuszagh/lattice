//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief URL object.
 */

#include "lattice.hpp"

#include <cassert>


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief Check if URL is relative.
 */
inline bool isRelative(const std::string &string) noexcept
{
    return string.empty() || string.front() == '/';
}


/** \brief Get Punycode-encoded URLK.
 */
void punyEncodedUrl(Url &url)
{
    if (url.absolute()) {
        auto names = split(url.host(), ".");
        for (auto &name: names) {
            if (isUnicode(name)) {
                name = "xn--" + UTF8_TO_PUNYCODE(name);
            }
        }
        url.setHost(join(names, "."));
    }
}


// OBJECTS
// -------


/** \brief Initialize from null-terminated C-string.
 */
Url::Url(const char *cstring):
    itl::string(cstring)
{
    punyEncodedUrl(*this);
}


/** \brief Initialize from char array.
 */
Url::Url(const char *array,
        size_t size):
    itl::string(array, size)
{
    punyEncodedUrl(*this);
}


/** \brief Copy constructor.
 */
Url::Url(const std::string &string):
    itl::string(string.data(), string.size())
{
    punyEncodedUrl(*this);
}


/** \brief Initializer list constructor.
 */
Url::Url(std::initializer_list<char> &&list):
    itl::string(LATTICE_FWD(list))
{
    punyEncodedUrl(*this);
}


/** \brief Get service from  URL.
 */
std::string Url::service() const noexcept
{
    assert(absolute());
    size_t index = find("://");
    if (index == itl::string::npos) {
        #ifdef HAVE_SSL
            return "https";
        #else
            return "http";
        #endif
    }
    return substr(0, index).data();
}


/** \brief Get hostname from  URL.
 */
std::string Url::host() const noexcept
{
    assert(absolute());
    size_t start = find("://");
    if (start == itl::string::npos) {
        return substr(0, find_first_of('/')).data();
    }
    size_t end = find_first_of('/', start+4);
    if (end == itl::string::npos) {
        substr(start+3).data();
    }
    return substr(start+3, end-start-3).data();
}


/** \brief Get hostname from  URL.
 */
std::string Url::path() const noexcept
{
    if (relative()) {
        return *this;
    }

    const size_t separator = find("://");
    size_t start;
    if (separator == itl::string::npos) {
        start = find_first_of('/');
    } else {
        start = find_first_of('/', separator+4);
    }
    if (start == itl::string::npos) {
        return "/";
    }
    return substr(start).data();
}


/** \brief Get directory component of URL.
 */
std::string Url::directory() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == itl::string::npos) {
        return data.substr(1);
    }

    return data.substr(1, separator - 1);
}


/** \brief Get file component of URL.
 */
std::string Url::file() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == itl::string::npos) {
        return data;
    }

    return data.substr(separator + 1);
}


/** \brief Set service component of URL.
 */
void Url::setService(const std::string &service)
{
    assert(absolute());
    size_t index = find("://");
    if (index != itl::string::npos) {
        // replace service
        replace(0, index, service);
    } else {
        // set a service
        insert(0, service + "://");
    }
}


/** \brief Set host component of URL.
 */
void Url::setHost(const std::string &host)
{
    size_t start = find("://");
    if (start == itl::string::npos) {
        replace(0, find_first_of('/'), host);
    } else {
        size_t end = find_first_of('/', start+4);
        replace(start+3, end-start-3, host);
    }
}


/** \brief Set path component of URL.
 */
void Url::setPath(const std::string &path)
{
    if (relative()) {
        assign(path);
    } else {
        size_t separator = find("://");
        if (separator == itl::string::npos) {
            replace(find_first_of('/'), itl::string::npos, path);
        } else {
            replace(find_first_of('/', separator+4), itl::string::npos, path);
        }
    }
}


/** \brief Set directory component of URL.
 */
void Url::setDirectory(const std::string &directory)
{
    size_t separator, start, end;
    end = find_last_of('/');
    if ((separator = find("://")) != itl::string::npos) {
        start = find_first_of('/', separator+4);
    } else {
        start = find_first_of('/');
    }

    if (start != itl::string::npos && ++start < end) {
        const size_t length = end - start;
        replace(start, length, directory);
    }
}


/** \brief Set file component of URL.
 */
void Url::setFile(const std::string &file)
{
    size_t index = find_last_of('/');
    replace(index + 1, itl::string::npos, file);
}



/** \brief URL is a relative identifier.
 */
bool Url::relative() const noexcept
{
    return isRelative(*this);
}


/** \brief URL is an absolute identifier.
 */
bool Url::absolute() const noexcept
{
    return !relative();
}


/** \brief Check if a URL has been set.
 */
Url::operator bool() const
{
    return !empty();
}


}   /* lattice */
