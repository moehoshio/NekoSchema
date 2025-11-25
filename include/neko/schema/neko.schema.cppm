// =====================
// === Global Module ===
// =====================

module;

// ====================
// = Standard Library =
// ====================

#include <cstdint>
#include <exception>
#include <source_location>
#include <string>
#include <string_view>

// =====================
// = Module Interface ==
// =====================

export module neko.schema;

// Control header files to not import dependencies (dependencies are declared and imported by the cppm)
#define NEKO_SCHEMA_ENABLE_MODULE true

// Export all declarations from the headers by including them in an export block
export {
#include "types.hpp"
#include "srcLoc.hpp"
#include "exception.hpp"
}
