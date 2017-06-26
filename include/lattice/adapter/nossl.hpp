//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Create no-opt adapter.
 */

#include "lattice/dns.hpp"
#include "lattice/ssl.hpp"
#include "lattice/util/exception.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4722)
#endif


namespace lattice
{
// OBJECTS
// -------


/** \brief No-opt SSL socket adapter.
 */
template <typename HttpAdapter>
class NoSslAdapter
{
protected:
    typedef NoSslAdapter<HttpAdapter> This;

public:
    NoSslAdapter();
    NoSslAdapter(const This &other) = delete;
    ~NoSslAdapter();

    // REQUESTS
    bool open(const addrinfo &info,
        const std::string &host);
    void close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);
};


// IMPLEMENTATION
// --------------


/** \brief Null constructor.
 */
template <typename HttpAdapter>
NoSslAdapter<HttpAdapter>::NoSslAdapter()
{}


/** \brief Destructor.
 */
template <typename HttpAdapter>
NoSslAdapter<HttpAdapter>::~NoSslAdapter()
{}


/** \brief Open socket.
 */
template <typename HttpAdapter>
bool NoSslAdapter<HttpAdapter>::open(const addrinfo &info,
    const std::string &host)
{
    throw MissingSslError();
    return false;
}


/** \brief Close socket.
 */
template <typename HttpAdapter>
void NoSslAdapter<HttpAdapter>::close()
{
    throw MissingSslError();
}


/** \brief Write data to socket.
 */
template <typename HttpAdapter>
size_t NoSslAdapter<HttpAdapter>::write(const char *buf,
    size_t len)
{
    throw MissingSslError();
    return 0;
}


/** \brief Read data from socket.
 */
template <typename HttpAdapter>
size_t NoSslAdapter<HttpAdapter>::read(char *buf,
    size_t count)
{
    throw MissingSslError();
    return 0;
}

}   /* lattice */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif
