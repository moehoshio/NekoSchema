cmake_minimum_required(VERSION 3.14)
project(NekoSchema VERSION 1.0 LANGUAGES CXX)

# ================
# === Config ====
# ================

# Neko schema project specific options
option(NEKO_SCHEMA_BUILD_TESTS "Neko Schema Build tests" ON)
option(NEKO_SCHEMA_AUTO_FETCH_DEPS "Neko Schema Automatically fetch dependencies" ON)


# Print configuration summary
message(STATUS "Start configuration Neko Schema...")
message(STATUS "Neko Schema configuration summary:")
message(STATUS "  - CMake version: ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "  - Neko Auto fetch deps: ${NEKO_SCHEMA_AUTO_FETCH_DEPS} common: ${NEKO_AUTO_FETCH_DEPS}")
message(STATUS "  - Neko Build tests: ${NEKO_SCHEMA_BUILD_TESTS} common: ${NEKO_BUILD_TESTS}")
message(STATUS "")
message(STATUS "Dependency summary:")
message(STATUS "  - No dependencies")
message(STATUS "")


if(NEKO_SCHEMA_AUTO_FETCH_DEPS)
    include(FetchContent)

    if(NEKO_SCHEMA_BUILD_TESTS)
        message(STATUS "Neko Schema Fetching GoogleTest...")

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.12.1
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

endif()


# ================
# = Main target =
# ================

add_library(NekoSchema INTERFACE)
add_library(Neko::Schema ALIAS NekoSchema)

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


# ================
# ==== Tests =====
# ================

if(NEKO_SCHEMA_BUILD_TESTS)
    enable_testing()
    message(STATUS "NekoSchema tests enabled")

    if (NOT NEKO_SCHEMA_AUTO_FETCH_DEPS)
        message(FATAL_ERROR "GTest is required for building tests. Please enable NEKO_SCHEMA_AUTO_FETCH_DEPS")
    endif()

    add_executable(NekoSchema_tests tests/schema_test.cpp)    
    target_link_libraries(NekoSchema_tests PRIVATE NekoSchema GTest::gtest_main)
    target_compile_features(NekoSchema_tests PRIVATE cxx_std_20)
    
    include(GoogleTest)
    gtest_discover_tests(NekoSchema_tests)
    
else()
    message(STATUS "NekoSchema tests disabled (NEKO_BUILD_TESTS=OFF)")
endif()

message(STATUS "End Neko Schema")