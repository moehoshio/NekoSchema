#include <neko/schema/types.hpp>
#include <iostream>
#include <string>

int main() {
    // Test basic integer types
    neko::int32 value = 42;
    neko::uint64 large = 9999999999ULL;
    neko::int8 tiny = 127;
    
    std::cout << "int32 value: " << value << std::endl;
    std::cout << "uint64 value: " << large << std::endl;
    std::cout << "int8 value: " << static_cast<int>(tiny) << std::endl;
    
    // Test string types
    neko::cstr greeting = "Hello from vcpkg!";
    std::string text = "Testing neko types";
    
    std::cout << "cstr: " << greeting << std::endl;
    std::cout << "string: " << text << std::endl;
    
    // Test enums
    auto state = neko::State::Completed;
    auto priority = neko::Priority::Normal;
    std::cout << "State: " << neko::toString(state) << std::endl;
    std::cout << "Priority: " << neko::toString(priority) << std::endl;
    
    // Verify types are correctly defined
    static_assert(std::is_same_v<neko::int32, std::int32_t>);
    static_assert(std::is_same_v<neko::uint64, std::uint64_t>);
    static_assert(std::is_same_v<neko::int8, std::int8_t>);
    
    std::cout << "All vcpkg integration tests passed!" << std::endl;
    return 0;
}
