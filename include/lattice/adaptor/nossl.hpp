//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Create no-opt adaptor.
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
 *  \brief No-opt SSL socket adaptor.
 */
template <typename HttpAdaptor>
struct no_ssl_adaptor_t
{
    typedef no_ssl_adaptor_t<HttpAdaptor> self;

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


template <typename HttpAdaptor>
no_ssl_adaptor_t<HttpAdaptor>::no_ssl_adaptor_t()
{}


template <typename HttpAdaptor>
no_ssl_adaptor_t<HttpAdaptor>::~no_ssl_adaptor_t()
{}


template <typename HttpAdaptor>
bool no_ssl_adaptor_t<HttpAdaptor>::open(const addrinfo &info,
    const std::string &host)
{
    assert(false);
    return false;
}


template <typename HttpAdaptor>
void no_ssl_adaptor_t<HttpAdaptor>::close()
{
    assert(false);
}


template <typename HttpAdaptor>
size_t no_ssl_adaptor_t<HttpAdaptor>::write(const char *buf,
    size_t len)
{
    assert(false);
    return 0;
}


template <typename HttpAdaptor>
size_t no_ssl_adaptor_t<HttpAdaptor>::read(char *buf, size_t count)
{
    assert(false);
    return 0;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
