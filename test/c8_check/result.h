/*
 * result.h
 */
#ifndef __RESULT_H
#define __RESULT_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

/*
 * Result class.
 */
class result {
public:
    result(const std::string &name) {
        name_ = name;
        pass_ = false;
    }

    auto get_name() -> const std::string & {
        return name_;
    }

    auto get_expected() -> const std::string & {
        return expected_;
    }

    auto get_pass() -> bool {
        return pass_;
    }

    auto set_pass(bool p) -> void {
        pass_ = p;
    }

    auto check_pass(const std::string &expected) -> bool {
        expected_ = expected;
        pass_ = (expected == s_.str());
        return pass_;
    }

    auto get_stream() -> std::stringstream & {
        return s_;
    }

    auto start_clock() -> void {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    auto stop_clock() -> void {
        end_time_ = std::chrono::high_resolution_clock::now();
    }

    auto get_elapsed() -> std::chrono::high_resolution_clock::duration {
        return end_time_ - start_time_;
    }

private:
    std::string name_;                  // Test name
    std::string expected_;              // Expected result
    bool pass_;                         // Did this test pass?
    std::chrono::high_resolution_clock::time_point start_time_;
                                        // Start time
    std::chrono::high_resolution_clock::time_point end_time_;
                                        // End time
    std::stringstream s_;               // Output string stream
};

typedef result (*test)();

#endif // __RESULT_H

