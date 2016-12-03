#  :copyright: (c) 2016 The Regents of the University of California.
#  :license: MIT, see LICENSE.md for more details.

cmake_minimum_required(VERSION 2.8)
project(Lattice CXX)

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 11)

# OPTIONS
# -------

option(BUILD_EXAMPLES "Build example files" ON)
option(BUILD_STATIC "Build static library" ON)

if(UNIX AND ${BUILD_STATIC})
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# INCLUDES
# --------

include(CheckIncludeFile)
enable_language(C)

if(UNIX)
    CHECK_INCLUDE_FILE(netdb.h HAVE_NETDB_H)
    CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H)
    CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)
    CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
    CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
    set(REQUIRED_HEADERS
        ${HAVE_NETDB_H} OR
        ${HAVE_SYS_SOCKET_H} OR
        ${HAVE_SYS_TIME_H} OR
        ${HAVE_SYS_TYPES_H} OR
        ${HAVE_UNISTD_H}
    )
elseif(WIN32)
endif()

if(NOT REQUIRED_HEADERS)
    message(FATAL_ERROR "Could not find required headers.")
endif()

# LIBRARY
# -------

set(LATTICE_SOURCES
    src/encode.cpp
    src/exception.cpp
    src/nt.cpp
    src/parameter.cpp
    src/session.cpp
    src/timeout.cpp
    src/unix.cpp
    src/url.cpp
)

set(LATTICE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LATTICE_INCLUDE_DIRS ${LATTICE_INCLUDE_DIR})
include_directories(${LATTICE_INCLUDE_DIRS})

if(BUILD_STATIC)
    add_library(Lattice STATIC ${LATTICE_SOURCES})
else()
    add_library(Lattice SHARED ${LATTICE_SOURCES})
endif()
set_target_properties(Lattice PROPERTIES OUTPUT_NAME lattice)
set(LATTICE_LIBRARIES Lattice)

# EXAMPLES
# --------

set(LATTICE_EXAMPLES
    get
)

if(BUILD_EXAMPLES)
    foreach(example ${LATTICE_EXAMPLES})
        set(target "Lattice_${example}")
        add_executable(${target} "example/${example}.cpp")
        set_target_properties(${target} PROPERTIES OUTPUT_NAME ${example})
        target_link_libraries(${target} ${LATTICE_LIBRARIES})
    endforeach(example)
endif()
