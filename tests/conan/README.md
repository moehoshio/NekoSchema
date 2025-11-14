# Conan Integration Test

This directory contains integration tests for the Conan package.

## Files

- `CMakeLists.txt` - CMake configuration for the test
- `test.cpp` - Test source code that verifies Conan package installation
- `conanfile.txt` - Conan dependencies declaration

## Usage

This test is automatically run by the Conan CI workflow (`.github/workflows/conan.yml`).

To run locally with Conan:

```bash
# Create the Conan package from local source
conan create . --build=missing

# Install dependencies
cd tests/conan
conan install . --build=missing

# Build the test
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run the test
./build/conan_test  # Unix
.\build\Release\conan_test.exe  # Windows
```
