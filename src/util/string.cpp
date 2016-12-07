//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief String utilities.
 */

#include "lattice.hpp"

#include <cstring>


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief Check if string starts with substring.
 */
bool startswith(const std::string &string,
    const std::string &substring)
{
    return string.substr(0, substring.size()) == substring;
}


/** \brief Modified getline which trims carriage returns.
 *
 *  Normalizes getline implementations among POSIX and Windows.
 */
std::istream & getline(std::istream &stream,
    std::string &string)
{
    std::getline(stream, string);
    if (!string.empty() && string.back() == '\r') {
        string.erase(string.size() - 1);
    }

    return stream;
}


/** \brief Trim characters from left-end of string.
 */
void ltrim(std::string &string,
    const std::string &characters)
{
    string.erase(0, string.find_first_not_of(characters));
}


/** \brief Trim characters from right-end of string.
 */
void rtrim(std::string &string,
    const std::string &characters)
{
    string.erase(string.find_last_not_of(characters) + 1);
}


/** \brief Trim characters from both ends of string.
 */
void trim(std::string &string,
    const std::string &characters)
{
    ltrim(string, characters);
    rtrim(string, characters);
}


/** \brief Split string by a character delimiter(s).
 *
 *  \param string           String to split
 *  \param delimiters       Character delimiters to split on
 */
std::vector<std::string> split(const std::string &string,
    const char *delimiters)
{
    std::vector<std::string> items;
    const char *begin = string.data();
    const char *end = begin + string.size();
    const char *interval;

    while (true) {
        interval = strpbrk(begin, delimiters);
        if (interval) {
            items.push_back(std::string(begin, interval));
        } else {
            items.push_back(std::string(begin, end));
            break;
        }
        begin = interval+1;
    }

    return items;
}


/** \brief Join string from begin and end iterator pairs.
 */
std::string join(const std::vector<std::string> &items,
    const std::string &delimiter)
{
    // alloc
    size_t size = 0;
    std::for_each(items.begin(), items.end(), [&](const std::string &i) {
        size += i.size() + delimiter.size();
    });

    // create
    std::string joined;
    joined.reserve(size);
    for (auto it = items.begin(); it != items.end(); ++it) {
        joined += *it;
        joined += delimiter;
    }

    // trim end
    if (joined.size() > delimiter.size()) {
        joined.erase(joined.size() - delimiter.size());
    }
    joined.shrink_to_fit();
    return joined;
}


/** \brief Transform the string to lower-case.
 */
void tolower(std::string &string)
{
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}


/** \brief Replace substring in string.
 *
 *  Replace up to `max` instances of `sub` with`repl` in the target
 *  string. By default, max is set to -1, or infinite occurences.
 */
void replace(std::string &string,
    const std::string &sub,
    const std::string &repl,
    int max)
{
    size_t start = 0;

    while(max && (start = string.find(sub, start)) != std::string::npos) {
        string.replace(start, sub.size(), repl);
        start += repl.size();
        --max;
    }
}


/** Escape printable, non-Unicode, non-alphanumeric characters.
 *
 *  Escapes all ASCII non-alphanumeric characters, assuming
 *  UTF-8 source encoding.
 */
std::string escapeUrl(const std::string src)
{
    std::string dst;
    dst.reserve(src.size() * 2 + 1);

    for (const char c: src) {
        // skip wildcard
        if ((c >= 0 && c <= 41) ||      // Null - )
            (c >= 43 && c <= 47) ||     // + - @
            (c >= 58 && c <= 64) ||     // : - @
            (c >= 91 && c <= 96) ||     // [ - `]
            (c >= 123 && c <= 126)) {   // ( - ~
            dst.push_back('\\');
        } else if (c == '*') {
            dst.push_back('.');
        }
        dst.push_back(c);
    }

    // now need to deal with special wildcards
    replace(dst, ".*\\.", ".*\\.?", 1);

    dst.shrink_to_fit();
    return dst;
}


}   /* lattice */
