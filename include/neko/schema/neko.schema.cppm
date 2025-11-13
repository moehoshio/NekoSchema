/**
 * @file neko.schema.cppm
 * @brief C++20 module interface for NekoSchema
 * @details This module exports all NekoSchema functionality by wrapping the header files.
 *          The original headers are still available for traditional include-based usage.
 */

module;

// Global module fragment - include headers that should not be exported
#include <cstdint>
#include <string_view>
#include <source_location>
#include <exception>
#include <string>
#include <array>
#include <cstddef>
#include <optional>
#include <utility>

export module neko.schema;

// Export all declarations from the headers by including them in an export block
export {
    #include "types.hpp"
    #include "srcLoc.hpp"
    #include "exception.hpp"
    #include "constexprMap.hpp"
}
