# URI-template

[![Language C++](https://img.shields.io/badge/language-c++-blue.svg)](https://isocpp.org)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

This library implements [URI Template](https://tools.ietf.org/html/rfc6570) with full support up to Level 4 providing **expansion** and **match** capabilities. It requires c++17 compiler support and has no dependencies.

## What?

URI templates are a convenient way to describe a range of possible URI by making some parts of it variable. For example, a variety of resources:
* `http://example.com/~fred/`
* `http://example.com/~mark/`
* `http://example.com/~admin/`
* `http://example.com/~guest/`
* `http://example.com/~elephant/`

Could be described by simple template – `http://example.com/~{username}/`.

Or, resources:
* `http://example.com/search?q=cat&lang=en`
* `http://example.com/search?q=chien&lang=fr`

Could be described by – `http://example.com/search{?q,lang}`.

A template is transformed into a URI by replacing each delimited expression with its value as defined by expansion rules and the values of variables.

URI Templates can also be used in reverse for the purpose of variable matching: comparing the template to a fully formed URI in order to extract the variables. It only works well if the template expressions are delimited by the beginning or end of the URI or by characters that cannot be part of the expansion, otherwise, some ambiguity is present. For example, a template `http://example.com/{foo}{bar}` matches `http://example.com/foobar`, but is is impossible to distinguish if:
* `foo='foobar'` and `bar` is undefined; or
* `foo` is undefined and `bar='foobar'`

Although, if you only interested if a template matches some URI with at least one possible set of values and does not care about values itself, then it is ok.

There is more to it. For better understanding please refer to [RFC 6570](https://tools.ietf.org/html/rfc6570).

## Quickstart

URI Templates are presented as instances of `URI::Template::Template` class. It is basically a vector of parts, which can be either `URI::Template::Literal` or `URI::Template::Expression`. To make one you can use `URI::Template::ParseTemplate()` function or construct it by hand using:
* `URI::Template::Operator`
* `URI::Template::Variable`
* `URI::Template::Modifier`

classes.

From there you can provide values for template variables with `URI::Template::VarValue` objects and expand it, or use `URI::Template::MatchURI()` to test if some URI matches a template, i.e. if it can be expanded from a template with correct values provided.

### Example

Here is basic example how to parse, match and expand URI template:
```c++
#include <uri-template/uri-template.h>
#include <iostream>

int main() {
    const std::string uri = "http://example.com/search?q=cat&lang=en";
    // Parse the template
    const URI::Template::Template uri_template = URI::Template::ParseTemplate("http://example.com/search{?q,lang}");

    // Match it to the URI
    // &matched_values can be nullptr if you don't care about values.
    std::unordered_map<std::string, URI::Template::VarValue> matched_values;
    bool matched = URI::Template::MatchURI(uri_template, uri, &matched_values);

    // Print results
    std::cout << std::boolalpha;
    std::cout << "Template matched: " << matched << std::endl;
    for (const auto& [name, value] : matched_values) {
        std::cout << name << "=" << value << std::endl;
    }

    // Expand
    const std::string expanded_uri = URI::Template::ExpandTemplate(uri_template, matched_values);
    std::cout << "Template expanded: " << expanded_uri << std::endl;
}
```

```bash
g++ -std=c++17 example.cpp -luri-template
```
Output:
```
Template matched: true
lang=en
q=cat
Template expanded: http://example.com/search?q=cat&lang=en
```

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

You can install it (sudo may be required):
```bash
make install
```

Or test:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON ..
make test
```

### Cmake options

* **CMAKE_BUILD_TYPE** - [build type](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). `RelWithDebInfo` by default.
* **BUILD_SHARED_LIBS** - [build shared or static library](https://cmake.org/cmake/help/v3.0/variable/BUILD_SHARED_LIBS.html). `ON` by default.
* **BUILD_TESTING** - [build tests or not](https://cmake.org/cmake/help/latest/module/CTest.html). `OFF` by default.
* **URITEMPLATE_BUNDLED_MODE** - if the library is being built as a part of another project. If this options is set then *BUILD_SHARED_LIBS* forces to be OFF. Defined by `"${PROJECT_SOURCE_DIR}" == "${CMAKE_SOURCE_DIR}"` by default.

## License

Developed at **Tinkoff.ru** in 2021.\
Distibuted under **Apache License 2.0** [LICENSE](./LICENSE). You may also obtain this license at https://www.apache.org/licenses/LICENSE-2.0.

## Contacts

Author - i.s.vovk@tinkoff.ru\
Current maintainer - i.s.vovk@tinkoff.ru
