//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Generic lattice utilities.
 */

#pragma once

#include <algorithm>
#include <istream>
#include <string>
#include <type_traits>
#include <vector>


namespace lattice
{
// MACROS
// ------

#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)


/** \brief Convert enum to underlying type.
 */
template <typename Enum>
auto FROM_ENUM(const Enum value)
    -> typename std::underlying_type<Enum>::type
{
    return static_cast<typename std::underlying_type<Enum>::type>(value);
}

/** \brief Macro to define enum operators.
 *
 *  Supports `&`, `&=`, `|`, `|=`, `^`, `^=`, `~`, and bool conversion.
 */
#define FLAGS(Enum)                                                     \
    /*  \brief Bitwise or operator. */                                  \
    inline Enum operator|(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) | FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise or assignment operator. */                       \
    inline Enum & operator|=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) | FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and operator. */                                 \
    inline Enum operator&(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) & FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and assignment operator. */                      \
    inline Enum & operator&=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) & FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor operator. */                                 \
    inline Enum operator^(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) ^ FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor assignment operator. */                      \
    inline Enum & operator^=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) ^ FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise negation operator. */                            \
    inline Enum operator~(const Enum value)                             \
    {                                                                   \
        return static_cast<Enum>(~FROM_ENUM(value));                    \
    }                                                                   \
                                                                        \
    /*  \brief Negation operator. */                                    \
    inline bool operator!(const Enum value)                             \
    {                                                                   \
        return !bool(FROM_ENUM(value));                                 \
    }


// FUNCTIONS
// ---------

/** \brief Check if string starts with substring.
 */
bool startswith(const std::string &string,
    const std::string &substring);


/** \brief Modified getline which trims carriage returns.
 */
std::istream & getline(std::istream &stream,
    std::string &string);


/** \brief Trim characters from left-end of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
inline void ltrim(std::string &string,
    const std::string &characters = " \n\r\t")
{
    string.erase(0, string.find_first_not_of(characters));
}


/** \brief Trim characters from right-end of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
inline void rtrim(std::string &string,
    const std::string &characters = " \n\r\t")
{
    string.erase(string.find_last_not_of(characters) + 1);
}


/** \brief Trim characters from both ends of string.
 *
 *  \param string               String to trim
 *  \param characters           Characters to trim string by
 */
inline void trim(std::string &string,
    const std::string &characters = " \n\r\t")
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
    const char *delimiters);


/** \brief Join string from begin and end iterator pairs.
 *
 *  \param items            Vector of items to join
 *  \param delimiter        Iterator at last element to add
 */
std::string join(const std::vector<std::string> &items,
    const std::string &delimiter);


/** \brief Transform the string to lower-case.
 */
inline void tolower(std::string &string)
{
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}


}   /* lattice */
