cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

project(NekoSchemaConanTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(NekoSchema REQUIRED CONFIG)

add_executable(conan_test test.cpp)
target_link_libraries(conan_test PRIVATE Neko::Schema)
