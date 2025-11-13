#pragma once

/* ===================== */
/* == Compiler Checks == */
/* ===================== */

#if !__has_include(<source_location>)
    #error "Neko SrcLoc Cannot find header <source_location>."
#endif

#include <source_location>

#if !defined(__cpp_lib_source_location) || __cpp_lib_source_location < 201907L
    #error "Neko SrcLoc requires <source_location> support."
#endif

#include <neko/schema/types.hpp>

namespace neko {

    inline namespace types {
        using SrcLoc = std::source_location;
    }

    /**
     * @brief Source location information
     */
    struct SrcLocInfo {
        neko::cstr file = nullptr;
        neko::uint32 line = 0;
        neko::cstr funcName = nullptr;

        constexpr SrcLocInfo(
            const SrcLoc &loc = SrcLoc::current()) noexcept
            : file(loc.file_name()), line(loc.line()), funcName(loc.function_name()) {}
        constexpr SrcLocInfo(neko::cstr file, neko::uint32 line, neko::cstr funcName) noexcept
            : file(file), line(line), funcName(funcName) {}

        constexpr neko::uint32 getLine() const noexcept { return line; }
        constexpr neko::cstr getFile() const noexcept { return file; }
        constexpr neko::cstr getFunc() const noexcept { return funcName; }
        constexpr bool hasInfo() const noexcept {
            return (line != 0 && file != nullptr) || funcName != nullptr;
        }
    };

} // namespace neko