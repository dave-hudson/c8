/*
 * exception.h
 */
#ifndef __C8_EXCEPTION_H
#define __C8_EXCEPTION_H

/*
 * Macros to allow the compiler to make better choices about code structure.
 */
#define C8_LIKELY(x) __builtin_expect(!!(x), 1)
#define C8_UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace c8 {
    /*
     * Comparison enum class.
     */
    enum class comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    /*
     * Exception class to signal an invalid argument.
     */
    class invalid_argument : public std::invalid_argument {
    public:
        invalid_argument(const std::string &s) : std::invalid_argument(s) {
        }
    };

    /*
     * Exception class to signal an overflow.
     */
    class overflow_error : public std::overflow_error {
    public:
        overflow_error() : std::overflow_error("overflow error") {
        }
    };

    /*
     * Exception class to signal an underflow.
     */
    class underflow_error : public std::underflow_error {
    public:
        underflow_error() : std::underflow_error("underflow error") {
        }
    };

    /*
     * Exception class to signal divide-by-zero.
     */
    class divide_by_zero : public std::runtime_error {
    public:
        divide_by_zero() : std::runtime_error("divide by zero") {
        }
    };

    /*
     * Exception class to signal not a number.
     */
    class not_a_number : public std::runtime_error {
    public:
        not_a_number() : std::runtime_error("not a number") {
        }
    };
}

#endif //

