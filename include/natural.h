/*
 * natural.h
 */
#ifndef __C8_NATURAL_H
#define __C8_NATURAL_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace c8 {
    /*
     * Define the concepts of a "digit" and a "double digit" (twice the size of a digit.
     * A digit is much larger than a numeral, but the algorithms we use for things like
     * addition, subtraction, multiplication and division all work just the same, whether
     * our digits are 1 bit, 32 bits or range from 0 to 9.
     *
     * Other versions that would work here:
     *
     * typedef uint16_t natural_digit;
     * typedef uint32_t natural_double_digit;
     *
     * Or:
     *
     * typedef uint8_t natural_digit;
     * typedef uint16_t natural_double_digit;
     */
    typedef uint32_t natural_digit;
    typedef uint64_t natural_double_digit;

    enum class natural_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class natural {
    public:
        /*
         * Constructors.
         */
        natural() {
        }

        natural(unsigned long long v);
        natural(const std::string &v);
        natural(const natural &v) = default;
        natural(natural &&v) = default;
        ~natural() = default;
        auto operator =(const natural &v) -> natural & = default;
        auto operator =(natural &&v) -> natural & = default;

        auto count_bits() -> unsigned int;
        auto add(const natural &v) const -> natural;
        auto subtract(const natural &v) const -> natural;
        auto shiftr(unsigned int count) const -> natural;
        auto shiftl(unsigned int count) const -> natural;
        auto multiply(const natural &v) const -> natural;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto divide(const natural &v) const -> natural;
        auto modulus(const natural &v) const -> natural;
        auto compare(const natural &v) const -> natural_comparison;
        auto normalize() -> void;
        friend auto operator<<(std::ostream &outstr, const natural &v) -> std::ostream &;

        auto operator+(const natural &v) const -> natural {
            return add(v);
        }

        auto operator+=(const natural &v) {
            *this = add(v);
        }

        auto operator-(const natural &v) const -> natural {
            return subtract(v);
        }

        auto operator-=(const natural &v) {
            *this = subtract(v);
        }

        auto operator>>(unsigned int count) const -> natural {
            return shiftr(count);
        }

        auto operator>>=(unsigned int count) {
            *this = shiftr(count);
        }

        auto operator<<(unsigned int count) const -> natural {
            return shiftl(count);
        }

        auto operator<<=(unsigned int count) {
            *this = shiftl(count);
        }

        auto operator*(const natural &v) const -> natural {
            return multiply(v);
        }

        auto operator*=(const natural &v) {
            *this = multiply(v);
        }

        auto operator/(const natural &v) const -> natural {
            return divide(v);
        }

        auto operator/=(const natural &v) {
            *this = divide(v);
        }

        auto operator%(const natural &v) const -> natural {
            return modulus(v);
        }

        auto operator%=(const natural &v) {
            *this = modulus(v);
        }

        auto operator==(const natural &v) const -> bool {
            return compare(v) == natural_comparison::eq;
        }

        auto operator!=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::eq;
        }

        auto operator>(const natural &v) const -> bool {
            return compare(v) == natural_comparison::gt;
        }

        auto operator>=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::lt;
        }

        auto operator<(const natural &v) const -> bool {
            return compare(v) == natural_comparison::lt;
        }

        auto operator<=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::gt;
        }

    private:
        std::vector<natural_digit> digits_; // Digits of the natural number
    };
}

#endif /* __C8_NATURAL_H */

