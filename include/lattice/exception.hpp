//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Socket and connection exceptions.
 */

#include <sstream>
#include <stdexcept>


namespace lattice
{
// OBJECTS
// -------


/** \brief Error in in getting an address from the host/service.
 */
class AddressError: public std::exception
{
protected:
    std::string host;
    std::string service;

public:
    AddressError(const std::string &host,
        const std::string &service);

    virtual const char * what() const throw() = 0;
};


/** \brief Error in establishing a connection.
 */
struct ConnectionError: public std::exception
{
    /** \brief Display message from connection error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to establish connection\n";
    }
};


/** \brief Error sending a request to the server.
 */
class RequestError: public std::exception
{
protected:
    int sent;
    int total;

public:
    RequestError(const int sent,
        const int total);

    /** \brief Display number of bytes successfully sent.
     */
    virtual const char * what() const throw()
    {
        std::stringstream stream;
        stream << "Unable to get make request: sent "
               << sent << " of " << total << " bytes\n";

        return stream.str().data();
    }
};


/** \brief Error in settings socket option.
 */
struct SocketOptionError: public std::exception
{
    /** \brief Display message from socket option error.
     */
    virtual const char * what() const throw()
    {
        return "Unable to set socket option via setsockopt()\n";
    }
};

}   /* lattice */
