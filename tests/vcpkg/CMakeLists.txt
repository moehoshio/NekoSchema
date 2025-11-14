cmake_minimum_required(VERSION 3.14)
project(NekoSchemaVcpkgTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the vcpkg port name (neko-schema) to find the package
find_package(neko-schema REQUIRED CONFIG)

add_executable(vcpkg_test test.cpp)
target_link_libraries(vcpkg_test PRIVATE Neko::Schema)