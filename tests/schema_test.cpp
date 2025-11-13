#include <gtest/gtest.h>
#include <neko/schema/types.hpp>
#include <neko/schema/exception.hpp>
#include <neko/schema/srcLoc.hpp>
#include <neko/schema/constexprMap.hpp>

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
// ConstexprMap Tests
// =============================================================================

class ConstexprMapTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ConstexprMapTest, BasicFunctionality) {
    constexpr auto map = ConstexprMap{std::array{
        std::make_pair(1, "one"),
        std::make_pair(2, "two"),
        std::make_pair(3, "three")
    }};
    
    EXPECT_EQ(map.size(), 3u);
    EXPECT_FALSE(map.empty());
    
    auto result1 = map.find(1);
    ASSERT_TRUE(result1.has_value());
    EXPECT_STREQ(result1.value(), "one");
    
    auto result2 = map.find(2);
    ASSERT_TRUE(result2.has_value());
    EXPECT_STREQ(result2.value(), "two");
    
    auto result3 = map.find(3);
    ASSERT_TRUE(result3.has_value());
    EXPECT_STREQ(result3.value(), "three");
    
    auto result_not_found = map.find(4);
    EXPECT_FALSE(result_not_found.has_value());
}

TEST_F(ConstexprMapTest, EmptyMap) {
    constexpr auto empty_map = ConstexprMap{std::array<std::pair<int, const char*>, 0>{}};
    
    EXPECT_EQ(empty_map.size(), 0u);
    EXPECT_TRUE(empty_map.empty());
    
    auto result = empty_map.find(1);
    EXPECT_FALSE(result.has_value());
}

TEST_F(ConstexprMapTest, StringKeyMap) {
    using namespace std::string_view_literals;
    constexpr auto str_map = ConstexprMap{std::array{
        std::make_pair("key1"sv, 100),
        std::make_pair("key2"sv, 200),
        std::make_pair("key3"sv, 300)
    }};
    
    auto result = str_map.find("key2"sv);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 200);
    
    auto not_found = str_map.find("key4"sv);
    EXPECT_FALSE(not_found.has_value());
}

TEST_F(ConstexprMapTest, Iterator) {
    constexpr auto map = ConstexprMap{std::array{
        std::make_pair(1, "a"),
        std::make_pair(2, "b"),
        std::make_pair(3, "c")
    }};
    
    int count = 0;
    for (const auto& [key, value] : map) {
        count++;
        EXPECT_TRUE(key >= 1 && key <= 3);
        EXPECT_TRUE(value != nullptr);
    }
    EXPECT_EQ(count, 3);
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
    neko::ex::InvalidArgument invalidArg("Invalid argument");
    EXPECT_STREQ(invalidArg.what(), "Invalid argument");
    
    neko::ex::OutOfRange outOfRange("Out of range");
    EXPECT_STREQ(outOfRange.what(), "Out of range");
    
    neko::ex::NotImplemented notImpl("Not implemented");
    EXPECT_STREQ(notImpl.what(), "Not implemented");
    
    neko::ex::Timeout timeout("Timeout occurred");
    EXPECT_STREQ(timeout.what(), "Timeout occurred");
}

TEST_F(ExceptionTest, DefaultMessages) {
    // Test default messages for various exception types
    neko::ex::InvalidArgument invalidArg;
    EXPECT_STREQ(invalidArg.what(), "Invalid argument!");
    
    neko::ex::FileError fileErr;
    EXPECT_STREQ(fileErr.what(), "File error!");
    
    neko::ex::NotImplemented notImpl;
    EXPECT_STREQ(notImpl.what(), "Not implemented!");
    
    neko::ex::Timeout timeout;
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
    neko::ex::InvalidArgument ex("test");
    
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

TEST_F(IntegrationTest, ConstexprMapWithTypes) {
    // Test ConstexprMap with neko types
    constexpr auto statusMap = ConstexprMap{std::array{
        std::make_pair(static_cast<uint8>(Priority::Low), State::Completed),
        std::make_pair(static_cast<uint8>(Priority::Normal), State::ActionNeeded),
        std::make_pair(static_cast<uint8>(Priority::High), State::RetryRequired),
        std::make_pair(static_cast<uint8>(Priority::Critical), State::ActionNeeded)
    }};
    
    auto result = statusMap.find(static_cast<uint8>(Priority::High));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), State::RetryRequired);
    
    auto notFound = statusMap.find(99);
    EXPECT_FALSE(notFound.has_value());
}

// =============================================================================
// Edge Cases and Error Conditions
// =============================================================================

class EdgeCaseTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(EdgeCaseTest, LargeConstexprMap) {
    // Test with a larger map
    constexpr auto largeMap = ConstexprMap{std::array{
        std::make_pair(0, "zero"), std::make_pair(1, "one"), std::make_pair(2, "two"),
        std::make_pair(3, "three"), std::make_pair(4, "four"), std::make_pair(5, "five"),
        std::make_pair(6, "six"), std::make_pair(7, "seven"), std::make_pair(8, "eight"),
        std::make_pair(9, "nine")
    }};
    
    EXPECT_EQ(largeMap.size(), 10u);
    
    // Test first and last elements
    auto first = largeMap.find(0);
    ASSERT_TRUE(first.has_value());
    EXPECT_STREQ(first.value(), "zero");
    
    auto last = largeMap.find(9);
    ASSERT_TRUE(last.has_value());
    EXPECT_STREQ(last.value(), "nine");
    
    // Test middle element
    auto middle = largeMap.find(5);
    ASSERT_TRUE(middle.has_value());
    EXPECT_STREQ(middle.value(), "five");
}

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

TEST_F(PerformanceTest, ConstexprMapLookupSpeed) {
    constexpr auto map = ConstexprMap{std::array{
        std::make_pair(1, "one"), std::make_pair(2, "two"), std::make_pair(3, "three"),
        std::make_pair(4, "four"), std::make_pair(5, "five"), std::make_pair(6, "six"),
        std::make_pair(7, "seven"), std::make_pair(8, "eight"), std::make_pair(9, "nine"),
        std::make_pair(10, "ten")
    }};
    
    // Perform multiple lookups (this is more of a smoke test than a real perf test)
    for (int i = 0; i < 1000; ++i) {
        auto result = map.find(i % 10 + 1);
        if (i % 10 == 0) {
            ASSERT_TRUE(result.has_value());
            EXPECT_STREQ(result.value(), "one");
        }
    }
}

// =============================================================================
// Main Test Runner
// =============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
