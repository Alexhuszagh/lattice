//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Hex representation utilities.
 */

#include <cstdio>
#include <string>


namespace lattice
{
// FUNCTIONS
// ---------

/** \brief Get number of digits from number.
 */
template <class T>
T digits(T number)
{
    T digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}


/** \brief Get the snprintf formatting string.
 *
 *  \code
 *      formatter(8) -> "%08x"
 */
template <typename T>
std::string formatter(const T width)
{
    const T length = digits(width);
    const T size = length + 4;
    char *buffer = new char[size];

    snprintf(buffer, size, "%%0%dx", width);
    std::string output(buffer, size - 1);
    delete[] buffer;

    return output;
}


/** \brief Convert source string to hex representation.
 *
 *  The width, as well as the step interval is controlled by
 */
template <typename T>
std::string hexlify(const std::string &string)
{
    // get formatting parameters
    constexpr size_t size = sizeof(T);
    constexpr int width = 2 * size;
    const size_t length = 2 * string.size();
    auto format = formatter(width);

    // get iterators
    auto *begin = reinterpret_cast<const T*>(string.data());
    auto end = begin + (length / width);

    // create buffer
    char *buffer = new char[length + 1];
    char *ptr = buffer;
    while (begin < end) {
        snprintf(ptr, width+1, format.data(), *begin++);
        ptr += width;
    }

    // create STL container and return
    std::string output(buffer, length);
    delete[] buffer;

    return output;
}

// API
// ---

template <typename T = uint8_t>
std::string HEX(const std::string &string)
{
    return hexlify<T>(string);
}

}   /* lattice */

