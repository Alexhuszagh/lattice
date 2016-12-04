//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Generic lattice utilities.
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
 *
 *  \param items            Vector of items to join
 *  \param delimiter        Iterator at last element to add
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


}   /* lattice */
