//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Create no-opt adapter.
 */

#include <lattice/dns.hpp>
#include <lattice/ssl.hpp>

#include <cassert>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4722)
#endif


namespace lattice
{
// OBJECTS
// -------


/**
 *  \brief No-opt SSL socket adapter.
 */
template <typename HttpAdapter>
struct no_ssl_adaptor_t
{
    typedef no_ssl_adaptor_t<HttpAdapter> self;

    no_ssl_adaptor_t();
    no_ssl_adaptor_t(const self&) = delete;
    self & operator=(const self&) = delete;
    ~no_ssl_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const std::string& host);
    void close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);
};


// IMPLEMENTATION
// --------------


template <typename HttpAdapter>
no_ssl_adaptor_t<HttpAdapter>::no_ssl_adaptor_t()
{}


template <typename HttpAdapter>
no_ssl_adaptor_t<HttpAdapter>::~no_ssl_adaptor_t()
{}


template <typename HttpAdapter>
bool no_ssl_adaptor_t<HttpAdapter>::open(const addrinfo &info,
    const std::string &host)
{
    assert(false);
    return false;
}


template <typename HttpAdapter>
void no_ssl_adaptor_t<HttpAdapter>::close()
{
    assert(false);
}


template <typename HttpAdapter>
size_t no_ssl_adaptor_t<HttpAdapter>::write(const char *buf,
    size_t len)
{
    assert(false);
    return 0;
}


template <typename HttpAdapter>
size_t no_ssl_adaptor_t<HttpAdapter>::read(char *buf, size_t count)
{
    assert(false);
    return 0;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
