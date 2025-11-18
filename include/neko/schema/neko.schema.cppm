/**
 * @file neko.schema.cppm
 * @brief C++20 module interface for NekoSchema
 * @details This module exports all NekoSchema functionality by wrapping the header files.
 *          The original headers are still available for traditional include-based usage.
 */

module;

#if defined(__cpp_lib_modules) && (__cpp_lib_modules > 202207L)
import std.core;
import std.string;
import std.exception;
import std.source_location;
import std.array;
import std.optional;
import std.utility;
#else
// Global module fragment - include headers that should not be exported
#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>
#endif

export module neko.schema;

// Export all declarations from the headers by including them in an export block
export {
#include "exception.hpp"
#include "srcLoc.hpp"
#include "types.hpp"
}
