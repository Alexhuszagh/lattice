//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief String utilities.
 */

#include <lattice/util/string.hpp>

#include <cstring>
#include <istream>


namespace lattice
{
// FUNCTIONS
// ---------


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

}   /* lattice */
