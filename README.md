lattice
=======

Lattice is a modern C++, cross-platform library for client-side network requests.

**Table of Contents**

- [Motivation](#motivation)
- [Features](#features)
- [Lattice Is Not...](#lattice-is-not...)
- [Dependencies](#dependencies)
- [Building](#building)
- [Documentation](#documentation)
- [Design](#design)
- [Planned Features](#planned-features)
- [Contributors](#contributors)
- [Contributor Guidelines](#contributors-guidelines)
- [Credits](#credits)
- [License](#license)

## Motivation

Requests in modern C++ can simple and expressive, abstracting low-level interfaces from the user. A get request, using lattice, is as easy as:

**1. Code**
```cpp
#include <lattice.hpp>
#include <iostream>


int main(int argc, char *argv[])
{
    lattice::Parameters parameters = {
        {"param1", "value1"},
        {"param2", "value2"},
    };
    lattice::Url url = {"http://httpbin.org/get"};
    auto response = lattice::Get(url, parameters);

    std::cout << response.body() << std::endl;
}
```

**2. Build & Run**

```bash
$ g++ get.cpp -o get -std=c++11 liblattice.a
$ ./get
{
  "args": {
    "param1": "value1", 
    "param2": "value2"
  }, 
  "headers": {
    "Accept": "*/*", 
    "Cookie": "fake=fake_value", 
    "Host": "httpbin.org", 
    "User-Agent": "lattice/0.1.0"
  }, 
  "origin": "XXX.X.X.X", 
  "url": "http://httpbin.org/get?param1=value1&param2=value2"
}
```

No dependencies, no defines, no unresolved symbols. 

## Features

- Custom Headers
- Parameters
- Cookies
- DNS caching
- Redirections
- Content-Type detection
- Pooled requests
- International domain names
- Unicode Support (UTF8, UTF16, UTF32)
- Auth (Basic, Digest)
- Proxies (Beta)

**HTTPS Only**

- SSL/TLS adapters for sockets
- Domain validation
- Certificate validation

## Lattice is Not...

All batteries included. Lattice will identify compressed data streams, JSON, XML and other data types, but it will not decode or parse them. It is lightweight, making minimal assumptions of the requirements of the user.

## Dependencies

- C++11 Compiler
- CMake
- Native threads (POSIX or Win32)
- (Optional) OpenSSL

SSL/TLS support is header-only, the library itself has no SSL dependencies.

## Building

Simply clone, configure with CMake, and build.

```
git clone https://github.com/Alexhuszagh/lattice.git
cd lattice/build
cmake .. -DBUILD_EXAMPLES=ON    # "-DWITH_OPENSSL=ON" for SSL examples
make -j 5                       # "msbuild Lattice.sln" for MSVC
```

## Documentation

Coming soon, for now, see the the [examples](/example) for how to use lattice.

## Design

Lattice uses template-driven socket adapters as the backend, which abstract the socket connection into a few core methods: `open`, `close`, `write`, and `read`, with optional extensions, making lattice easy to extend.

For example, to write a UDP socket adapter for streaming requests, is as simple as:

```cpp
class UdpSocketAdapter
{
    int sock = -1;

    bool open(const addrinfo &info,
        const std::string &/*host*/);
    void close();
    size_t write(const char *buf,
        size_t len);
    size_t read(char *buf,
        size_t count);
};


/** Open connection to socket, return if the connection was 
 *  successfully established 
 */
void UdpSocketAdapter::open(const addrinfo &info,
        const std::string &/*host*/)
{
    sock = socket(info.ai_family, SOCK_DGRAM, info.ai_protocol);
    if (sock < 0) {
        return false;
    }
    if (connect(sock, info.ai_addr, info.ai_addrlen) >= 0) {
        return true;
    }

    ::close(sock);
    return false;
}


/** Close open connection.
 */
void UdpSocketAdapter::close()
{
    if (sock >= 0) {
        ::close(sock);
        sock = -1;
    }
}

/** Send data to socket.
 */
size_t UdpSocketAdapter::write(const char *buf,
    size_t len)
{
    return ::send(sock, buf, len, 0);
}


/** Read data from socket.
 */
size_t UdpSocketAdapter::read(char *buf,
    size_t count)
{
    return ::recv(sock, buf, count, 0);
}
```

## Planned Features

- OAuth
- Asynchronous requests

## Contributors

- Alex Huszagh

Lattice uses code directly adapted from [CPR](https://github.com/whoshuu/cpr) and therefore credits those [authors](AUTHORS), and can be considered a fork in some form. Contributors to CPR are not affiliated with Lattice. 

The OpenSSL adapter was reimplemented from libssl annotations in the cURL project, and therefore is  a derivative work.

## Contributor Guidelines

All useful pull requests will be merged, provided that they do not add external dependencies and follow these guidelines.

1. Preprocessor macros should be used sparingly.
2. Code syntax should not depend on the preprocessor.
3. Your code must be readable.

## License

MIT, see [license](LICENSE.md).
