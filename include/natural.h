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
            delete_on_final_ = false;
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
            if (this == &v) {
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
            if (this == &v) {
                return *this;
            }

            /*
             * Delete the old contents of this natural number and steal the original's digits.
             */
            delete_digits();
            steal_digits(v);
            return *this;
        }

        /*
         * Return the number of bits required to represent this natural number.
         */
        auto count_bits() const noexcept -> unsigned int {
            /*
             * If we have no digits then this is a simple (special) case.
             */
            std::size_t this_sz = num_digits_;
            if (this_sz == 0) {
                return 0;
            }

            /*
             * We can account for trailing digits easily, but the most significant digit is
             * more tricky.  We use __builtin_clz() to count the leadign zeros of the digit,
             * but if the size of a digit is smaller than the size of an integer (which is
             * what __builtin_clz() uses) then we must compensate for the extra zeros that
             * it returns.
             */
            natural_digit d = digits_[this_sz - 1];
            auto c = (sizeof(int) / sizeof(natural_digit)) - 1;
            return static_cast<unsigned int>((this_sz + c) * natural_digit_bits) - static_cast<unsigned int>(__builtin_clz(d));
        }

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
        auto compare(const natural &v) const noexcept -> comparison;
        auto gcd(const natural &v) const -> natural;
        auto to_unsigned_long_long() const -> unsigned long long;
        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

        auto is_zero() const noexcept -> bool {
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
        bool delete_on_final_;              // Do we need to delete digits_ on finalizing?
        natural_digit *digits_;             // Digits of the natural number
        std::size_t digits_size_;           // Number of digits_ allocated
        std::size_t num_digits_;            // The number of digits
        natural_digit small_digits_[16];    // Small fixed-size digit buffer

        /*
         * Delete digits array if it is marked for deletion.
         */
        auto delete_digits() -> void {
            if (delete_on_final_) {
                delete[] digits_;
            }
        }

        /*
         * Reserve a number of digits in this natural number.
         */
        auto reserve(std::size_t new_digits) -> void {
            if (digits_size_ >= new_digits) {
                return;
            }

            digits_size_ = new_digits;
            delete_on_final_ = true;
            digits_ = new natural_digit[new_digits];
        }

        /*
         * Expand the number of digits in this natural number.
         */
        auto expand(std::size_t new_digits) -> void {
            if (digits_size_ >= new_digits) {
                return;
            }

            auto d = new natural_digit[new_digits];
            std::memcpy(d, digits_, sizeof(natural_digit) * num_digits_);

            delete_digits();
            digits_size_ = new_digits;
            delete_on_final_ = true;
            digits_ = d;
        }

        /*
         * Copy the contents of a natural number into this one.
         */
        auto copy_digits(const natural &v) -> void {
            digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
            delete_on_final_ = false;
            digits_ = small_digits_;
            num_digits_ = v.num_digits_;
            if (!num_digits_) {
                return;
            }

            reserve(v.num_digits_);
            std::memcpy(digits_, v.digits_, sizeof(natural_digit) * num_digits_);
        }

        /*
         * Steal the contents of a natural number into this one.
         */
        auto steal_digits(natural &v) -> void {
            /*
             * Are we currently using the default small buffer?  If we are then we
             * need to deep copy it.
             */
            if (v.digits_ == v.small_digits_) {
                std::memcpy(small_digits_, v.small_digits_, sizeof(natural_digit) * v.num_digits_);
                digits_ = small_digits_;
            } else {
                /*
                 * We aren't using the default buffer so we can shallow copy instead.
                 */
                digits_ = v.digits_;
            }

            num_digits_ = v.num_digits_;
            v.num_digits_ = 0;
            digits_size_ = v.digits_size_;
            v.digits_size_ = sizeof(v.small_digits_) / sizeof(natural_digit);
            v.digits_ = v.small_digits_;
            delete_on_final_ = v.delete_on_final_;
            v.delete_on_final_ = false;
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

#endif /* __C8_NATURAL_H */

