#include <gtest/gtest.h>
#include <neko/schema/types.hpp>
#include <neko/schema/exception.hpp>
#include <neko/schema/srcLoc.hpp>

#include <string>
#include <sstream>
#include <stdexcept>

using namespace neko;

// =============================================================================
// Types Tests
// =============================================================================

class TypesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TypesTest, BasicTypesSize) {
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

TEST_F(TypesTest, StringTypes) {
    // Test string type aliases
    const char* test_cstr = "test";
    cstr c = test_cstr;
    EXPECT_STREQ(c, "test");
    
    strview sv = "test_view";
    EXPECT_EQ(sv, "test_view");
    EXPECT_EQ(sv.size(), 9u);
}

TEST_F(TypesTest, SyncModeEnum) {
    // Test SyncMode enum values
    EXPECT_EQ(static_cast<int>(SyncMode::Sync), 0);
    EXPECT_EQ(static_cast<int>(SyncMode::Async), 1);
}

TEST_F(TypesTest, StateEnum) {
    // Test State enum
    State completed = State::Completed;
    State actionNeeded = State::ActionNeeded;
    State retryRequired = State::RetryRequired;
    
    EXPECT_NE(completed, actionNeeded);
    EXPECT_NE(actionNeeded, retryRequired);
    EXPECT_NE(completed, retryRequired);
}

TEST_F(TypesTest, PriorityEnum) {
    // Test Priority enum values
    EXPECT_EQ(static_cast<uint8>(Priority::Low), 0);
    EXPECT_EQ(static_cast<uint8>(Priority::Normal), 1);
    EXPECT_EQ(static_cast<uint8>(Priority::High), 2);
    EXPECT_EQ(static_cast<uint8>(Priority::Critical), 3);
}

TEST_F(TypesTest, PriorityToString) {
    // Test Priority toString function
    EXPECT_STREQ(toString(Priority::Low), "Low");
    EXPECT_STREQ(toString(Priority::Normal), "Normal");
    EXPECT_STREQ(toString(Priority::High), "High");
    EXPECT_STREQ(toString(Priority::Critical), "Critical");
}

// =============================================================================
// SrcLoc Tests
// =============================================================================

class SrcLocTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SrcLocTest, DefaultConstruction) {
    SrcLocInfo info;
    
    // The default constructor should capture current location
    EXPECT_TRUE(info.hasInfo());
    EXPECT_NE(info.getLine(), 0u);
    EXPECT_NE(info.getFile(), nullptr);
    EXPECT_NE(info.getFunc(), nullptr);
}

TEST_F(SrcLocTest, ManualConstruction) {
    SrcLocInfo info("test_file.cpp", 42, "test_function");
    
    EXPECT_TRUE(info.hasInfo());
    EXPECT_EQ(info.getLine(), 42u);
    EXPECT_STREQ(info.getFile(), "test_file.cpp");
    EXPECT_STREQ(info.getFunc(), "test_function");
}

TEST_F(SrcLocTest, EmptyInfo) {
    SrcLocInfo info(nullptr, 0, nullptr);
    
    EXPECT_FALSE(info.hasInfo());
    EXPECT_EQ(info.getLine(), 0u);
    EXPECT_EQ(info.getFile(), nullptr);
    EXPECT_EQ(info.getFunc(), nullptr);
}

TEST_F(SrcLocTest, PartialInfo) {
    // Test with only function name
    SrcLocInfo info1(nullptr, 0, "test_func");
    EXPECT_TRUE(info1.hasInfo());
    
    // Test with only file and line
    SrcLocInfo info2("test.cpp", 10, nullptr);
    EXPECT_TRUE(info2.hasInfo());
}

// =============================================================================
// Exception Tests
// =============================================================================

class ExceptionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ExceptionTest, BaseExceptionConstruction) {
    neko::ex::Exception ex("Test message");
    
    EXPECT_STREQ(ex.what(), "Test message");
    EXPECT_EQ(ex.getMessage(), "Test message");
}

TEST_F(ExceptionTest, ExceptionWithSourceLocation) {
    SrcLocInfo srcLoc("test.cpp", 100, "test_function");
    neko::ex::Exception ex("Test with location", srcLoc);
    
    EXPECT_STREQ(ex.what(), "Test with location");
    EXPECT_TRUE(ex.hasSrcLocInfo());
    EXPECT_EQ(ex.getLine(), 100u);
    EXPECT_STREQ(ex.getFile(), "test.cpp");
    EXPECT_STREQ(ex.getFunc(), "test_function");
}

TEST_F(ExceptionTest, ExceptionWithCString) {
    const char* msg = "C-style message";
    neko::ex::Exception ex(msg);
    
    EXPECT_STREQ(ex.what(), "C-style message");
}

