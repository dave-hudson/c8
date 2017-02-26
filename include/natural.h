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

    /*
     * Zero an array of digits.
     */
    auto inline zero_digit_array(natural_digit *p, std::size_t num_digits) -> void {
        if (num_digits & 1) {
            num_digits--;
            *p++ = 0;
        }

        while (num_digits) {
            num_digits -= 2;
            *p++ = 0;
            *p++ = 0;
        }
    }

    /*
     * Copy an array of digits.
     */
    auto inline copy_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_digits) -> void {
        if (num_digits & 1) {
            num_digits--;
            *dest++ = *src++;
        }

        while (num_digits) {
            num_digits -= 2;
            *dest++ = *src++;
            *dest++ = *src++;
        }
    }

    /*
     * Reverse copy an array of digits.
     */
    auto inline rcopy_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_digits) -> void {
        dest += num_digits;
        src += num_digits;

        if (num_digits & 1) {
            num_digits--;
            *--dest = *--src;
        }

        while (num_digits) {
            num_digits -= 2;
            *--dest = *--src;
            *--dest = *--src;
        }
    }

    /*
     * Shift a digit array left by a whole number of digits.
     */
    inline auto shift_left_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, std::size_t shift_count) -> void {
        zero_digit_array(dest, shift_count);
        copy_digit_array(&dest[shift_count], src, num_src_digits);
    }

    /*
     * Shift a digit array left by a whole number of digits.
     */
    inline auto shift_left_digit_array(natural_digit *digits, std::size_t num_digits, std::size_t shift_count) -> void {
        rcopy_digit_array(&digits[shift_count], digits, num_digits);
        zero_digit_array(digits, shift_count);
    }

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

        /*
         * Return the number of bits required to represent this natural number.
         */
        auto count_bits() const noexcept -> unsigned int {
            /*
             * If we have no digits then this is a simple (special) case.
             */
            std::size_t this_sz = num_digits_;
            if (C8_UNLIKELY(this_sz == 0)) {
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
        auto operator <<(unsigned int count) const -> natural;
        auto operator <<=(unsigned int count) -> natural &;
        auto operator >>(unsigned int count) const -> natural;
        auto operator >>=(unsigned int count) -> natural &;
        auto operator *(natural_digit v) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto operator *=(natural_digit v) -> natural &;
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
            return dm.second;
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

        /*
         * Compare this natural number with another one.
         */
        auto compare(const natural &v) const noexcept -> comparison {
            std::size_t this_sz = num_digits_;
            std::size_t v_sz = v.num_digits_;

            /*
             * If our sizes differ then this is really easy!
             */
            if (C8_UNLIKELY(this_sz > v_sz)) {
                return comparison::gt;
            }

            if (C8_UNLIKELY(this_sz < v_sz)) {
                return comparison::lt;
            }

            const natural_digit *this_digits = digits_;
            const natural_digit *v_digits = v.digits_;

            /*
             * Our sizes are the same so do digit-by-digit comparisons.
             */
            std::size_t i = this_sz;
            while (i--) {
                auto a = this_digits[i];
                auto b = v_digits[i];
                if (a > b) {
                    return comparison::gt;
                }

                if (a < b) {
                    return comparison::lt;
                }
            }

            return comparison::eq;
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
        bool delete_on_final_;              // Do we need to delete digits_ on finalizing?
        natural_digit small_digits_[16];    // Small fixed-size digit buffer

        /*
         * Delete digits array if it is marked for deletion.
         */
        auto delete_digits() -> void {
            if (C8_UNLIKELY(delete_on_final_)) {
                delete[] digits_;
            }
        }

        /*
         * Reserve a number of digits in this natural number.
         */
        auto reserve(std::size_t new_digits) -> void {
            if (C8_LIKELY(digits_size_ >= new_digits)) {
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
            if (C8_LIKELY(digits_size_ >= new_digits)) {
                return;
            }

            auto d = new natural_digit[new_digits];
            copy_digit_array(d, digits_, num_digits_);

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
            if (C8_UNLIKELY(!num_digits_)) {
                return;
            }

            reserve(v.num_digits_);
            copy_digit_array(digits_, v.digits_, num_digits_);
        }

        /*
         * Steal the contents of a natural number into this one.
         */
        auto steal_digits(natural &v) -> void {
            /*
             * Are we currently using the default small buffer?  If we are then we
             * need to deep copy it.
             */
            if (C8_LIKELY(v.digits_ == v.small_digits_)) {
                copy_digit_array(small_digits_, v.small_digits_, v.num_digits_);
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

        /*
         * Shift by an integer number of digits, assuming that our number is non-zero.
         */
        auto shift_left_digits(std::size_t count) const -> natural {
            std::size_t this_sz = num_digits_;
            std::size_t new_sz = this_sz + count;

            natural res;
            res.reserve(new_sz);

            res.num_digits_ = new_sz;
            shift_left_digit_array(res.digits_, digits_, this_sz, count);

            return res;
        }

        /*
         * Shift by an integer number of digits, assuming that our number is non-zero,
         * and write the result back to this number.
         */
        auto shift_left_digits_this(std::size_t count) -> natural & {
            std::size_t this_sz = num_digits_;
            std::size_t new_sz = this_sz + count;

            expand(new_sz);

            num_digits_ = new_sz;
            shift_left_digit_array(digits_, this_sz, count);

            return *this;
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

