cmake_minimum_required(VERSION 3.14)
project(NekoSchema VERSION 1.0 LANGUAGES CXX)

add_library(NekoSchema INTERFACE)

target_include_directories(NekoSchema INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(NekoSchema INTERFACE cxx_std_20)