# URI-template

[![Language C++](https://img.shields.io/badge/language-c++-blue.svg?logo=c%2B%2B)](https://isocpp.org)
[![Github releases](https://img.shields.io/github/release/TinkoffCreditSystems/uri-template.svg)](https://github.com/TinkoffCreditSystems/uri-template/releases)
[![Coverage Status](https://coveralls.io/repos/github/TinkoffCreditSystems/uri-template/badge.svg?branch=develop)](https://coveralls.io/github/TinkoffCreditSystems/uri-template?branch=develop)
[![Conan Package](https://img.shields.io/badge/Conan-package-blueviolet)](https://conan.io/center/tcsbank-uri-template)
[![License](https://img.shields.io/github/license/TinkoffCreditSystems/uri-template.svg)](./LICENSE)

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

## Detailed description

For full API reference look here – https://tinkoffcreditsystems.github.io/uri-template/

## How to use in your project

Generally, to use this library you need to tell your compiler where to lookup for its' headers and library. For gcc/clang it can be done via `-I` and `-l` flags. Any particular situation depends on what you are using to build your project.

### Use installed

Easiest way is to install this library onto your system. To do so, execute these commands from `uri-template` folder (sudo may be required):

```bash
cmake -H. -Bbuild -DUCONFIG_BUILD_TESTING=OFF -DUCONFIG_BUILD_DOCS=OFF
cmake --build ./build --target install
```

This will put uri-template headers into system default folder. From there you should be able to use it like any other library (`#include <uri-template/uri-template.h>` and so on).

### Manually

If you [have installed](#use-installed) uri-template then you only need to link with it via `-luri-template`. If you don't want to install, pass an `-I` flag with path to uri-template include folder and `-l` with path to library binary. For example, if you cloned it into `~/uri-template/` and build it there, then use `-I~/uri-template/include -l~/uri-template/build/liburi-template.a` when calling gcc or clang.

### Cmake

If you [have installed](#use-installed) uri-template then use `find_package(uri-template REQUIRED)` and `target_link_libraries(<your target> uri-template::uri-template)`. Alternatively, you can use cmake's [`add_subdirectory`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html), [`ExternalProject`](https://cmake.org/cmake/help/latest/module/ExternalProject.html), [`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html) to bring it and include in configure stage of you project.

Also, this may be helpful - https://cliutils.gitlab.io/modern-cmake/

## How to build

This library supposed to be somewhat multi-platform, however, it was tested and mainly used on ubuntu and macOS. </br>
Prefer [out-of-source](https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#what-is-an-out-of-source-build) building:

```bash
cmake -H. -Bbuild
cmake --build ./build
```

To install (sudo may be required):
```bash
cmake -H. -Bbuild -DUCONFIG_BUILD_TESTING=OFF -DUCONFIG_BUILD_DOCS=OFF
cmake --build ./build --target install
```

Or test:
```bash
cmake -H. -Bbuild -DUCONFIG_BUILD_TESTING=ON
cmake --build ./build
cmake -E chdir ./build ctest --output-on-failure
```

*All these commands assume you are in uconfig root folder*

### Cmake options

* **CMAKE_BUILD_TYPE** – [build type](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). `RelWithDebInfo` by default.
* **BUILD_SHARED_LIBS** – [build shared or static library](https://cmake.org/cmake/help/v3.0/variable/BUILD_SHARED_LIBS.html). `OFF` by default.
* **UCONFIG_BUILD_TESTING** – build included unit-tests. `OFF` by default.
* **UCONFIG_BUILD_DOCS** – build html (sphinx) reference docs. `OFF` by default.

## License

Developed at **Tinkoff.ru** in 2021.\
Distibuted under **Apache License 2.0** [LICENSE](./LICENSE). You may also obtain this license at https://www.apache.org/licenses/LICENSE-2.0.

## Contacts

Author - i.s.vovk@tinkoff.ru\
Current maintainer - i.s.vovk@tinkoff.ru
