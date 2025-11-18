cmake_minimum_required(VERSION 3.14)
project(NekoSchema VERSION 1.1.0 LANGUAGES CXX)

# ================
# === Config ====
# ================

# Neko schema project specific options
option(NEKO_SCHEMA_BUILD_TESTS "Neko Schema Build tests" ON)
option(NEKO_SCHEMA_AUTO_FETCH_DEPS "Neko Schema Automatically fetch dependencies" ON)
option(NEKO_SCHEMA_ENABLE_MODULE "Neko Schema Enable C++20 module" OFF)

find_package(GTest QUIET)

# Print configuration summary
message(STATUS "Start configuration Neko Schema...")
message(STATUS "")
message(STATUS "Neko Schema configuration summary:")
message(STATUS "  - CMake version: ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "  - Neko Schema Auto fetch deps: ${NEKO_SCHEMA_AUTO_FETCH_DEPS}")
message(STATUS "  - Neko Schema Build tests: ${NEKO_SCHEMA_BUILD_TESTS}")
message(STATUS "  - Neko Schema Enable module: ${NEKO_SCHEMA_ENABLE_MODULE}")
message(STATUS "")
message(STATUS "Dependency summary:")
message(STATUS "  - GTest : ${GTest_FOUND} version : ${GTest_VERSION}")
message(STATUS "")


if(NEKO_SCHEMA_AUTO_FETCH_DEPS)
    include(FetchContent)

    if(NOT GTEST_FOUND AND NEKO_SCHEMA_BUILD_TESTS)
        message(STATUS "GTest not found; Neko Schema Fetching GoogleTest...")

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.17.0
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


# ================
# = C++20 Module =
# ================

if(NEKO_SCHEMA_ENABLE_MODULE)
    message(STATUS "NekoSchema C++20 module enabled (NEKO_SCHEMA_ENABLE_MODULE=ON)")
    
    # Check CMake version for module support
    if(CMAKE_VERSION VERSION_LESS 3.28)
        message(WARNING "CMake 3.28+ is recommended for full C++20 module support")
    endif()
    
    # Create module library
    add_library(NekoSchema_module)
    add_library(Neko::Schema::Module ALIAS NekoSchema_module)
    
    target_sources(NekoSchema_module
        PUBLIC
            FILE_SET CXX_MODULES FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/include/neko/schema/neko.schema.cppm
    )
    
    target_compile_features(NekoSchema_module PUBLIC cxx_std_20)
    
    # Compiler-specific module flags
    if(MSVC)
        target_compile_options(NekoSchema_module PUBLIC /experimental:module)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(NekoSchema_module PUBLIC -fmodules-ts)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(NekoSchema_module PUBLIC -fmodules)
    endif()
    
    # Module still needs access to the include directory for the headers
    target_include_directories(NekoSchema_module PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    
else()
    message(STATUS "NekoSchema C++20 module disabled (NEKO_SCHEMA_ENABLE_MODULE=OFF)")
endif()

# ================
# ==== Tests =====
# ================

if(NEKO_SCHEMA_BUILD_TESTS)
    enable_testing()
    message(STATUS "NekoSchema tests enabled (NEKO_SCHEMA_BUILD_TESTS=ON)")

    if (NOT GTEST_FOUND AND NOT NEKO_SCHEMA_AUTO_FETCH_DEPS)
        message(WARNING "GTest is required for building tests but was not found.")
        message(FATAL_ERROR "Please enable -DNEKO_SCHEMA_AUTO_FETCH_DEPS=ON or install GTest and make it discoverable by CMake. e.g -DCMAKE_PREFIX_PATH=</path/to/googletest>")
    endif()
    

    # Traditional header-based tests
    add_executable(NekoSchema_tests tests/schema_test.cpp)    
    target_link_libraries(NekoSchema_tests PRIVATE NekoSchema GTest::gtest GTest::gtest_main)
    target_compile_features(NekoSchema_tests PRIVATE cxx_std_20)
    
    include(GoogleTest)
    gtest_discover_tests(NekoSchema_tests DISCOVERY_MODE PRE_TEST)
    
    # Module-based tests (if module is enabled)
    if(NEKO_SCHEMA_ENABLE_MODULE)
        message(STATUS "NekoSchema module tests enabled")
        add_executable(NekoSchema_module_tests tests/schema_module_test.cpp)
        target_link_libraries(NekoSchema_module_tests PRIVATE NekoSchema_module GTest::gtest GTest::gtest_main)
        target_compile_features(NekoSchema_module_tests PRIVATE cxx_std_20)
        gtest_discover_tests(NekoSchema_module_tests DISCOVERY_MODE PRE_TEST)
    endif()
    
else()
    message(STATUS "NekoSchema tests disabled (NEKO_SCHEMA_BUILD_TESTS=OFF)")
endif()


# ================
# == Install =====
# ================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING 
    PATTERN "*.hpp"
    PATTERN "*.cppm"
)

# Install targets
install(TARGETS NekoSchema
    EXPORT NekoSchemaTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export targets
install(EXPORT NekoSchemaTargets
    FILE NekoSchemaTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSchema
)

# Create and install Config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/NekoSchemaConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSchemaConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSchema
)

# Create and install Version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSchemaConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSchemaConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSchemaConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSchema
)

message(STATUS "Neko Schema End of configuration")