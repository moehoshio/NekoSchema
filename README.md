# NekoSchema

A lightweight C++20 header-only library providing fundamental type definitions and utilities for the Neko ecosystem.

## Overview

NekoSchema serves as a foundational module that defines common types, enumerations, and exception handling utilities. It provides consistent type aliases and basic infrastructure components that can be shared across multiple projects.

[![License](https://img.shields.io/badge/License-MIT%20OR%20Apache--2.0-blue.svg)](LICENSE)
![Require](https://img.shields.io/badge/%20Require%20-%3E=%20C++%2020-orange.svg)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

## Features

- **Type Definitions**: Standard type aliases for integers, strings, and character types
- **Enumerations**: Common enums for synchronization modes, states, and priorities
- **Exception Handling**: Comprehensive exception classes with source location tracking
- **Source Location**: Utilities for capturing and handling source code location information
- **Header-Only**: No compilation required, just include and use

## Requirements

- C++20 or higher compatible compiler
- CMake 3.14 or higher (if using CMake)

## Integration

### CMake

Add NekoSchema to your CMake project using `FetchContent`:

```cmake
include(FetchContent)

# Add NekoSchema to your CMake project
FetchContent_Declare(
    NekoSchema
    GIT_REPOSITORY https://github.com/moehoshio/NekoSchema.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(NekoSchema)

# Define your target and link NekoSchema
add_executable(your_target main.cpp)

target_link_libraries(your_target PRIVATE NekoSchema)
```

### Manually

1. Clone or download the repository to your local machine:

```shell
git clone https://github.com/moehoshio/NekoSchema.git
```

or

```shell
curl -L -o NekoSchema.zip https://github.com/moehoshio/NekoSchema/archive/refs/heads/main.zip

unzip NekoSchema.zip
```

2. Copy the `include` folder to your include directory

```shell
cp -r NekoSchema/include/ /path/to/your/include/
```

## Type Definitions

Numerical Types:

```cpp
#include <neko/schema/types.hpp>

neko::uint8 c = 255;
neko::int16 s = -32768;
neko::uint32 count = 42;
neko::int64 i = -123456789;
```

String Types:

```cpp
#include <neko/schema/types.hpp>

neko::cstr t = "Sample C-String";
neko::uchar c = 'A';
neko::strview text = t;
```

Enumerations:

```cpp
#include <neko/schema/types.hpp>

neko::SyncMode mode = neko::SyncMode::Async;
neko::State state = neko::State::Completed;
neko::Priority priority = neko::Priority::High;
```

## Automatic Source Location

With the `neko::SrcLocInfo` object, you can automatically capture source code location information by simply constructing an empty object (`{}`).

```cpp
#include <neko/schema/srcLoc.hpp>

void exampleFunction(const neko::SrcLocInfo & srcloc = {}) {
    std::cout<< "call from " << srcloc.getFile() << ":" << srcloc.getLine() << " in function " << srcloc.getFunc() << std::endl;
}

void anotherFunction(const neko::SrcLocInfo & srcloc) {
    if (srcloc.hasInfo()){
        // ...
    }
}

int main() {
    exampleFunction(); // Automatically captures the call site information
    anotherFunction(neko::SrcLocInfo("file.cpp", 2, "CustomFunction")); // You can also provide custom source location
}
```

## Constexpr Map

```cpp
#include <neko/schema/map.hpp>

int main(){
constexpr neko::Map<neko::strview, neko::uint32> map{
    {"one", 1},
    {"two", 2},
    {"three", 3}
};
static_assert(map.at("two") == 2);
}
```

## Exception

```cpp
#include <neko/schema/exception.hpp>

try {

    throw neko::ex::Runtime("Sample runtime error");

} catch (const neko::ex::OutOfRange& e) {

    std::cout << "OutOfRange error caught at " << loc.getFile() << ":" << loc.getLine() << " in function " << loc.getFunc() << std::endl;
    std::cout << "Error message: " << e.what() << std::endl;

} catch (const neko::ex::SystemError& e) {

    if (e.hasSrcLocInfo()) {
        auto loc = e.getSrcLoc();
        // ...
    }
    
} catch (const neko::ex::Exception& e) {
    // ...
}
```

## Testing

You can run the tests to verify that everything is working correctly.

If you haven't configured the build yet, please run:

```shell
cmake -B ./build . -DNEKO_BUILD_TESTS=ON
```

Now, you can build the test files (you must build them manually at least once before running the tests!).

```shell
cmake --build ./build --config Debug
```

Then, you can run the tests with the following commands:

Unix Makefile / Ninja generator：

```shell
cmake --build ./build --target test
```

Visual Studio generator：

```shell
cmake --build ./build --config Debug --target RUN_TESTS
```

If everything is set up correctly, you should see output similar to the following:

```shell
  Test project /path/to/NekoSchema/build
        Start  1: TypesTest.BasicTypesSize
   1/28 Test  #1: TypesTest.BasicTypesSize ........................
    .....   Passed    0.02 sec

    .....

        Start 28: PerformanceTest.ConstexprMapLookupSpeed
  28/28 Test #28: PerformanceTest.ConstexprMapLookupSpeed .........
  .....   Passed    0.02 sec

  100% tests passed, 0 tests failed out of 28

  Total Test time (real) =   0.58 sec
```

### Disable Tests

If you want to disable building and running tests, you can set the following CMake option when configuring your project:

```shell
cmake -B ./build . -DNEKO_BUILD_TESTS=OFF
```

This will skip test targets during the build process.

## License

[LICENSE](LICENSE) MIT OR Apache-2.0

## See More

- [NekoLog](https://github.com/moehoshio/nlog): An easy-to-use, modern, lightweight, and efficient C++20 logging library.
- [NekoEvent](https://github.com/moehoshio/NekoEvent): A modern easy to use type-safe and high-performance event handling system for C++.
- [NekoSchema](https://github.com/moehoshio/NekoSchema): A lightweight, header-only C++20 schema library.
- [NekoSystem](https://github.com/moehoshio/NekoSystem): A modern C++20 cross-platform system utility library.
- [NekoFunction](https://github.com/moehoshio/NekoFunction): A comprehensive modern C++ utility library that provides practical functions for common programming tasks.
- [NekoThreadPool](https://github.com/moehoshio/NekoThreadPool): An easy to use and efficient C++ 20 thread pool that supports priorities and submission to specific threads.
