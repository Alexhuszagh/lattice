#  :copyright: (c) 2016 The Regents of the University of California.
#  :license: MIT, see LICENSE.md for more details.

cmake_minimum_required(VERSION 2.8)
project(lattice CXX)

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 11)

# OPTIONS
# -------

option(BUILD_EXAMPLES "Build example files" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build unittests (requires GTest)" OFF)
option(WITH_OPENSSL "Build with OpenSSL" OFF)

if(BUILD_STATIC)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /MTd /O0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox")
    endif()
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

# OPENSSL
# -------

if(WITH_OPENSSL)
    find_package(OpenSSL "1.0")
endif()

# THREADING
# ---------

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# SSL
# ---

if(OPENSSL_FOUND)
    add_definitions(-DHAVE_OPENSSL)
endif()
set(HAVE_SSL ${OPENSSL_FOUND})

# LIBRARY
# -------

file(GLOB LATTICE_SOURCES src/*.cpp)
file(GLOB LATTICE_CRYPTO_SOURCES src/crypto/*.cpp)
file(GLOB LATTICE_ENCODING_SOURCES src/encoding/*.cpp)
file(GLOB LATTICE_UTIL_SOURCES src/util/*.cpp)
list(APPEND LATTICE_SOURCES
    ${LATTICE_CRYPTO_SOURCES}
    ${LATTICE_ENCODING_SOURCES}
    ${LATTICE_UTIL_SOURCES}
)

if(UNIX)
    list(APPEND LATTICE_SOURCES src/adapters/posix.cpp)
elseif(WIN32)
    list(APPEND LATTICE_SOURCES src/adapters/windows.cpp)
endif()

set(LATTICE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LATTICE_INCLUDE_DIRS ${LATTICE_INCLUDE_DIR})
include_directories(${LATTICE_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

if(BUILD_STATIC)
    add_library(Lattice STATIC ${LATTICE_SOURCES})
else()
    add_library(Lattice SHARED ${LATTICE_SOURCES})
endif()

set_target_properties(Lattice PROPERTIES OUTPUT_NAME lattice)
set(LATTICE_LIBRARIES Lattice Threads::Threads ${OPENSSL_LIBRARIES})
if(MSVC)
    list(APPEND LATTICE_LIBRARIES ws2_32.lib crypt32.lib)
elseif(MINGW)
    list(APPEND LATTICE_LIBRARIES -lws2_32 -lcrypt32)
endif()

# EXAMPLES
# --------

file(GLOB LATTICE_EXAMPLES example/*.cpp)
file(GLOB LATTICE_SSL_EXAMPLES example/ssl/*.cpp)
file(GLOB LATTICE_MULTIPART_EXAMPLES example/multipart/*)

# SSL
if(HAVE_SSL AND BUILD_EXAMPLES)
    list(APPEND LATTICE_EXAMPLES ${LATTICE_SSL_EXAMPLES})
    file(COPY example/ssl/client.crt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

# MULTIPART
if(BUILD_EXAMPLES)
    file(COPY ${LATTICE_MULTIPART_EXAMPLES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()


if(BUILD_EXAMPLES)
    foreach(source ${LATTICE_EXAMPLES})
        get_filename_component(example ${source} NAME_WE)
        set(target "Lattice_${example}")
        add_executable(${target} "${source}")
        set_target_properties(${target} PROPERTIES OUTPUT_NAME ${example})
        target_link_libraries(${target} ${LATTICE_LIBRARIES})
    endforeach(source)
endif()

# TESTS
# -----

file(GLOB LATTICE_TESTS test/*.cpp)
file(GLOB LATTICE_CRYPTO_TESTS test/crypto/*.cpp)
file(GLOB LATTICE_ENCODING_TESTS test/encoding/*.cpp)
list(APPEND LATTICE_TESTS
    ${LATTICE_CRYPTO_TESTS}
    ${LATTICE_ENCODING_TESTS}
)

if(BUILD_TESTS)
    add_subdirectory(googletest)
    add_executable(LatticeTests ${LATTICE_TESTS})
    target_link_libraries(LatticeTests
        gtest
        gtest_main
        ${LATTICE_LIBRARIES}
    )

     add_custom_target(check_lattice
        COMMAND $<TARGET_FILE:LatticeTests>
        DEPENDS LatticeTests
    )

endif()