TEST_F(ExceptionTest, ExceptionWithNullptr) {
    neko::ex::Exception ex(static_cast<const char*>(nullptr));
    
    EXPECT_STREQ(ex.what(), "");
}

TEST_F(ExceptionTest, SystemErrorHierarchy) {
    neko::ex::SystemError sysErr("System error");
    EXPECT_STREQ(sysErr.what(), "System error");
    
    neko::ex::FileError fileErr("File error");
    EXPECT_STREQ(fileErr.what(), "File error");
    
    neko::ex::NetworkError netErr("Network error");
    EXPECT_STREQ(netErr.what(), "Network error");
    
    neko::ex::DatabaseError dbErr("Database error");
    EXPECT_STREQ(dbErr.what(), "Database error");
}

TEST_F(ExceptionTest, SpecializedExceptions) {
    neko::ex::ArgumentError invalidArg("Invalid argument");
    EXPECT_STREQ(invalidArg.what(), "Invalid argument");
    
    neko::ex::RangeError outOfRange("Out of range");
    EXPECT_STREQ(outOfRange.what(), "Out of range");
    
    neko::ex::NotSupported notImpl("Not supported");
    EXPECT_STREQ(notImpl.what(), "Not supported");
    
    neko::ex::TimeoutError timeout("Timeout occurred");
    EXPECT_STREQ(timeout.what(), "Timeout occurred");
}

TEST_F(ExceptionTest, DefaultMessages) {
    // Test default messages for various exception types
    neko::ex::ArgumentError invalidArg;
    EXPECT_STREQ(invalidArg.what(), "Invalid argument!");
    
    neko::ex::FileError fileErr;
    EXPECT_STREQ(fileErr.what(), "File error!");
    
    neko::ex::NotSupported notImpl;
    EXPECT_STREQ(notImpl.what(), "Not supported!");
    
    neko::ex::TimeoutError timeout;
    EXPECT_STREQ(timeout.what(), "Timeout!");
}

TEST_F(ExceptionTest, ExceptionPolymorphism) {
    // Test that exceptions can be caught by their base types
    try {
        throw neko::ex::FileError("File not found");
    } catch (const neko::ex::SystemError& e) {
        EXPECT_STREQ(e.what(), "File not found");
    } catch (...) {
        FAIL() << "Should have caught as SystemError";
    }
    
    try {
        throw neko::ex::NetworkError("Connection failed");
    } catch (const neko::ex::Exception& e) {
        EXPECT_STREQ(e.what(), "Connection failed");
    } catch (...) {
        FAIL() << "Should have caught as Exception";
    }
}

TEST_F(ExceptionTest, ExceptionInheritanceFromStdException) {
    // Test that our exceptions inherit from std::exception
    neko::ex::ArgumentError ex("test");
    
    // Should be catchable as std::exception
    try {
        throw ex;
    } catch (const std::exception& e) {
        EXPECT_STREQ(e.what(), "test");
    } catch (...) {
        FAIL() << "Should have caught as std::exception";
    }
}

// =============================================================================
// Integration Tests
// =============================================================================

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(IntegrationTest, ExceptionWithTypesAndSrcLoc) {
    // Test integration between different components
    uint32 errorCode = 404;
    Priority priority = Priority::High;
    SrcLocInfo srcLoc("integration_test.cpp", 200, "test_function");
    
    std::string message = "Error code: " + std::to_string(errorCode) + 
                         ", Priority: " + toString(priority);
    
    neko::ex::SystemError error(message, srcLoc);
    
    EXPECT_EQ(error.getMessage(), "Error code: 404, Priority: High");
    EXPECT_TRUE(error.hasSrcLocInfo());
    EXPECT_EQ(error.getLine(), 200u);
    EXPECT_STREQ(error.getFunc(), "test_function");
}

// =============================================================================
// Edge Cases and Error Conditions
// =============================================================================

class EdgeCaseTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(EdgeCaseTest, ExceptionNesting) {
    // Test nested exception functionality
    try {
        try {
            throw neko::ex::FileError("Inner file error");
        } catch (...) {
            std::throw_with_nested(neko::ex::SystemError("Outer system error"));
        }
    } catch (const neko::ex::SystemError& e) {
        EXPECT_STREQ(e.what(), "Outer system error");
        
        // Check if there's a nested exception
        try {
            std::rethrow_if_nested(e);
        } catch (const neko::ex::FileError& nested) {
            EXPECT_STREQ(nested.what(), "Inner file error");
        } catch (...) {
            FAIL() << "Nested exception should be FileError";
        }
    }
}

// =============================================================================
// Performance Tests (Basic)
// =============================================================================

class PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// =============================================================================
// Main Test Runner
// =============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
