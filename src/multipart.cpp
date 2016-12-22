//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Multipart requests (file uploads).
 */

#include "lattice.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>


namespace lattice
{
namespace detail
{
// CONSTANTS
// ---------

/** \brief Lookup table for common application types.
 */
std::unordered_map<std::string, std::string> CONTENT_TYPES = {
    // TEXT
    {"css", "text/css"},
    {"csv", "text/csv"},
    {"html", "text/html"},
    {"md", "text/markdown"},
    {"tbt", "text/tab-separated-values"},
    {"txt", "text/plain"},
    {"tsv", "text/tab-separated-values"},
    {"vcard", "text/vcard"},

    // APPLICATION
    {"gz", "application/gzip"},
    {"gzip", "application/gzip"},
    {"doc", "application/msword"},
    {"http", "application/http"},
    {"js", "application/javascript"},
    {"json", "application/json"},
    {"jsn",  "application/json"},
    {"mp4",  "application/mp4"},
    {"mpeg4",  "application/mpeg4"},
    {"ogg",  "application/ogg"},
    {"pdf",  "application/pdf"},
    // PS postscript? powershell? Don't detect.
    {"rtf", "application/rtf"},
    {"sql", "application/sql"},
    {"xml", "application/xml"},

    // MICROSOFT APPLICATION
    {"dot", "application/msword"},
    {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {"dotx", "application/vnd.openxmlformats-officedocument.wordprocessingml.template"},
    {"dotm", "application/vnd.ms-word.document.macroEnabled.12"},
    {"dotc", "application/vnd.ms-word.template.macroEnabled.12"},
    {"xls", "application/vnd.ms-excel"},
    {".xlt", "application/vnd.ms-excel"},
    {".xla", "application/vnd.ms-excel"},
    {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xltx", "application/vnd.openxmlformats-officedocument.spreadsheetml.template"},
    {".xlsm", "application/vnd.ms-excel.sheet.macroEnabled.12"},
    {".xltm", "application/vnd.ms-excel.template.macroEnabled.12"},
    {".xlam", "application/vnd.ms-excel.addin.macroEnabled.12"},
    {".xlsb", "application/vnd.ms-excel.sheet.binary.macroEnabled.12"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pot", "application/vnd.ms-powerpoint"},
    {".pps", "application/vnd.ms-powerpoint"},
    {".ppa", "application/vnd.ms-powerpoint"},
    {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".potx", "application/vnd.openxmlformats-officedocument.presentationml.template"},
    {".ppsx", "application/vnd.openxmlformats-officedocument.presentationml.slideshow"},
    {".ppam", "application/vnd.ms-powerpoint.addin.macroEnabled.12"},
    {".pptm", "application/vnd.ms-powerpoint.presentation.macroEnabled.12"},
    {".potm", "application/vnd.ms-powerpoint.template.macroEnabled.12"},
    {".ppsm", "application/vnd.ms-powerpoint.slideshow.macroEnabled.12"},
};


// FUNCTIONS
// ---------


/** \brief Read a file on a POSIX system.
 *
 *  POSIX systems require a null terminating byte for filenames, so
 *  all Unicode-supporting POSIX systems typically use UTF8 for
 *  filename encoding. This is easy, since it's our internal
 *  representation.
 */
std::string readNarrow(const std::string &filename)
{
    auto *name = filename.data();
    std::ifstream file(name, std::ios_base::in | std::ios_base::binary);
    std::ostringstream stream;
    stream << file.rdbuf();

    return stream.str();
}



/** \brief Read a file on a Win32 system.
 *
 *  Windows systems support two APIs: a legacy, narrow API, for
 *  backwards compatibility with the local code page, and a
 *  UTF-16 wide API. Although MSVC provides a wide character
 *  overload for std::ifstream, MinGW does not, so the file must
 *  be opened with std::wifstream.
 */
std::string readWide(const std::string &filename)
{
    if (!isUnicode(filename)) {
        // ascii only filename, narrow API works.
        return readNarrow(filename);
    }

    auto utf16 = UTF8_TO_UTF16(filename);
    auto *name = reinterpret_cast<const wchar_t*>(utf16.data());
    std::wifstream file(filename, std::ios_base::in | std::ios_base::binary);
    std::wostringstream stream;
    stream << file.rdbuf();

    std::wstring wide = stream.str();
    const size_t size = wide.size() * sizeof(char) / sizeof(wchar_t);
    std::string string(reinterpret_cast<const char*>(wide.data()), size);

    return UTF16_TO_UTF8(string);
}


/** \brief Detect the content type (if not provided).
 */
std::string detectContentType(const std::string &filename)
{
    std::string suffix = splitext(filename)[1];
    auto it = CONTENT_TYPES.find(suffix);
    if (it != CONTENT_TYPES.end()) {
        return it->second;
    }

    return "";
}


// OBJECTS
// -------


/** \brief Initializer copy constructor.
 */
PartValue::PartValue(const std::string &filename,
        const std::string &contentType):
    filename(filename)
{
    if (contentType.empty()) {
        this->contentType_ = detectContentType(this->filename);
    } else {
        this->contentType_ = contentType;
    }
}


/** \brief Initializer move constructor.
 */
PartValue::PartValue(std::string &&path,
        std::string &&contentType):
    filename(LATTICE_FWD(filename))
{
    if (contentType.empty()) {
        this->contentType_ = detectContentType(this->filename);
    } else {
        this->contentType_ = contentType;
    }
}


/** \brief Get basename for file.
 */
std::string PartValue::basename() const
{
    return ::lattice::basename(filename);
}


/** \brief Get name (without extension).
 */
std::string PartValue::name() const
{
    return splitext(basename())[0];
}


/** \brief Get contentType property for file.
 */
const std::string & PartValue::contentType() const
{
    return contentType_;
}


/** \brief Get multipartition data for upload.
 *
 *  RFC-7231 clearly states, if the content type is unknown, do not
 *  send it.
 */
std::string PartValue::string() const
{
    std::ostringstream stream;
    stream << "Content-Disposition: form-data; "
           << "name=\"" <<  name() << "\"; "
           << "filename=\"" << basename() << "\"\r\n";

    if (!contentType().empty()) {
       stream << "Content-Type: " << contentType() << "\r\n";
    }
    stream << "\r\n";

    return stream.str();
}


/** \brief Get data contents from file.
 */
std::string FileValue::buffer() const
{
    #ifdef _WIN32
        // WIN32 has the wide API for files, using UTF-16
        return readWide(filename);
    #else
        return readNarrow(filename);
    #endif
}


/** \brief Get multipartition data for upload.
 */
std::string FileValue::string() const
{
    std::ostringstream stream;
    stream << PartValue::string() << buffer() << "\r\n";

    return stream.str();
}


/** \brief Initializer copy constructor.
 */
BufferValue::BufferValue(const std::string &filename,
        const std::string &buffer,
        const std::string &contentType):
    PartValue(filename, contentType),
    buffer_(buffer)
{}


/** \brief Initializer move constructor.
 */
BufferValue::BufferValue(std::string &&filename,
        std::string &&buffer,
        std::string &&contentType):
    PartValue(LATTICE_FWD(filename), LATTICE_FWD(contentType)),
    buffer_(LATTICE_FWD(buffer))
{}


/** \brief Get data contents from file.
 */
const std::string & BufferValue::buffer() const
{
    return buffer_;
}


/** \brief Get multipartition data for upload.
 */
std::string BufferValue::string() const
{
    std::ostringstream stream;
    stream << PartValue::string() << buffer() << "\r\n";

    return stream.str();
}

}   /* detail */


/** \brief Get boundary for the multipartition.
 */
const std::string & Multipart::boundary() const
{
    return boundary_;
}


/** \brief Add part to multipart detail.
 */
void Multipart::add(const detail::PartPtr &part)
{
    push_back(part);
}


/** \brief Add part to multipart detail.
 */
void Multipart::add(detail::PartPtr &&part)
{
    emplace_back(LATTICE_FWD(part));
}


/** \brief Get bytes for multipart upload.
 */
std::string Multipart::string() const
{
    std::ostringstream stream;
    for (const auto &item: *this) {
        stream << "--" << boundary() << "\r\n" << item->string();
    }

    // if any elements were written, write a trailing separator.
    if (*this) {
        stream << "--" << boundary() << "--\r\n";
    }

    return stream.str();
}


/** \brief Get header for multipart upload.
 */
std::string Multipart::header() const
{
    return "multipart/form-data; boundary=" + boundary();
}


/** \brief Check if data left to return.
 */
Multipart::operator bool() const
{
    return !empty();
}


}   /* lattice */
