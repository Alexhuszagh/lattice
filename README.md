lattice
=======

A light-weight, dependency-free, cross-platform library for client-side network requests.

**Table of Contents**

- [Motivation](#motivation)
- [Features](#features)
- [Lattice Is Not...](#lattice-is-not...)
- [Dependencies](#dependencies)
- [Contributors](#contributors)
- [Contributor Guidelines](#contributor-guidelines)
- [License](#license)

## Motivation

Lattice aims to be a light-weight, adaptable, and easily embeddable client-side networking library, simplifying integration of basic networking into projects.

## Features

- Custom Headers
- URL-encoded parameters
- GET/POST/DELETE/TRACE methods 
- Response type detection
- Pseudo-asynchronous requests

## Lattice is Not...

All batteries included. Lattice will identify compressed data streams, JSON, XML and other data types, but it will not decode or parse them. It is lightweight, making minimal assumptions of the requirements of the user.

## Dependencies

- C++11 Compiler
- CMake or GNU Make
- POSIX or WIN32 threads

## Contributors

- Alex Huszagh

Lattice uses code directly adapted from [CPR](https://github.com/whoshuu/cpr) and therefore credits those [authors](AUTHORS), and can be considered a fork in some form. Contributors to CPR are not affiliated with Lattice. 

## Contributor Guidelines

Contributors should follow the same coding conventions present in the source code, and must agree to either an MIT or Boost license.

## License

MIT, see [license](LICENSE.md).
