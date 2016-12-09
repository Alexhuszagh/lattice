//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Create session and respond to data from send from server.
 */

#include "lattice.hpp"

#include <iostream>


/** Process data for request and send data to connection.
 */
void sendRequest(lattice::HttpConnection &connection)
{
    // create our options
    lattice::Parameters parameters = {
        {"url", "http://httpbin.org/get"},
    };
    lattice::Url url {"http://httpbin.org/redirect-to"};
    lattice::Timeout timeout(1000);

    // create our request
    lattice::Request request;
    lattice::setOption(request, lattice::GET, url, timeout, parameters);

    // send data and get response
    connection.send(request.message());
}


/** Follow redirect and resend data to new URL.
 */
void sendRedirect(lattice::HttpConnection &connection,
    lattice::Response &response)
{
    // create our options
    lattice::Url url(response.headers().at("location"));
    lattice::Timeout timeout(1000);

    // create our request
    lattice::Request request;
    lattice::setOption(request, lattice::GET, url, timeout);

    // send data and get response
    connection.send(request.message());
}


/** Exit if request failed.
 */
void checkCode(lattice::Response &response,
    const int code)
{
    if (response.status() != code) {
        std::cout << "Could not establish session, error code "
                  << response.status() << ", exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    // initialize connection
    lattice::Url url = {"http://httpbin.org"};
    lattice::HttpConnection connection;
    connection.open(url);

    // create our response and send initial request
    lattice::Response response;
    sendRequest(connection);
    response = lattice::Response(connection);
    checkCode(response, 302);

    // follow redirect
    sendRedirect(connection, response);
    response = lattice::Response(connection);
    checkCode(response, 200);

    std::cout << "Body:\n"
              << "------------------\n"
              << response.body()
              << "------------------\n"
              << "Encoding: " << response.encoding() << "\n"
              << "------------------\n";

    return 0;
}
