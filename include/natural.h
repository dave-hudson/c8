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

    class natural {
    public:
        /*
         * Constructors.
         */
        natural() {
            num_digits_ = 0;
            digits_size_ = 0;
            digits_ = nullptr;
        }

        natural(unsigned long long v);
        natural(const std::string &v);
        natural(const natural &v);
        natural(natural &&v);
        ~natural();
        auto operator =(const natural &v) -> natural &;
        auto operator =(natural &&v) -> natural &;

        auto count_bits() const -> unsigned int;
        auto operator +(natural_digit v) const -> natural;
        auto operator +(const natural &v) const -> natural;
        auto operator +=(natural_digit v) -> natural &;
        auto operator +=(const natural &v) -> natural &;
        auto operator -(natural_digit v) const -> natural;
        auto operator -(const natural &v) const -> natural;
        auto operator -=(natural_digit v) -> natural &;
        auto operator -=(const natural &v) -> natural &;
        auto operator >>(unsigned int count) const -> natural;
        auto operator <<(unsigned int count) const -> natural;
        auto operator *(natural_digit v) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto operator *=(natural_digit v) -> natural &;
        auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto compare(const natural &v) const -> comparison;
        auto gcd(const natural &v) const -> natural;
        auto to_unsigned_long_long() const -> unsigned long long;
        auto reserve(std::size_t new_digits) -> void;
        auto expand(std::size_t new_digits) -> void;
        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

        auto is_zero() const -> bool {
            return (num_digits_ == 0) ? true : false;
        }

        auto operator >>=(unsigned int count) -> natural & {
            *this = *this >> count;
            return *this;
        }

        auto operator <<=(unsigned int count) -> natural & {
            *this = *this << count;
            return *this;
        }

        auto operator *=(const natural &v) -> natural & {
            *this = *this * v;
            return *this;
        }

        auto operator /(natural_digit v) const -> natural {
            std::pair<natural, natural_digit> dm = divide_modulus(v);
            return std::move(dm.first);
        }

        auto operator /(const natural &v) const -> natural {
            std::pair<natural, natural> dm = divide_modulus(v);
            return std::move(dm.first);
        }

        auto operator /=(natural_digit v) -> natural & {
            *this = *this / v;
            return *this;
        }

        auto operator /=(const natural &v) -> natural & {
            *this = *this / v;
            return *this;
        }

        auto operator %(natural_digit v) const -> natural_digit {
            std::pair<natural, natural_digit> dm = divide_modulus(v);
            return std::move(dm.second);
        }

        auto operator %(const natural &v) const -> natural {
            std::pair<natural, natural> dm = divide_modulus(v);
            return std::move(dm.second);
        }

        auto operator %=(natural_digit v) -> natural & {
            *this = *this % v;
            return *this;
        }

        auto operator %=(const natural &v) -> natural & {
            *this = *this % v;
            return *this;
        }

        auto operator ==(const natural &v) const -> bool {
            return compare(v) == comparison::eq;
        }

        auto operator !=(const natural &v) const -> bool {
            return compare(v) != comparison::eq;
        }

        auto operator >(const natural &v) const -> bool {
            return compare(v) == comparison::gt;
        }

        auto operator >=(const natural &v) const -> bool {
            return compare(v) != comparison::lt;
        }

        auto operator <(const natural &v) const -> bool {
            return compare(v) == comparison::lt;
        }

        auto operator <=(const natural &v) const -> bool {
            return compare(v) != comparison::gt;
        }

    private:
        natural_digit *digits_;             // Digits of the natural number
        std::size_t digits_size_;           // Number of digits_ allocated
        std::size_t num_digits_;            // The number of digits
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

