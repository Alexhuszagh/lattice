//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief String utilities.
 */

#include "lattice/util/string.hpp"

#include <cstring>
#include <istream>


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
    std::vector<std::string> data;
    const char *begin = string.data();
    const char *end = begin + string.size();
    const char *interval;

    while (true) {
        interval = strpbrk(begin, delimiters);
        if (interval) {
            data.push_back(std::string(begin, interval));
        } else {
            data.push_back(std::string(begin, end));
            break;
        }
        begin = interval+1;
    }

    return data;
}


/** \brief Split string by delimiter(s).
 *
 *  \param string           String to split
 *  \param quote            Quoting character
 *  \param escape           Escape character
 *  \param delimiters       Character delimiter
 */
std::vector<std::string> safesplit(const std::string &string,
    const char delimiter,
    const char quote,
    const char escape)
{
    std::vector<std::string> data;

    bool isQuoted = false;
    bool isEscaped = false;
    char *word = new char[string.size()];
    int j = 0;
    int k = 0;

    for (const char c : string) {
        if (isEscaped) {            // escape letter and undo escaping
            isEscaped = false;
            word[j] = c;
            j++;
        }  else if (c == escape) {
            isEscaped = true;       // escape next character
        } else if (c == quote) {
            isQuoted ^= true;       // opening/ending quote
        } else if (isQuoted) {
            word[j] = c;            // append quoted character to word
            j++;
        } else if (c == delimiter) {
            data.emplace_back(std::string(word, j));
            memset(word, 0, j);     // write null values to line
            j = 0;
            k++;
        } else {
            word[j] = c;            // append unquoted word
            j++;
        }
    }

    data.emplace_back(std::string(word, j));
    delete[] word;

    return data;
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
