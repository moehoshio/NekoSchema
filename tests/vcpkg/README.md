# vcpkg Integration Test

This directory contains integration tests for the vcpkg package.

## Files

- `CMakeLists.txt` - CMake configuration for the test
- `test.cpp` - Test source code that verifies vcpkg package installation

## Usage

This test is automatically run by the vcpkg CI workflow (`.github/workflows/vcpkg.yml`).

To run locally with vcpkg:

```bash
# Copy the port files to your local vcpkg instance
cp -r packages/vcpkg/neko-schema/* /path/to/vcpkg/ports/neko-schema/

# Install the package
vcpkg install neko-schema

# Build the test
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build

# Run the test
./build/vcpkg_test  # Unix
.\build\Release\vcpkg_test.exe  # Windows
```
