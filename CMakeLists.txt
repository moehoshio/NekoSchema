cmake_minimum_required(VERSION 3.14)
project(NekoSchema VERSION 1.0 LANGUAGES CXX)

option(NEKO_AUTO_FETCH_DEPS "Automatically fetch dependencies" ON)
option(NEKO_BUILD_TESTS "Build tests" ON)

if(NEKO_AUTO_FETCH_DEPS)

    if(NEKO_BUILD_TESTS)
        find_package(GTest QUIET)

        if(NOT GTest_FOUND)
            # If GTest is not found, fetch it
            include(FetchContent)
            FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG        release-1.12.1
            )
            # For Windows: Prevent overriding the parent project's compiler/linker settings
            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            FetchContent_MakeAvailable(googletest)
        endif(NOT GTest_FOUND)

    endif(NEKO_BUILD_TESTS)

endif(NEKO_AUTO_FETCH_DEPS)


# Library definition
add_library(NekoSchema INTERFACE)

target_include_directories(NekoSchema INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(NekoSchema INTERFACE cxx_std_20)


if(MSVC)
    target_compile_options(NekoSchema INTERFACE /Zc:__cplusplus)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16.10)
        message(FATAL_ERROR "MSVC version must be at least 16.10 (Visual Studio 2019 version 16.10) to support C++20 <source_location>.")
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(NekoSchema INTERFACE -std=c++20)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "GCC version must be at least 11.0 to support C++20 <source_location>.")
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(NekoSchema INTERFACE -std=c++20)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.0)
        message(FATAL_ERROR "Clang version must be at least 13.0 to support C++20 <source_location>.")
    endif()
endif()

# Testing support
if(NEKO_BUILD_TESTS)
    if(NOT NEKO_AUTO_FETCH_DEPS AND NOT GTest_FOUND)
        message(FATAL_ERROR "GTest is required for building tests. Please enable NEKO_AUTO_FETCH_DEPS or install GTest manually.")
    endif()

    enable_testing()

    # Add test executable
    add_executable(schema_tests tests/schema_test.cpp)
    
    target_link_libraries(schema_tests 
        PRIVATE 
        NekoSchema
        GTest::gtest_main
    )
    target_compile_features(schema_tests PRIVATE cxx_std_20)
    
    # Include test directories
    include(GoogleTest)
    gtest_discover_tests(schema_tests)
endif()