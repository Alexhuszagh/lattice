lattice
=======

Lattice is a C++11, cross-platform HTTP library, inspired by Python's [Requests](https://github.com/kennethreitz/requests) and [C++ Requests](https://github.com/whoshuu/cpr.

**Table of Contents**

- [Motivation](#motivation)
- [Features](#features)
- [Design](#design)
- [Dependencies](#dependencies)
- [Building](#building)
- [Portability](#portability)
- [Documentation](#documentation)
- [Planned Features](#planned-features)
- [Contributors](#contributors)
- [Contributor Guidelines](#contributors-guidelines)
- [Credits](#credits)
- [License](#license)

## Motivation

Lattice is a modern, self-contained, thread-safe HTTP library for C++, abstracting away the low-level interfaces. No more hand-encoded URL parameters or international domain names.

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

Compare this to the same code using [cURL](https://gist.github.com/Alexhuszagh/e3c534f7cf317c5f88102f1307113622). No external libraries, no defines, no unresolved symbols. No hassle.

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

## Design

Lattice has four main components: adapters, connections, requests and responses. 

1. Adapters wrap native sockets or SSL connections into an interface with 4 core methods: `open`, `close`, `write`, and `read`. 
2. Connections wrap specific adapters, and add shared methods like DNS lookup. 
3. Requests format data for HTTP requests, with user-friendly methods like `setAuth` or `setMultiPart`.
4. Responses parse data sent from the server.

## Dependencies

- C++11 Compiler
- CMake
- Native threads (POSIX or Win32)
- (Optional) OpenSSL

SSL/TLS support is header-only, the library itself has no SSL dependencies.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/lattice.git
cd lattice/build
cmake .. -DBUILD_EXAMPLES=ON    # "-DWITH_OPENSSL=ON" for SSL examples
make -j 5                       # "msbuild lattice.sln" for MSVC
```

## Portability

Lattice has been tested with the following compilers and operating systems:

- 64-bit Linux with Clang 3.8.2
- 64-bit Linux with GCC 5.4.0
- 32/64-bit Windows with MinGW 5.3.0 (MXE, MinGW, and MSYS2) 
- 32/64-bit Windows with Visual Studio 14 2015

## Documentation

Coming soon, for now, see the the [examples](/example) for how to use lattice.

## Planned Features

- OAuth
- Asynchronous requests

## Contributors

- Alex Huszagh

Lattice is a fork of [C++ Requests](https://github.com/whoshuu/cpr) with a different backend, and credits those contributors in [authors](AUTHORS). 

## Contributor Guidelines

All useful pull requests will be merged, provided that they do not add external dependencies and follow these guidelines.

1. Preprocessor macros should be used sparingly.
2. Code syntax should not depend on the preprocessor.
3. Your code must be readable.

## License

MIT, see [license](LICENSE.md).
