//
// Created by PC on 17.01.2025.
//

#ifndef COURSE_PROJECT_TESTSUITE_H
#define COURSE_PROJECT_TESTSUITE_H

#include <iostream>
#include <sstream>
#include <stdexcept>

// The ASSERT_TRUE macro checks if the condition is true.
// If the condition is false, it throws an exception with an error message.
#define ASSERT_TRUE(condition)                                           \
    do {                                                                \
        if (!(condition)) {                                             \
            std::ostringstream oss;                                     \
            oss << "Assertion failed: " #condition                      \
                << " is false at " << __FILE__ << ":" << __LINE__;      \
            throw std::runtime_error(oss.str());                        \
        }                                                               \
    } while (false)

// The ASSERT_FALSE macro checks if the condition is false.
// If the condition is true, it throws an exception with an error message.
#define ASSERT_FALSE(condition)                                          \
    do {                                                                 \
        if (condition) {                                                 \
            std::ostringstream oss;                                      \
            oss << "Assertion failed: " #condition                       \
                << " is true at " << __FILE__ << ":" << __LINE__;        \
            throw std::runtime_error(oss.str());                         \
        }                                                                \
    } while (false)

// The ASSERT_EQ macro checks if the expected and actual values are equal.
// If the values are not equal, it throws an exception with detailed error information.
#define ASSERT_EQ(expected, actual)                                      \
    do {                                                                 \
        if ((expected) != (actual)) {                                    \
            std::ostringstream oss;                                      \
            oss << "Assertion failed: " #expected " == " #actual         \
                << "\n  Expected: " << (expected)                        \
                << "\n  Actual: " << (actual)                            \
                << "\n  Location: " << __FILE__ << ":" << __LINE__;      \
            throw std::runtime_error(oss.str());                         \
        }                                                                \
    } while (false)

// The ASSERT_NE macro checks if the expected and actual values are not equal.
// If the values are equal, it throws an exception with detailed error information.
#define ASSERT_NE(expected, actual)                                      \
    do {                                                                 \
        if ((expected) == (actual)) {                                    \
            std::ostringstream oss;                                      \
            oss << "Assertion failed: " #expected " != " #actual         \
                << "\n  Both values: " << (actual)                       \
                << "\n  Location: " << __FILE__ << ":" << __LINE__;      \
            throw std::runtime_error(oss.str());                         \
        }                                                                \
    } while (false)


#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cstdlib>
#include <cstdint>

class TestSuite {
private:
    std::vector<std::pair<std::string, std::function<void()>>> m_tests;
public:
    void add_test(const std::string& test_name, std::function<void()> test_function);
    void run() const;
};

#endif //COURSE_PROJECT_TESTSUITE_H
