//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :license: Boost, see licenses/Boost.txt for more details.
/**
 *  \addtogroup Lattice
 *  \brief Parese request response.
 */

#include "lattice/response.hpp"
#include "lattice/util/string.hpp"

#include <getline.h>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>


namespace lattice
{
// OBJECTS
// -------


/** \brief Parse status code from HTTP response line.
 *
 *  This is valid according to RFC 2616 [Section 6.1][reference].
 *
 *  [reference] http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6
 */
void Response::parseCode(const std::string &line)
{
    const size_t start = 9;
    size_t end = start;
    while (end < line.size() && isdigit(line[end++]));

    // no code returned, give an ok status (non-standard conforming)
    const int number = start == end ? 200 : std::stoi(line.substr(start, end));
    status_ = static_cast<StatusCode>(number);
}


/** \brief Parse cookie from response line.
 */
void Response::parseCookie(const std::string &string)
{
    size_t delimiter = string.find_first_of("=");
    size_t end = string.find_first_of(";", delimiter);
    size_t count = end - delimiter;
    cookies_.emplace(string.substr(0, delimiter), string.substr(delimiter, count));
}


/** \brief Parse transfer encoding from data.
 */
void Response::parseTransferEncoding(std::string &string)
{
    ::lattice::tolower(string);
    auto encodings = split(string, ",");
    for (auto &encoding: encodings) {
        trim(encoding);
        if (encoding == "chunked") {
            transfer |= CHUNKED;
        } else if (encoding == "compress") {
            transfer |= COMPRESS;
        } else if (encoding == "deflate") {
            transfer |= DEFLATE;
        } else if (encoding == "gzip") {
            transfer |= GZIP;
        } else if (encoding == "identity") {
            transfer |= IDENTITY;
        }
    }
}


/** \brief Parse the "Content-Type" header.
 */
void Response::parseContentType(std::string &string)
{
    ::lattice::tolower(string);
    auto values = split(string, ";");
    if (values.size()) {
        // get type, subtype
        auto &front = values.front();
        parseType(front);
        std::get<1>(mime) = front.substr(front.find("/") + 1);

        // get parameters
        for (auto it = values.begin() + 1; it != values.end(); ++it) {
            if (startswith(*it, "charset")) {
                charset = it->substr(8);
            } else {
                // generalized parameter
                headers_["content-type"] += *it + ";";
            }
        }
    }
}


/** \brief Parse the main data type from "Content-Type".
 */
void Response::parseType(const std::string &string)
{
    switch (string.front()) {
        case 'a': {
            std::get<0>(mime) = string[1] == 'p' ? APPLICATION : AUDIO;
            break;
        }
        case 'i': {
            std::get<0>(mime) = IMAGE;
            break;
        }
        case 'm': {
            std::get<0>(mime) = string[1] == 'e' ? MESSAGE : MULTIPART;
            break;
        }
        case 't': {
            std::get<0>(mime) = TEXT;
            break;
        }
        case 'v': {
            std::get<0>(mime) = VIDEO;
            break;
        }
        case 'x': {
            // custom token, must store
            std::get<0>(mime) = XTOKEN;
            headers_["x-token"] += string.substr(2, string.find("/") - 2);
            break;
        }
        default:
            break;
    }
}


/** \brief Parse header from the HTTP response.
 *
 *  The parsed code must, for HTTP/1.1 start
 */
void Response::parseHeader(const std::string &line)
{
    if (startswith(line, "HTTP/")) {
        // this is valid
        parseCode(line);
    } else {
        // common headers
        size_t colon = line.find_first_of(":");
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon+1);
        ::lattice::tolower(key);
        trim(value);

        if (key == "set-cookie") {
            parseCookie(value);
        } else if (key == "transfer-encoding") {
            parseTransferEncoding(value);
        } else if (key == "content-type") {
            parseContentType(value);
        } else if (key == "authorization") {
            // TODO
        } else {
            // fallthrough
            headers_.emplace(key, value);
        }
    }
}


/** \brief Extract headers from the HTTP response.
 */
void Response::parseHeaders(const std::string &lines)
{
    std::istringstream stream(lines);
    std::string line;
    while (getline(stream, line)) {
        if (!line.empty()) {
            // data() is used to fix a compiler optimization bug in MSYS2
            parseHeader(line.data());
        }
    }
}


/** \brief Get status code from response.
 */
const int Response::status() const
{
    return static_cast<int>(status_);
}


/** \brief Get response text body.
 */
const std::string & Response::body() const
{
    return body_;
}


/** \brief Get response headers.
 */
const Header & Response::headers() const
{
    return headers_;
}


/** \brief Get response cookies.
 */
const Cookies & Response::cookies() const
{
    return cookies_;
}


/** \brief Get transfer encoding.
 */
TransferEncoding Response::transferEncoding() const
{
    return transfer;
}


/** \brief Get content encoding.
 */
std::string Response::contentEncoding() const
{
    auto it = headers().find("content-encoding");
    if (it != headers().end()) {
        return it->second;
    }

    return "";
}


/** \brief Data type is compressed in some format.
 *
 *  \warning This is necessarily slow, since the RFC 2616 can accept
 *  many different compression formats different content-encodings,
 *  transfer-encodings, or application types.
 *
 */
