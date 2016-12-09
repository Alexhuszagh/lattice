//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief HTTP response object.
 */

#pragma once

#include "cookie.hpp"
#include "header.hpp"
#include "method.hpp"
#include "redirect.hpp"
#include "transfer.hpp"
#include "url.hpp"

#include <string>
#include <tuple>
#include <type_traits>


namespace lattice
{
// OBJECTS
// -------


/** \brief HTTP status codes.
 */
enum StatusCode
{
    // INFORMATIONAL
    CONTINUE                            = 100,
    SWITCH_PROTOCOLS                    = 101,
    PROCESSING                          = 102,

    // SUCCESS
    OK                                  = 200,
    CREATED                             = 201,
    ACCEPTED                            = 202,
    MODIFIED                            = 203,
    NO_CONTENT                          = 204,
    RESET_CONTENT                       = 205,
    PARTIAL_CONTENT                     = 206,
    MULTI_STATUS                        = 207,
    ALREADY_REPORTED                    = 208,
    IM_USED                             = 226,

    // REDIRECTION
    MULTIPLE_CHOICES                    = 300,
    MOVED_PERMANENTLY                   = 301,
    FOUND                               = 302,
    SEE_OTHER                           = 303,
    NOT_MODIFIED                        = 304,
    USE_PROXY                           = 305,
    SWITCH_PROXY                        = 306,
    TEMPORARY_REDIRECT                  = 307,
    PERMANENT_REDIRECT                  = 308,

    // CLIENT ERROR
    BAD_REQUEST                         = 400,
    UNAUTHORIZED                        = 401,
    PAYMENT_REQUIRED                    = 402,
    FORBIDDEN                           = 403,
    NOT_FOUND                           = 404,
    METHOD_NOT_ALLOWED                  = 405,
    NOT_ACCEPTABLE                      = 406,
    PROXY_AUTHENTIFICATION_REQUIRED     = 407,
    REQUEST_TIMEOUT                     = 408,
    CONFLICT                            = 409,
    GONE                                = 410,
    LENGTH_REQUIRED                     = 411,
    PRECONDITION_FAILED                 = 412,
    PAYLOAD_TOO_LARGE                   = 413,
    URI_TOO_LONG                        = 414,
    UNSUPPORTED_MEDIA_TYPE              = 415,
    RANGE_NOT_SATISFIABLE               = 416,
    EXPECTATION_FAILED                  = 417,
    IM_A_TEAPOT                         = 418,
    MISDIRECTED_REQUEST                 = 421,
    UNPROCESSABLE_ENTITY                = 422,
    LOCKED                              = 423,
    FAILED_DEPENDENCY                   = 424,
    UPGRADE_REQUIRED                    = 426,
    PRECONDITION_REQUIRED               = 428,
    TOO_MANY_REQUESTS                   = 429,
    REQUEST_HEADERS_TOO_LARGE           = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS       = 451,

    // SERVER ERROR
    INTERNAL_SERVER_ERROR               = 500,
    NOT_IMPLEMENTED                     = 501,
    BAD_GATEWAY                         = 502,
    SERVICE_UNAVAILABLE                 = 503,
    GATEWAY_TIMEOUT                     = 504,
    HTTP_VERSION_ERROR                  = 505,
    CONTENT_NEGOTIATES                  = 506,
    INSUFFICIENT_STORAGE                = 507,
    LOOP_DETECTED                       = 508,
    NOT_EXTENDED                        = 510,
    AUTHENTIFICATION_REQUIRED           = 511,
};


/** \brief Registered types for "Content-Types".
 */
enum ContentType
{
    APPLICATION     = 0,
    AUDIO           = 1,
    IMAGE           = 2,
    MESSAGE         = 3,
    MULTIPART       = 4,
    TEXT            = 5,
    VIDEO           = 6,
    XTOKEN          = 7,
};


/** \brief Generate response from a request.
 */
class Response
{
protected:
    typedef std::tuple<ContentType, std::string> MimeType;

    template <typename T>
    using IsNotSame = typename std::enable_if<!std::is_same<T, Response>::value>::type;

    StatusCode code;
    Header header;
    Cookies cookie;
    TransferEncoding transfer = static_cast<TransferEncoding>(0);
    MimeType mime;
    std::string charset;
    std::string data;

    void parseCode(const std::string &line);
    void parseCookie(const std::string &string);
    void parseTransferEncoding(std::string &string);
    void parseContentType(std::string &string);
    void parseType(const std::string &string);
    void parseHeader(const std::string &line);
    void parseHeaders(const std::string &lines);

public:
    Response();
    Response(const Response &other);
    ~Response();

    template <typename Connection, typename = IsNotSame<Connection>>
    Response(Connection &connection);

    // DATA
    const int status() const;
    const std::string & body() const;
    const Header & headers() const;
    const Cookies & cookies() const;

    // DATA ENCODING
    TransferEncoding transferEncoding() const;
    std::string contentEncoding() const;

    // COMPRESSION
    bool compressed() const;
    bool compress() const;
    bool deflate() const;
    bool exi() const;
    bool gzip() const;
    bool pack200Gzip() const;
    bool br() const;
    bool bzip() const;
    bool bzip2() const;
    bool lzma() const;
    bool peerdist() const;
    bool xpress() const;
    bool xz() const;

    // COMPRESSED FILES
    bool _7z() const;
    bool ace() const;
    bool rar() const;

    // TYPE
    const MimeType & type() const;
    bool application() const;
    bool audio() const;
    bool image() const;
    bool message() const;
    bool multipart() const;
    bool text() const;
    bool video() const;
    bool xtoken() const;
    bool json() const;
    bool xml() const;
    std::string encoding() const;

    // STATUS
    bool ok() const;
    bool unauthorized() const;
    Method redirect(Method method) const;
    bool permanentRedirect() const;
};


// IMPLEMENTATION
// --------------


/** \brief Initializer constructor.
 *
 *  Initializes the response, first parsing the headers and parsing
 *  them based on RFC 2616 [Section 4.4][reference].
 *
 *  If the transfer encoding is set, and not identity, assume the data
 *  is chunked, unless the connection was closed.
 *
 *  [reference] https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4
 */
template <typename Connection, typename>
Response::Response(Connection &connection)
{
    parseHeaders(connection.headers());
    if (!!transfer && !(transfer & IDENTITY)) {
        // connection has the transfer set and is not identity
        data = connection.chunked();
    } else if (header.find("content-length") != header.end()) {
        data = connection.body(std::stol(header.at("content-length")));
    } else {
        // no content-length or chunked storage, just read
        data = connection.read();
    }
}


}   /* lattice */
