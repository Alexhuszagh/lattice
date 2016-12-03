//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :license: Boost, see licenses/Boost.txt for more details.
/**
 *  \addtogroup Lattice
 *  \brief Parese request response.
 */

#include "lattice.hpp"

#include <sstream>
#include <string>
#include <iostream>             // remove


namespace lattice
{
// OBJECTS
// -------


/** \brief Parse header from the HTTP response.
 */
void Response::parseHeader(const std::string &line)
{
    // TODO: here...
}


/** \brief Null constructor.
 */
Response::Response()
{}


/** \brief Initializer constructor.
 */
Response::Response(const std::string &data)
{
    std::istringstream stream(data);
    std::string line;

    // parse headers
    bool headers = true;
    while (headers && std::getline(stream, line)) {
        std::cout << line << std::endl;
        if (headers = !line.empty()) {
            parseHeader(line);
        }
    }

    // parse body
}


/** \brief Copy constructor.
 */
Response::Response(const Response &other)
{}


/** \brief Destructor.
 */
Response::~Response()
{}


}   /* lattice */
