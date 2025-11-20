/**
 * @file neko.schema.cppm
 * @brief C++20 module interface for NekoSchema
 * @details This module exports all NekoSchema functionality by wrapping the header files.
 *          The original headers are still available for traditional include-based usage.
 */

module;

#if defined(__cpp_lib_modules) && (__cpp_lib_modules >= 202207L)
import std;
#else
// Global module fragment - include headers that should not be exported
#include <cstdint>
#include <string_view>
#include <string>
#include <exception>
#include <source_location>
#endif

export module neko.schema;

// Control header files to not import dependencies (dependencies are declared and imported by the cppm)
#define NEKO_SCHEMA_ENABLE_MODULE true

// Export all declarations from the headers by including them in an export block
export {
#include "types.hpp"
#include "srcLoc.hpp"
#include "exception.hpp"
}
