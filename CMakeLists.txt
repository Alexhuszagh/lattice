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
elseif(MINGW AND ${BUILD_STATIC})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
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
    CHECK_INCLUDE_FILE(winsock2.h HAVE_WINSOCK2)
    CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS)
    CHECK_INCLUDE_FILE(ws2tcpip.h HAVE_WS2TCPIP)
    set(REQUIRED_HEADERS
        ${HAVE_WINSOCK2} OR
        ${HAVE_WINDOWS} OR
        ${HAVE_WS2TCPIP}
    )
endif()

if(NOT REQUIRED_HEADERS)
    message(FATAL_ERROR "Could not find required headers.")
endif()

# THREADING
# ---------

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# LIBRARY
# -------

file(GLOB LATTICE_SOURCES src/*.cpp)
if(UNIX)
    list(APPEND LATTICE_SOURCES src/adapters/posix.cpp)
elseif(WIN32)
    list(APPEND LATTICE_SOURCES src/adapters/windows.cpp)
endif()

# TODO: need to add SSL support
if(OPENSSL_FOUND)
    list(APPEND LATTICE_SOURCES src/adapters/openssl.cpp)
elseif(LIBRESSL_FOUND)
    list(APPEND LATTICE_SOURCES src/adapters/libressl.cpp)
endif()

set(LATTICE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LATTICE_INCLUDE_DIRS ${LATTICE_INCLUDE_DIR})
include_directories(${LATTICE_INCLUDE_DIRS})

if(BUILD_STATIC)
    add_library(Lattice STATIC ${LATTICE_SOURCES})
else()
    add_library(Lattice SHARED ${LATTICE_SOURCES})
endif()

set_target_properties(Lattice PROPERTIES OUTPUT_NAME lattice)
set(LATTICE_LIBRARIES Lattice Threads::Threads)
if(MSVC)
    list(APPEND LATTICE_LIBRARIES ws2_32.lib)
elseif(MINGW)
    list(APPEND LATTICE_LIBRARIES -lws2_32)
endif()

# EXAMPLES
# --------

file(GLOB LATTICE_EXAMPLES example/*.cpp)

if(BUILD_EXAMPLES)
    foreach(source ${LATTICE_EXAMPLES})
        get_filename_component(example ${source} NAME_WE)
        set(target "Lattice_${example}")
        add_executable(${target} "${source}")
        set_target_properties(${target} PROPERTIES OUTPUT_NAME ${example})
        target_link_libraries(${target} ${LATTICE_LIBRARIES})
    endforeach(source)
endif()
