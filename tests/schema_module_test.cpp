/**
 * @file schema_module_test.cpp
 * @brief Test file for NekoSchema C++20 module
 * @details This test file uses the C++20 module interface instead of traditional headers
 */

// Include GTest and standard headers BEFORE importing the module
// This is necessary to avoid symbol conflicts with MSVC's module implementation

#if !defined(__cpp_lib_modules) || (__cpp_lib_modules < 202207L)
#include <sstream>
#include <stdexcept>
#include <string>
#else
import std.core;
import std.string;
import std.exception;
import std.source_location;
import std.array;
import std.optional;
import std.utility;
#endif

#include <gtest/gtest.h>

import neko.schema;

using namespace neko;

// =============================================================================
// Module Import Tests
// =============================================================================

class ModuleTypesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ModuleTypesTest, BasicTypesSize) {
    // Test that type aliases have correct sizes
    EXPECT_EQ(sizeof(uint64), 8u);
    EXPECT_EQ(sizeof(uint32), 4u);
    EXPECT_EQ(sizeof(uint16), 2u);
    EXPECT_EQ(sizeof(uint8), 1u);

    EXPECT_EQ(sizeof(int64), 8u);
    EXPECT_EQ(sizeof(int32), 4u);
    EXPECT_EQ(sizeof(int16), 2u);
    EXPECT_EQ(sizeof(int8), 1u);
}

TEST_F(ModuleTypesTest, StringTypes) {
    // Test string type aliases
    const char *test_cstr = "test";
    cstr c = test_cstr;
    EXPECT_STREQ(c, "test");

    strview sv = "test_view";
    EXPECT_EQ(sv, "test_view");
    EXPECT_EQ(sv.size(), 9u);
}

TEST_F(ModuleTypesTest, SyncModeEnum) {
    // Test SyncMode enum values
    EXPECT_EQ(static_cast<int>(SyncMode::Sync), 0);
    EXPECT_EQ(static_cast<int>(SyncMode::Async), 1);
}

TEST_F(ModuleTypesTest, StateEnum) {
    // Test State enum
    State completed = State::Completed;
    State actionNeeded = State::ActionNeeded;
    State retryRequired = State::RetryRequired;

    EXPECT_NE(completed, actionNeeded);
    EXPECT_NE(actionNeeded, retryRequired);
    EXPECT_NE(completed, retryRequired);
}

TEST_F(ModuleTypesTest, PriorityEnum) {
    // Test Priority enum values
    EXPECT_EQ(static_cast<uint8>(Priority::Low), 0);
    EXPECT_EQ(static_cast<uint8>(Priority::Normal), 1);
    EXPECT_EQ(static_cast<uint8>(Priority::High), 2);
    EXPECT_EQ(static_cast<uint8>(Priority::Critical), 3);
}

TEST_F(ModuleTypesTest, PriorityToString) {
    // Test Priority toString function
    EXPECT_STREQ(toString(Priority::Low), "Low");
    EXPECT_STREQ(toString(Priority::Normal), "Normal");
    EXPECT_STREQ(toString(Priority::High), "High");
    EXPECT_STREQ(toString(Priority::Critical), "Critical");
}

TEST_F(ModuleTypesTest, StateToString) {
    // Test State toString function
    EXPECT_STREQ(toString(State::Completed), "Completed");
    EXPECT_STREQ(toString(State::ActionNeeded), "ActionNeeded");
    EXPECT_STREQ(toString(State::RetryRequired), "RetryRequired");
    EXPECT_STREQ(toString(State::Failed), "Failed");
}

// =============================================================================
// Module SrcLoc Tests
// =============================================================================

class ModuleSrcLocTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ModuleSrcLocTest, DefaultConstruction) {
    SrcLocInfo info;

    // The default constructor should capture current location
    EXPECT_TRUE(info.hasInfo());
    EXPECT_NE(info.getLine(), 0u);
    EXPECT_NE(info.getFile(), nullptr);
    EXPECT_NE(info.getFunc(), nullptr);
}

TEST_F(ModuleSrcLocTest, ManualConstruction) {
    SrcLocInfo info("test.cpp", 42, "testFunction");

    EXPECT_TRUE(info.hasInfo());
    EXPECT_EQ(info.getLine(), 42u);
    EXPECT_STREQ(info.getFile(), "test.cpp");
    EXPECT_STREQ(info.getFunc(), "testFunction");
}

TEST_F(ModuleSrcLocTest, EmptyInfo) {
    SrcLocInfo info(nullptr, 0, nullptr);

    EXPECT_FALSE(info.hasInfo());
    EXPECT_EQ(info.getLine(), 0u);
    EXPECT_EQ(info.getFile(), nullptr);
    EXPECT_EQ(info.getFunc(), nullptr);
}

// =============================================================================
// Module Exception Tests
// =============================================================================

class ModuleExceptionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ModuleExceptionTest, BasicException) {
    neko::ex::Exception ex("Test error");

    EXPECT_STREQ(ex.what(), "Test error");
    EXPECT_TRUE(ex.hasSrcLocInfo());
}

TEST_F(ModuleExceptionTest, ExceptionWithLocation) {
    SrcLocInfo loc("test.cpp", 100, "testFunc");
    neko::ex::Exception ex("Test error", loc);

    EXPECT_STREQ(ex.what(), "Test error");
    EXPECT_TRUE(ex.hasSrcLocInfo());
    EXPECT_EQ(ex.getLine(), 100u);
    EXPECT_STREQ(ex.getFile(), "test.cpp");
    EXPECT_STREQ(ex.getFunc(), "testFunc");
}

TEST_F(ModuleExceptionTest, CStringConstructor) {
    const char *msg = "C-string error";
    neko::ex::Exception ex(msg);

    EXPECT_STREQ(ex.what(), "C-string error");
}

TEST_F(ModuleExceptionTest, InvalidOperationException) {
    neko::ex::InvalidOperation ex("Invalid operation");

    EXPECT_STREQ(ex.what(), "Invalid operation");
    EXPECT_TRUE(ex.hasSrcLocInfo());
}

TEST_F(ModuleExceptionTest, ArgumentException) {
    neko::ex::InvalidArgument ex("Bad argument");

    EXPECT_STREQ(ex.what(), "Bad argument");
}

// =============================================================================
// Main
// =============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
