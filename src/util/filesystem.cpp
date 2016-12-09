//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Filesystem and path utilities.
 */

#include "lattice.hpp"


namespace lattice
{
// FUNCTIONS
// ---------


/** \brief Get the directory name for a path.
 */
std::string dirname(const std::string &path)
{
    const size_t index = path.find_last_of("/\\");
    if (index == std::string::npos) {
        return "";
    }
    return path.substr(0, index);
}


/** \brief Get the name without directory.
 */
std::string basename(const std::string &path)
{
    const size_t index = path.find_last_of("/\\");
    if (index == std::string::npos) {
        return path;
    } else if (index == path.size() - 1) {
        return "";
    }
    return path.substr(index + 1);
}


/** \brief Split the filename and extension.
 */
std::array<std::string, 2> splitext(const std::string &path)
{
    const size_t index = path.find_last_of("./\\");
    if (index == std::string::npos || index == 0 || path[index] != '.') {
        // didn't find a suffix or found separator before suffix
        return {path, ""};
    }

    return {path.substr(0, index), path.substr(index+1)};
}


}   /* lattice */
