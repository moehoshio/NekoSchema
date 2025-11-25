#pragma once

#if !defined(NEKO_SCHEMA_ENABLE_MODULE) || (NEKO_SCHEMA_ENABLE_MODULE == false)
#include <cstdint>
#include <string_view>
#endif

namespace neko {

    // Basic types should use all-lowercase style
    inline namespace types {

        // =================
        // == Char/String ==
        // =================

        using cstr = const char *;
        using strview = std::string_view;
        using uchar = unsigned char;

        // ==================
        // ===== Number =====
        // ==================

        using uint64 = std::uint64_t;
        using uint32 = std::uint32_t;
        using uint16 = std::uint16_t;
        using uint8 = std::uint8_t;

        using int64 = std::int64_t;
        using int32 = std::int32_t;
        using int16 = std::int16_t;
        using int8 = std::int8_t;

        // =================
        // ===== Enums =====
        // =================

        enum class SyncMode {
            Sync = 0,
            Async = 1
        };

        enum class State {
            Completed,     // Operation finished successfully
            ActionNeeded,  // Action required from user or system
            RetryRequired, // Temporary failure, should retry later
            Failed         // Permanent failure, cannot proceed
        };

        enum class Priority : uint8 {
            Low = 0,
            Normal = 1,
            High = 2,
            Critical = 3
        };

        // =================
        // ===== Method ====
        // =================

        inline neko::cstr toString(State state) {
            switch (state) {
                case State::Completed:
                    return "Completed";
                case State::ActionNeeded:
                    return "ActionNeeded";
                case State::RetryRequired:
                    return "RetryRequired";
                case State::Failed:
                    return "Failed";
                default:
                    return "Unknown";
            }
        }

        inline neko::cstr toString(Priority priority) {
            switch (priority) {
                case Priority::Low:
                    return "Low";
                case Priority::Normal:
                    return "Normal";
                case Priority::High:
                    return "High";
                case Priority::Critical:
                    return "Critical";
                default:
                    return "Unknown";
            }
        }
    } // namespace types

} // namespace neko
