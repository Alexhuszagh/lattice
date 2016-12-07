lattice
=======

A light-weight, dependency-free, cross-platform library for client-side network requests.

**Table of Contents**

- [Motivation](#motivation)
- [Features](#features)
- [Lattice Is Not...](#lattice-is-not...)
- [Dependencies](#dependencies)
- [Building](#building)
- [TODO](#todo)
- [Contributors](#contributors)
- [Contributor Guidelines](#contributors-guidelines)
- [Credits](#credits)
- [License](#license)

## Motivation

Lattice aims to be a light-weight, adaptable, and easily embeddable client-side networking library, simplifying integration of basic networking into projects.

## Features

- Custom Headers
- URL-encoded parameters
- Cookies
- DNS caching
- Redirection following
- GET/HEAD/POST/PATCH/PUT/OPTIONS/DELETE/TRACE methods 
- Response type detection
- Pseudo-asynchronous requests
- International domain names and Unicode parameters

**HTTPS Only**

- SSL/TLS adapters for sockets
- Domain Name and certification validation

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

## TODO

- Multipart
- Body
- Options
- Payload

## Contributors

- Alex Huszagh

Lattice uses code directly adapted from [CPR](https://github.com/whoshuu/cpr) and therefore credits those [authors](AUTHORS), and can be considered a fork in some form. Contributors to CPR are not affiliated with Lattice. 

The OpenSSL adapter was reimplemented from libssl annotations in the cURL project, and therefore is a derivative work.

## Contributor Guidelines

All useful pull requests will be merged, provided that they do not add external dependencies and follow these guidelines.

1. Preprocessor macros should be used sparingly.
2. Code syntax should not depend on the preprocess.
3. Your code must be readable.

## License

MIT, see [license](LICENSE.md).
