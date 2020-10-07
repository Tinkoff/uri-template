# URI-template

[![Language C++](https://img.shields.io/badge/language-c++-blue.svg)](https://isocpp.org)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

TODO

## Why

TODO

## How to use

TODO

## How to build

This library supposed to be somewhat multi-platform, however, it was tested and mainly used on ubuntu and macOS. Therefore build instructions are given for only these OSes.

### Ubuntu dependencies

```bash
sudo apt update
sudo apt install build-essential cmake
```

### macOS dependencies

```bash
brew install cmake pkg-config
```

### Build

Prefer [out-of-source](https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#what-is-an-out-of-source-build) building:

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
make install
```

You may also test it with:
```bash
make test
```
Or install (sudo may be required):
```bash
make install
```

### Cmake options

* **CMAKE_BUILD_TYPE** - [build type](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). `RelWithDebInfo` by default.
* **BUILD_SHARED_LIBS** - [build shared or static library](https://cmake.org/cmake/help/v3.0/variable/BUILD_SHARED_LIBS.html). `ON` by default.
* **BUILD_TESTING** - [build tests or not](https://cmake.org/cmake/help/latest/module/CTest.html). `ON` by default.
* **URITEMPLATE_BUNDLED_MODE** - if the library is being built as a part of another project. If this options is set then *BUILD_SHARED_LIBS* forces to be OFF. Defined by `"${PROJECT_SOURCE_DIR}" == "${CMAKE_SOURCE_DIR}"` by default.

## License

Developed at **Tinkoff.ru** in 2020.\
Distibuted under **Apache License 2.0** [LICENSE](./LICENSE). You may also obtain this license at https://www.apache.org/licenses/LICENSE-2.0.

## Contacts

Author - i.s.vovk@tinkoff.ru\
Current maintainer - i.s.vovk@tinkoff.ru
