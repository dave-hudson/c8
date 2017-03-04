/*
 * natural.h
 */
#ifndef __C8_NATURAL_H
#define __C8_NATURAL_H

#include <cstring>
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

    const std::size_t natural_digit_bits = 8 * sizeof(natural_digit);

    class natural {
    public:
        /*
         * Constructors.
         */
        natural() noexcept {
            num_digits_ = 0;
            digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
            digits_ = small_digits_;
            delete_digits_on_final_ = false;
        }

        natural(unsigned long long v);
        natural(const std::string &v);

        /*
         * Copy constructor.
         */
        natural(const natural &v) {
            copy_digits(v);
        }

        /*
         * Move constructor.
         */
        natural(natural &&v) noexcept {
            steal_digits(v);
        }

        /*
         * Destructor.
         */
        ~natural() {
            delete_digits();
        }

        /*
         * Copy assignment operator.
         */
        auto operator =(const natural &v) -> natural & {
            /*
             * Are we assigning to ourself?  If we are then we don't need to do anything.
             */
            if (C8_UNLIKELY(this == &v)) {
                return *this;
            }

            /*
             * Delete the old contents of this natural number and copy the original's digits.
             */
            delete_digits();
            copy_digits(v);
            return *this;
        }

        /*
         * Move assignment operator.
         */
        auto operator =(natural &&v) noexcept -> natural & {
            /*
             * Are we assigning to ourself?  If we are then we don't have to do anything.
             */
            if (C8_UNLIKELY(this == &v)) {
                return *this;
            }

            /*
             * Delete the old contents of this natural number and steal the original's digits.
             */
            delete_digits();
            steal_digits(v);
            return *this;
        }

        auto count_bits() const noexcept -> unsigned int;
        auto compare(const natural &v) const noexcept -> comparison;
        auto operator +(natural_digit v) const -> natural;
        auto operator +(const natural &v) const -> natural;
        auto operator +=(natural_digit v) -> natural &;
        auto operator +=(const natural &v) -> natural &;
        auto operator -(natural_digit v) const -> natural;
        auto operator -(const natural &v) const -> natural;
        auto operator -=(natural_digit v) -> natural &;
        auto operator -=(const natural &v) -> natural &;
        auto operator <<(unsigned int count) const -> natural;
        auto operator <<=(unsigned int count) -> natural &;
        auto operator >>(unsigned int count) const -> natural;
        auto operator >>=(unsigned int count) -> natural &;
        auto operator *(natural_digit v) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto operator *=(natural_digit v) -> natural &;
        auto operator /(natural_digit v) const -> natural;
        auto operator /(const natural &v) const -> natural;
        auto operator %(natural_digit v) const -> natural_digit;
        auto operator %(const natural &v) const -> natural;
        auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto gcd(const natural &v) const -> natural;
        auto to_unsigned_long_long() const -> unsigned long long;
        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

        /*
         * Is this number zero?
         */
        auto is_zero() const noexcept -> bool {
            return (num_digits_ == 0) ? true : false;
        }

        auto operator *=(const natural &v) -> natural & {
            *this = *this * v;
            return *this;
        }

        auto operator /=(natural_digit v) -> natural & {
            *this = *this / v;
            return *this;
        }

        auto operator /=(const natural &v) -> natural & {
            *this = *this / v;
            return *this;
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
        std::size_t num_digits_;            // The number of digits in this number
        std::size_t digits_size_;           // Number of digits_ allocated
        natural_digit *digits_;             // Digits of the natural number
        bool delete_digits_on_final_;       // Do we need to delete digits_ on finalizing?
        natural_digit small_digits_[16];    // Small fixed-size digit buffer

        auto reserve(std::size_t new_digits) -> void;
        auto expand(std::size_t new_digits) -> void;
        auto copy_digits(const natural &v) -> void;
        auto steal_digits(natural &v) -> void;

        /*
         * Delete digits array if it is marked for deletion.
         */
        auto delete_digits() -> void {
            if (C8_UNLIKELY(delete_digits_on_final_)) {
                delete[] digits_;
            }
        }
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

#endif // __C8_NATURAL_H

