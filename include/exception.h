/*
 * exception.h
 */
#ifndef __C8_EXCEPTION_H
#define __C8_EXCEPTION_H

namespace c8 {
    /*
     * Exception class to signal an invalid argument.
     */
    class invalid_argument : public std::invalid_argument {
    public:
        invalid_argument(const std::string &s) : std::invalid_argument(s) {
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
}

#endif /* __C8_EXCEPTION_H */

