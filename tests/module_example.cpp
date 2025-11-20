/**
 * @file module_example.cpp
 * @brief Example of using NekoSchema C++20 module
 * @details This example demonstrates how to use the NekoSchema module interface
 * 
 * Compile with:
 *   MSVC: cl /std:c++20 /experimental:module module_example.cpp
 *   GCC:  g++ -std=c++20 -fmodules-ts module_example.cpp
 *   Clang: clang++ -std=c++20 -fmodules module_example.cpp
 */

#include <iostream>

import neko.schema;

int main() {
    std::cout << "=== NekoSchema C++20 Module Example ===" << std::endl << std::endl;
    
    // =========================================================================
    // Type Definitions
    // =========================================================================
    std::cout << "1. Type Definitions:" << std::endl;
    
    neko::uint32 count = 42;
    neko::int64 bigNumber = -123456789;
    neko::cstr message = "Hello from NekoSchema module!";
    neko::strview view = message;
    
    std::cout << "  count (uint32): " << count << std::endl;
    std::cout << "  bigNumber (int64): " << bigNumber << std::endl;
    std::cout << "  message (cstr): " << message << std::endl;
    std::cout << "  view (strview): " << view << std::endl;
    std::cout << std::endl;
    
    // =========================================================================
    // Enumerations
    // =========================================================================
    std::cout << "2. Enumerations:" << std::endl;
    
    neko::SyncMode mode = neko::SyncMode::Async;
    neko::State state = neko::State::Completed;
    neko::Priority priority = neko::Priority::High;
    
    std::cout << "  SyncMode: " << static_cast<int>(mode) << std::endl;
    std::cout << "  State: " << neko::toString(state) << std::endl;
    std::cout << "  Priority: " << neko::toString(priority) << std::endl;
    std::cout << std::endl;
    
    // =========================================================================
    // Source Location
    // =========================================================================
    std::cout << "3. Source Location:" << std::endl;
    
    neko::SrcLocInfo loc;
    std::cout << "  File: " << loc.getFile() << std::endl;
    std::cout << "  Line: " << loc.getLine() << std::endl;
    std::cout << "  Function: " << loc.getFunc() << std::endl;
    std::cout << std::endl;
    
    // =========================================================================
    // Exception Handling
    // =========================================================================
    std::cout << "4. Exception Handling:" << std::endl;
    
    try {
        throw neko::ex::Exception("This is a test exception from module");
    } catch (const neko::ex::Exception& e) {
        std::cout << "  Caught exception: " << e.what() << std::endl;
        std::cout << "  Exception location: " << e.getFile() << ":" << e.getLine() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "=== Example completed successfully ===" << std::endl;
    
    return 0;
}
