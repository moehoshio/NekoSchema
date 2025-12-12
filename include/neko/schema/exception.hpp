/**
 * @file exception.hpp
 * @brief Exception classes for the NekoLauncher
 */
#pragma once

#if !defined(NEKO_SCHEMA_ENABLE_MODULE) || (NEKO_SCHEMA_ENABLE_MODULE == false)
#include <neko/schema/types.hpp>
#include <neko/schema/srcLoc.hpp>

#include <exception>
#include <string>
#endif

/**
 * @brief Exception classes
 * @namespace neko::ex
 */
namespace neko::ex {

    /**
     * @brief Base error class extending std::exception and std::nested_exception.
     *
     * Provides basic error handling functionality for all derived error types.
     * Stores error message and extension info
     */
    class Exception : public std::exception, public std::nested_exception {
    private:
        std::string msg;
        neko::SrcLocInfo srcLoc;

    public:
        /**
         * @brief Construct an Exception with a message.
         * @param Msg Error message.
         * @param SrcLoc Source location information.
         */
        explicit Exception(const std::string &Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : msg(Msg), srcLoc(SrcLoc) {}
        /**
         * @brief Construct an Exception with a C-string message.
         * @param Msg Error message.
         * @param SrcLoc Source location information.
         */
        explicit Exception(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : msg(Msg ? Msg : ""), srcLoc(SrcLoc) {}

        /**
         * @brief Get the error message.
         * @return Error message as a C-string.
         */
        neko::cstr what() const noexcept override {
            return msg.c_str();
        }

        /**
         * @brief Check if extra error info is available.
         * @return True if extension info is present.
         */
        bool hasSrcLocInfo() const noexcept {
            return srcLoc.hasInfo();
        }

        /**
         * @brief Get the extended error information.
         * @return Reference to ErrorExtensionInfo.
         */
        const neko::SrcLocInfo &getSrcLoc() const noexcept {
            return srcLoc;
        }

        /**
         * @brief Get the line number where the error occurred.
         * @return Line number.
         */
        neko::uint32 getLine() const noexcept {
            return srcLoc.getLine();
        }
        /**
         * @brief Get the file name where the error occurred.
         * @return File name as a C-string.
         */
        neko::cstr getFile() const noexcept {
            return srcLoc.getFile();
        }
        /**
         * @brief Get the function name where the error occurred.
         * @return Function name as a C-string.
         */
        neko::cstr getFunc() const noexcept {
            return srcLoc.getFunc();
        }
        /**
         * @brief Get the error message as a string.
         * @return Error message.
         */
        const std::string &getMessage() const noexcept {
            return msg;
        }
    };

    /**
     * @brief Exception for program termination or exit.
     */
    class ProgramExit : public Exception {
    public:
        explicit ProgramExit(const std::string &Msg = "Program exited!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : Exception(Msg, SrcLoc) {}
    };

    // ---------------------------------------------------------------------
    // Logic-layer errors
    // ---------------------------------------------------------------------

    class LogicError : public Exception {
    public:
        explicit LogicError(const std::string &Msg = "Logic error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : Exception(Msg, SrcLoc) {}
        explicit LogicError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : Exception(Msg ? Msg : "Logic error!", SrcLoc) {}
    };

    class ArgumentError : public LogicError {
    public:
        explicit ArgumentError(const std::string &Msg = "Invalid argument!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg, SrcLoc) {}
        explicit ArgumentError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg ? Msg : "Invalid argument!", SrcLoc) {}
    };

    class RangeError : public ArgumentError {
    public:
        explicit RangeError(const std::string &Msg = "Out of range!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : ArgumentError(Msg, SrcLoc) {}
        explicit RangeError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : ArgumentError(Msg ? Msg : "Out of range!", SrcLoc) {}
    };

    class NotSupported : public LogicError {
    public:
        explicit NotSupported(const std::string &Msg = "Not supported!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg, SrcLoc) {}
        explicit NotSupported(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg ? Msg : "Not supported!", SrcLoc) {}
    };

    class InvalidState : public LogicError {
    public:
        explicit InvalidState(const std::string &Msg = "Invalid state!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg, SrcLoc) {}
        explicit InvalidState(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg ? Msg : "Invalid state!", SrcLoc) {}
    };

    class AssertionFailure : public LogicError {
    public:
        explicit AssertionFailure(const std::string &Msg = "Assertion failed!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg, SrcLoc) {}
        explicit AssertionFailure(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg ? Msg : "Assertion failed!", SrcLoc) {}
    };

    class DuplicateError : public LogicError {
    public:
        explicit DuplicateError(const std::string &Msg = "Object already exists!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg, SrcLoc) {}
        explicit DuplicateError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : LogicError(Msg ? Msg : "Object already exists!", SrcLoc) {}
    };

    // ---------------------------------------------------------------------
    // Runtime-layer errors
    // ---------------------------------------------------------------------

    class RuntimeError : public Exception {
    public:
        explicit RuntimeError(const std::string &Msg = "Runtime error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : Exception(Msg, SrcLoc) {}
        explicit RuntimeError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : Exception(Msg ? Msg : "Runtime error!", SrcLoc) {}
    };

    class ConfigurationError : public RuntimeError {
    public:
        explicit ConfigurationError(const std::string &Msg = "Configuration error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit ConfigurationError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "Configuration error!", SrcLoc) {}
    };

    class ParseError : public RuntimeError {
    public:
        explicit ParseError(const std::string &Msg = "Parse error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit ParseError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "Parse error!", SrcLoc) {}
    };

    class ConcurrencyError : public RuntimeError {
    public:
        explicit ConcurrencyError(const std::string &Msg = "Concurrency error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit ConcurrencyError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "Concurrency error!", SrcLoc) {}
    };

    class TaskRejectedError : public ConcurrencyError {
    public:
        explicit TaskRejectedError(const std::string &Msg = "Task rejected!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : ConcurrencyError(Msg, SrcLoc) {}
        explicit TaskRejectedError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : ConcurrencyError(Msg ? Msg : "Task rejected!", SrcLoc) {}
    };

    class PermissionDeniedError : public RuntimeError {
    public:
        explicit PermissionDeniedError(const std::string &Msg = "Permission denied!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit PermissionDeniedError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "Permission denied!", SrcLoc) {}
    };

    class TimeoutError : public RuntimeError {
    public:
        explicit TimeoutError(const std::string &Msg = "Timeout!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit TimeoutError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "Timeout!", SrcLoc) {}
    };

    class SystemError : public RuntimeError {
    public:
        explicit SystemError(const std::string &Msg = "System error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg, SrcLoc) {}
        explicit SystemError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : RuntimeError(Msg ? Msg : "System error!", SrcLoc) {}
    };

    class FileError : public SystemError {
    public:
        explicit FileError(const std::string &Msg = "File error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg, SrcLoc) {}
        explicit FileError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg ? Msg : "File error!", SrcLoc) {}
    };

    class NetworkError : public SystemError {
    public:
        explicit NetworkError(const std::string &Msg = "Network error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg, SrcLoc) {}
        explicit NetworkError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg ? Msg : "Network error!", SrcLoc) {}
    };

    class DatabaseError : public SystemError {
    public:
        explicit DatabaseError(const std::string &Msg = "Database error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg, SrcLoc) {}
        explicit DatabaseError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg ? Msg : "Database error!", SrcLoc) {}
    };

    class ExternalDependencyError : public SystemError {
    public:
        explicit ExternalDependencyError(const std::string &Msg = "External dependency error!", const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg, SrcLoc) {}
        explicit ExternalDependencyError(neko::cstr Msg, const neko::SrcLocInfo &SrcLoc = {}) noexcept
            : SystemError(Msg ? Msg : "External dependency error!", SrcLoc) {}
    };

    // ---------------------------------------------------------------------
    // Compatibility aliases (previous names kept for callers)
    // ---------------------------------------------------------------------

    using AlreadyExists [[deprecated("Use DuplicateError")]] = DuplicateError;
    using InvalidArgument [[deprecated("Use ArgumentError")]] = ArgumentError;
    using OutOfRange [[deprecated("Use RangeError")]] = RangeError;
    using NotImplemented [[deprecated("Use NotSupported")]] = NotSupported;
    using InvalidOperation [[deprecated("Use InvalidState")]] = InvalidState;
    using Assertion [[deprecated("Use AssertionFailure")]] = AssertionFailure;
    using Config [[deprecated("Use ConfigurationError")]] = ConfigurationError;
    using Parse [[deprecated("Use ParseError")]] = ParseError;
    using Concurrency [[deprecated("Use ConcurrencyError")]] = ConcurrencyError;
    using TaskRejected [[deprecated("Use TaskRejectedError")]] = TaskRejectedError;
    using PermissionDenied [[deprecated("Use PermissionDeniedError")]] = PermissionDeniedError;
    using Timeout [[deprecated("Use TimeoutError")]] = TimeoutError;
    using Logic [[deprecated("Use LogicError")]] = LogicError;
    using Runtime [[deprecated("Use RuntimeError")]] = RuntimeError;
    using ExternalLibraryError [[deprecated("Use ExternalDependencyError")]] = ExternalDependencyError;

} // namespace neko::ex