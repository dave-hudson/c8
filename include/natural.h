/*
 * natural.h
 */
#ifndef __C8_NATURAL_H
#define __C8_NATURAL_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "exception.h"

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
        natural(const natural &v);
        natural(natural &&v);
        ~natural();
        auto operator =(const natural &v) -> natural &;
        auto operator =(natural &&v) -> natural &;

        auto count_bits() const -> unsigned int;
        auto operator +(const natural &v) const -> natural;
        auto operator -(const natural &v) const -> natural;
        auto operator >>(unsigned int count) const -> natural;
        auto operator <<(unsigned int count) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto compare(const natural &v) const -> natural_comparison;
        auto gcd(const natural &v) const -> natural;
        auto to_unsigned_long_long() const -> unsigned long long;
        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

        auto is_zero() const -> bool {
            return (digits_.size() == 0) ? true : false;
        }

        auto operator +=(const natural &v) {
            *this = *this + v;
        }

        auto operator -=(const natural &v) {
            *this = *this - v;
        }

        auto operator >>=(unsigned int count) {
            *this = *this >> count;
        }

        auto operator <<=(unsigned int count) {
            *this = *this << count;
        }

        auto operator *=(const natural &v) {
            *this = *this * v;
        }

        auto operator /(const natural &v) const -> natural {
            std::pair<natural, natural> dm = divide_modulus(v);
            return std::move(dm.first);
        }

        auto operator /=(const natural &v) {
            *this = *this / v;
        }

        auto operator %(const natural &v) const -> natural {
            std::pair<natural, natural> dm = divide_modulus(v);
            return std::move(dm.second);
        }

        auto operator %=(const natural &v) {
            *this = *this % v;
        }

        auto operator ==(const natural &v) const -> bool {
            return compare(v) == natural_comparison::eq;
        }

        auto operator !=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::eq;
        }

        auto operator >(const natural &v) const -> bool {
            return compare(v) == natural_comparison::gt;
        }

        auto operator >=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::lt;
        }

        auto operator <(const natural &v) const -> bool {
            return compare(v) == natural_comparison::lt;
        }

        auto operator <=(const natural &v) const -> bool {
            return compare(v) != natural_comparison::gt;
        }

    private:
        std::vector<natural_digit> digits_; // Digits of the natural number

        auto normalize() -> void;
    };

    inline auto is_zero(const natural &v) -> bool {
        return v.is_zero();
    }

    inline auto gcd(const natural &v, const natural &u) -> natural {
        return v.gcd(u);
    }

    inline auto to_unsigned_long_long(const natural &v) -> unsigned long long {
        return v.to_unsigned_long_long();
    }
}

#endif /* __C8_NATURAL_H */

