//
// Created by PC on 17.01.2025.
//

#include "TestSuite.h"

void TestSuite::add_test(const std::string& test_name, std::function<void()> test_function) {
    m_tests.emplace_back(test_name, test_function);
}

void TestSuite::run() const {
    uint32_t passed = 0;
    uint32_t failed = 0;

    for (const auto& [test_name, test_function] : m_tests) {
        try {
            test_function();
            std::cout << "[PASSED] " << test_name << "\n";
            ++passed;
        } catch (const std::exception& e) {
            std::cerr << "[FAILED] " << test_name << ": " << e.what() << "\n";
            ++failed;
        } catch (...) {
            std::cerr << "[FAILED] " << test_name << ": Unknown error\n";
            ++failed;
        }
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";
}