bool Response::compressed() const
{
    // fast check if the transer-encoding was set
    auto transit = (
        (transfer & COMPRESS) |
        (transfer & DEFLATE) |
        (transfer & GZIP)
    );
    if (!!transit) {
        return true;
    }

    // check if the content-encoding was set and a transformation applied
    auto it = headers().find("content-encoding");
    if (it != headers().end() && it->second != "identity") {
        return true;
    }

    // check if the mime type is a known compressed format
    auto &format = std::get<1>(mime);
    return (
        format == "x-compress" ||
        format == "x-gzip" ||
        format == "x-bzip" ||
        format == "x-bzip2" ||
        format == "x-7z-compressed" ||
        format == "x-ace-compressed" ||
        format == "x-rar-compressed"
    );
}


/** \brief Content uses `compress`.
 *
 *  \warning Compress is deprecated and should not appear in
 *  real applications.
 */
bool Response::compress() const
{
    if (!!(transfer & COMPRESS) || std::get<1>(mime) == "x-compress") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "compress"));
}


/** \brief Content is Zlib's deflate.
 */
bool Response::deflate() const
{
    if (!!(transfer & DEFLATE)) {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "deflate"));
}


/** \brief Content is W3's EXI.
 */
bool Response::exi() const
{
    if (std::get<1>(mime) == "exi") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "exi"));
}


/** \brief Content-type is GZIP-compressed.
 *
 *  Return whether the "Transfer-Encoding", or "Content-Encoding" is
 *  set the gzip.
 */
bool Response::gzip() const
{
    if (!!(transfer & GZIP) || std::get<1>(mime) == "x-gzip") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "gzip"));
}


/** \brief Content is a GZIP-compressed pack200 file.
 */
bool Response::pack200Gzip() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "pack200-gzip"));
}


/** \brief Content is Brotli-compressed.
 */
bool Response::br() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "br"));
}


/** \brief Content is a bzip archive.
 */
bool Response::bzip() const
{
    return std::get<1>(mime) == "x-bzip";
}


/** \brief Content is a bzip2 archive.
 */
bool Response::bzip2() const
{
    if (std::get<1>(mime) == "x-bzip2") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "bzip2"));
}


/** \brief Content is lzma compressed.
 */
bool Response::lzma() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "lzma"));
}


/** \brief Content is peerdist compressed.
 */
bool Response::peerdist() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "peerdist"));
}


/** \brief Content is xpress compressed.
 */
bool Response::xpress() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "xpress"));
}


/** \brief Content is xz compressed.
 */
bool Response::xz() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "xz"));
}


/** \brief Content is a _7z compressed archive.
 */
bool Response::_7z() const
{
    return std::get<1>(mime) == "x-7z-compressed";
}


/** \brief Content is an ACE compressed archive.
 */
bool Response::ace() const
{
    return std::get<1>(mime) == "x-ace-compressed";
}


/** \brief Content is an RAR compressed archive.
 */
bool Response::rar() const
{
    return std::get<1>(mime) == "x-rar-compressed";
}


/** \brief Content-type is text.
 */
auto Response::type() const
    -> const MimeType &
{
    return mime;
}


/** \brief Content-type is application.
 */
bool Response::application() const
{
    return std::get<0>(mime) == APPLICATION;
}


/** \brief Content-type is audio.
 */
bool Response::audio() const
{
    return std::get<0>(mime) == AUDIO;
}


/** \brief Content-type is image.
 */
bool Response::image() const
{
    return std::get<0>(mime) == IMAGE;
}


/** \brief Content-type is message.
 */
bool Response::message() const
{
    return std::get<0>(mime) == MESSAGE;
}


/** \brief Content-type is multipart.
 */
bool Response::multipart() const
{
    return std::get<0>(mime) == MULTIPART;
}


/** \brief Content-type is text.
 */
bool Response::text() const
{
    return std::get<0>(mime) == TEXT;
}

/** \brief Content-type is video.
 */
bool Response::video() const
{
    return std::get<0>(mime) == VIDEO;
}


/** \brief Content-type is X-token.
 */
bool Response::xtoken() const
{
    return std::get<0>(mime) == XTOKEN;
}


/** \brief Content-type is JSON.
 */
bool Response::json() const
{
    return application() && std::get<1>(mime) == "json";
}


/** \brief Content-type is XML.
 */
bool Response::xml() const
{
    return application() && std::get<1>(mime) == "xml";
}


/** \brief Get encoding from response.
 */
std::string Response::encoding() const
{
    return charset;
}


/** \brief Check if response is ok.
 */
bool Response::ok() const
{
    return status_ == StatusCode::OK;
}


/** \brief Check if response is unathorized.
 */
bool Response::unauthorized() const
{
    return status_ == StatusCode::UNAUTHORIZED;
}


/** \brief Check if response is a redirect.
 *
 *  Found should **not** automatically redirect except for GET/HEAD
 *  requests.
 */
Method Response::redirect(Method method) const
{
    switch (status_) {
        case StatusCode::MOVED_PERMANENTLY:
            /* fallthrough */
        case StatusCode::TEMPORARY_REDIRECT:
            /* fallthrough */
        case StatusCode::PERMANENT_REDIRECT:
            /* fallthrough */
        case StatusCode::FOUND:
            if (method == GET || method == HEAD) {
                return method;
            }
            return STOP;
        case StatusCode::SEE_OTHER:
            return GET;
            /* fallthrough */
        default:
            return STOP;
    }
}


/** \brief Check if response is a permanent redirect.
 */
bool Response::permanentRedirect() const
{
    return status_ == StatusCode::PERMANENT_REDIRECT;
}


/** \brief Check if any data has been set.
 */
Response::operator bool() const
{
    return (
        status() ||
        headers() ||
        cookies() ||
        FROM_ENUM(transfer) ||
        !charset.empty() ||
        !body().empty()
    );
}


}   /* lattice */

