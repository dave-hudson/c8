/*
 * digit_array.h
 */
#ifndef __C8___DIGIT_ARRAY_H
#define __C8___DIGIT_ARRAY_H

#include "c8.h"

#define C8_USE_ASM 1

namespace c8 {
    /*
     * Return the number of bits actually used within this digit array.
     */
    inline auto __digit_array_size_bits(const natural_digit *p, std::size_t p_num_digits) noexcept -> std::size_t {
        /*
         * If we have no digits then this is a simple (special) case.
         */
        if (C8_UNLIKELY(p_num_digits == 0)) {
            return 0;
        }

        auto d = p[p_num_digits - 1];

        /*
         * This if statement is actually a bit strange.  It's comparing two things that
         * are compile-time constants because we actually only want one of the following
         * code paths to be retained.
         */
        if (sizeof(natural_digit) <= sizeof(int)) {
            /*
             * We can account for trailing digits easily, but the most significant digit is
             * more tricky.  We use __builtin_clz() to count the leading zeros of the digit,
             * but if the size of a digit is smaller than the size of an integer (which is
             * what __builtin_clz() uses) then we must compensate for the extra zeros that
             * it returns.
             */
            auto clz = static_cast<unsigned int>(__builtin_clz(static_cast<unsigned int>(d)));
            auto c = (sizeof(int) / sizeof(natural_digit)) - 1;
            return static_cast<std::size_t>((p_num_digits + c) * natural_digit_bits) - clz;
        }

        auto clz = static_cast<unsigned int>(__builtin_clzll(static_cast<unsigned long long>(d)));
        return static_cast<std::size_t>(p_num_digits * natural_digit_bits) - clz;
    }

    /*
     * Zero an array of digits.
     */
    inline auto __digit_array_zero(natural_digit *p, std::size_t p_num_digits) -> void {
        if (p_num_digits & 1) {
            p_num_digits--;
            *p++ = 0;
        }

        while (p_num_digits) {
            p_num_digits -= 2;
            *p++ = 0;
            *p++ = 0;
        }
    }

    /*
     * Copy (forwards) an array of digits.
     */
    inline auto __digit_array_copy(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        if (src_num_digits & 1) {
            src_num_digits--;
            *res++ = *src++;
        }

        while (src_num_digits) {
            src_num_digits -= 2;
            *res++ = *src++;
            *res++ = *src++;
        }
    }

    /*
     * Reverse copy an array of digits.
     */
    inline auto __digit_array_rcopy(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        res += src_num_digits;
        src += src_num_digits;

        if (src_num_digits & 1) {
            src_num_digits--;
            *--res = *--src;
        }

        while (src_num_digits) {
            src_num_digits -= 2;
            *--res = *--src;
            *--res = *--src;
        }
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    inline auto __digit_array_compare_eq(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits != src2_num_digits) {
            return false;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            if (src1[i] != src2[i]) {
                return false;
            }
        }

        return true;
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    inline auto __digit_array_compare_ne(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits != src2_num_digits) {
            return true;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            if (src1[i] != src2[i]) {
                return true;
            }
        }

        return false;
    }

    /*
     * Compare if digit array src1 is greater than digit array src2.
     */
    inline auto __digit_array_compare_gt(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits > src2_num_digits) {
            return true;
        }

        if (src1_num_digits < src2_num_digits) {
            return false;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            auto a = src1[i];
            auto b = src2[i];
            if (a > b) {
                return true;
            }

            if (a < b) {
                return false;
            }
        }

        return false;
    }

    /*
     * Compare if digit array src1 is greater than or equal to digit array src2.
     */
    inline auto __digit_array_compare_ge(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits > src2_num_digits) {
            return true;
        }

        if (src1_num_digits < src2_num_digits) {
            return false;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            auto a = src1[i];
            auto b = src2[i];
            if (a > b) {
                return true;
            }

            if (a < b) {
                return false;
            }
        }

        return true;
    }

    /*
     * Compare if digit array src1 is less than digit array src2.
     */
    inline auto __digit_array_compare_lt(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits > src2_num_digits) {
            return false;
        }

        if (src1_num_digits < src2_num_digits) {
            return true;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            auto a = src1[i];
            auto b = src2[i];
            if (a > b) {
                return false;
            }

            if (a < b) {
                return true;
            }
        }

        return false;
    }

    /*
     * Compare if digit array src1 is less than or equal to digit array src2.
     */
    inline auto __digit_array_compare_le(const natural_digit *src1, std::size_t src1_num_digits,
                                         const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits > src2_num_digits) {
            return false;
        }

        if (src1_num_digits < src2_num_digits) {
            return true;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            auto a = src1[i];
            auto b = src2[i];
            if (a > b) {
                return false;
            }

            if (a < b) {
                return true;
            }
        }

        return true;
    }

    /*
     * Add two non-zero single digits.
     */
    inline auto __digit_array_add_1_1(natural_digit *res, std::size_t &res_num_digits,
                                      const natural_digit *src1,
                                      const natural_digit *src2) -> void {
        auto acc = static_cast<natural_double_digit>(src1[0]) + static_cast<natural_double_digit>(src2[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        std::size_t r_num_digits = 1;
        if (C8_UNLIKELY(acc_c)) {
            res[1] = acc_c;
            r_num_digits = 2;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Add a single digit to a digit array.
     *
     * Note: It is OK for res and src1 to be the same pointer.
     */
    inline auto __digit_array_add_m_1(natural_digit *res, std::size_t &res_num_digits,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2) -> void {
        /*
         * Add the first digit.
         */
        auto acc = (static_cast<natural_double_digit>(src1[0]) + static_cast<natural_double_digit>(src2[0]));
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        /*
         * Add the remaining digits and any carries.
         */
        std::size_t r_num_digits = 1;
        do {
            acc = static_cast<natural_double_digit>(acc_c) + static_cast<natural_double_digit>(src1[r_num_digits]);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < src1_num_digits);

        /*
         * We may have a final carry digit, so handle that if it exists.
         */
        if (C8_UNLIKELY(acc_c)) {
            res[r_num_digits++] = acc_c;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto __digit_array_add_m_n(natural_digit *res, std::size_t &res_num_digits,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        const natural_digit *larger;
        const natural_digit *smaller;
        std::size_t larger_num_digits;
        std::size_t smaller_num_digits;
        if (src1_num_digits >= src2_num_digits) {
            larger = src1;
            larger_num_digits = src1_num_digits;
            smaller = src2;
            smaller_num_digits = src2_num_digits;
        } else {
            larger = src2;
            larger_num_digits = src2_num_digits;
            smaller = src1;
            smaller_num_digits = src1_num_digits;
        }

        /*
         * Add the first digits together.
         */
        auto acc = static_cast<natural_double_digit>(larger[0]) + static_cast<natural_double_digit>(smaller[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        std::size_t r_num_digits = 1;
        do {
            acc = (static_cast<natural_double_digit>(larger[r_num_digits]) + static_cast<natural_double_digit>(smaller[r_num_digits]));
            acc += static_cast<natural_double_digit>(acc_c);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < smaller_num_digits);

        /*
         * Add any carries.
         */
        while (r_num_digits < larger_num_digits) {
            acc = static_cast<natural_double_digit>(larger[r_num_digits]);
            acc += static_cast<natural_double_digit>(acc_c);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        }

        /*
         * We may have a final carry digit, so handle that if it exists.
         */
        if (C8_UNLIKELY(acc_c)) {
            res[r_num_digits++] = acc_c;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Subtract a single digit from another single digit.
     *
     * The value of src1 must be larger than, or equal to, src2.
     */
    inline auto __digit_array_subtract_1_1(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1,
                                           const natural_digit *src2) -> void {
        std::size_t r_num_digits = 0;

        auto acc = src1[0] - src2[0];
        if (acc) {
            res[0] = acc;
            r_num_digits = 1;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Subtract a single digit from a digit array.
     *
     * Note: It is OK for res and src1 to be the same pointer.
     * The value of src1 must be larger than, or equal to, src2.
     */
    inline auto __digit_array_subtract_m_1(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2) -> void {
        /*
         * Subtract the first digit.
         */
        auto acc = (static_cast<natural_double_digit>(src1[0]) - static_cast<natural_double_digit>(src2[0]));
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        /*
         * Subtract the remaining digits and any carries.
         */
        std::size_t r_num_digits = 1;
        do {
            acc = static_cast<natural_double_digit>(src1[r_num_digits]) - static_cast<natural_double_digit>(acc_c & 1);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < src1_num_digits);

        /*
         * We may have a zero upper digit so account for this.
         */
        if (!res[r_num_digits - 1]) {
            r_num_digits--;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     * The value of src1 must be larger than, or equal to, src2.
     */
    inline auto __digit_array_subtract_m_n(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Subtract the first digit.
         */
        auto acc = static_cast<natural_double_digit>(src1[0]) - static_cast<natural_double_digit>(src2[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        /*
         * Subtract the remaining digits and any carries.
         */
        std::size_t r_num_digits = 1;
        do {
            acc = static_cast<natural_double_digit>(src1[r_num_digits]) - static_cast<natural_double_digit>(src2[r_num_digits]);
            acc -= static_cast<natural_double_digit>(acc_c & 1);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < src2_num_digits);

        /*
         * Subtract any carries.
         */
        while (r_num_digits < src1_num_digits) {
            acc = static_cast<natural_double_digit>(src1[r_num_digits]);
            acc -= static_cast<natural_double_digit>(acc_c & 1);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        }

        /*
         * Calculate the number of resulting digits.  With subtraction there's no
         * easy way to estimate this.
         */
        while (r_num_digits--) {
            if (res[r_num_digits]) {
                break;
            }
        }

        res_num_digits = r_num_digits + 1;
    }

    /*
     * Left shift a digit.
     */
    inline auto __digit_array_left_shift_1(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src,
                                           std::size_t shift_digits, std::size_t shift_bits) -> void {
        std::size_t r_num_digits = 1 + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            res[shift_digits] = src[0];
            __digit_array_zero(res, shift_digits);
            res_num_digits = r_num_digits;
            return;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        auto d = src[0];
        res[shift_digits] = d << shift_bits;
        auto d_hi = d >> (natural_digit_bits - shift_bits);
        if (d_hi) {
            res[r_num_digits++] = d_hi;
        }

        __digit_array_zero(res, shift_digits);
        res_num_digits = r_num_digits;
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __digit_array_left_shift_m(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src, std::size_t src_num_digits,
                                           std::size_t shift_digits, std::size_t shift_bits) -> void {
        std::size_t r_num_digits = src_num_digits + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            __digit_array_rcopy(&res[shift_digits], src, src_num_digits);
            __digit_array_zero(res, shift_digits);
            res_num_digits = r_num_digits;
            return;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        auto d = src[src_num_digits - 1];
        auto d_hi = d >> (natural_digit_bits - shift_bits);
        if (d_hi) {
            res[r_num_digits++] = d_hi;
        }

        for (std::size_t i = src_num_digits - 1; i > 0; i--) {
            d_hi = d << shift_bits;
            d = src[i - 1];
            res[i + shift_digits] = d_hi | (d >> (natural_digit_bits - shift_bits));
        }

        res[shift_digits] = d << shift_bits;
        __digit_array_zero(res, shift_digits);
        res_num_digits = r_num_digits;
    }

    /*
     * Right shift a digit.
     */
    inline auto __digit_array_right_shift_1(natural_digit *res, std::size_t &res_num_digits,
                                            const natural_digit *src, std::size_t shift_bits) -> void {
        std::size_t r_num_digits = 0;

        auto r = src[0] >> shift_bits;
        if (r) {
            res[0] = r;
            r_num_digits = 1;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __digit_array_right_shift_m(natural_digit *res, std::size_t &res_num_digits,
                                            const natural_digit *src, std::size_t src_num_digits,
                                            std::size_t shift_digits, std::size_t shift_bits) -> void {
        std::size_t r_num_digits = src_num_digits - (shift_digits + 1);

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            r_num_digits++;
            __digit_array_copy(res, &src[shift_digits], r_num_digits);
            res_num_digits = r_num_digits;
            return;
        }

        /*
         * Shift the original value and insert in the result.
         */
        auto d_lo = src[shift_digits] >> shift_bits;
        for (std::size_t i = 1; i <= r_num_digits; i++) {
            auto d = src[i + shift_digits];
            res[i - 1] = d_lo | (d << (natural_digit_bits - shift_bits));
            d_lo = d >> shift_bits;
        }

        if (d_lo) {
            res[r_num_digits++] = d_lo;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Multiply two non-zero single digits.
     */
    inline auto __digit_array_multiply_1_1(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1,
                                           const natural_digit *src2) -> void {
        auto acc = static_cast<natural_double_digit>(src1[0]) * static_cast<natural_double_digit>(src2[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        std::size_t r_num_digits = 1;
        if (acc_c) {
            res[1] = acc_c;
            r_num_digits = 2;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Multiply a digit array by a single digit.
     *
     * Note: It is OK for res and src1 to be the same pointer.
     */
    inline auto __digit_array_multiply_m_1(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2) -> void {
        auto v = src2[0];

        /*
         * Long multiply.  Start with the first digit.
         */
        auto acc = static_cast<natural_double_digit>(src1[0]) * static_cast<natural_double_digit>(v);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[0] = static_cast<natural_digit>(acc);

        /*
         * Multiply the remaining digits. 
         */
        std::size_t r_num_digits = 1;
        do {
            acc = static_cast<natural_double_digit>(src1[r_num_digits]) * static_cast<natural_double_digit>(v);
            acc += static_cast<natural_double_digit>(acc_c);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < src1_num_digits);

        if (acc_c) {
            res[r_num_digits++] = acc_c;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto __digit_array_multiply_m_n(natural_digit *res, std::size_t &res_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * If we're going to update in place then we actually have to copy the source array
         * because we'll overwrite it.
         */
        auto src1_1 = src1;
        natural_digit src1_copy[src1_num_digits];
        if (res == src1) {
            __digit_array_copy(src1_copy, src1, src1_num_digits);
            src1_1 = src1_copy;
        }

        /*
         * Comba multiply.
         *
         * In this style of multplier we work out all of the multplies that contribute to
         * a single result digit at the same time.
         *
         * When we multiply two digits together we get a double digit result.  We track
         * the upper digit via an accumulator, acc1, along with all previous accumulated
         * carries.
         */
        natural_double_digit acc1 = 0;
        std::size_t tj_lim = src2_num_digits - 1;

        std::size_t r_num_digits = src1_num_digits + src2_num_digits - 1;

        for (std::size_t res_column = 0; res_column < r_num_digits; res_column++) {
            std::size_t tj = (res_column < tj_lim) ? res_column : tj_lim;
            std::size_t ti = res_column - tj;
            std::size_t num_multiplies = ((src1_num_digits - ti) < (tj + 1)) ? (src1_num_digits - ti) : (tj + 1);
            auto acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(acc1));
            acc1 >>= natural_digit_bits;
            for (std::size_t j = 0; j < num_multiplies; j++) {
                auto a = static_cast<natural_double_digit>(src1_1[ti++]);
                auto b = static_cast<natural_double_digit>(src2[tj--]);
                auto d0 = acc0 + (a * b);
                acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(d0));
                acc1 += (d0 >> natural_digit_bits);
            }

            res[res_column] = static_cast<natural_digit>(acc0);
        }

        /*
         * We may have an extra upper digit, so account for this.
         */
        auto c = static_cast<natural_digit>(acc1);
        if (c) {
            res[r_num_digits++] = c;
        }

        res_num_digits = r_num_digits;
    }

    /*
     * Divide/modulus a single digit by another single digit.
     */
    inline auto __digit_array_divide_modulus_1_1(natural_digit *quotient, std::size_t &quotient_num_digits,
                                                 natural_digit *remainder, std::size_t &remainder_num_digits,
                                                 const natural_digit *src1,
                                                 const natural_digit *src2) -> void {
        auto v1 = src1[0];
        auto v2 = src2[0];

        auto q = v1 / v2;
        auto r = v1 % v2;

        std::size_t r_num_digits = 0;

        if (r) {
            remainder[0] = r;
            r_num_digits = 1;
        }

        remainder_num_digits = r_num_digits;

        std::size_t q_num_digits = 0;
        if (q) {
            quotient[0] = q;
            q_num_digits = 1;
        }

        quotient_num_digits = q_num_digits;
    }

    /*
     * Divide/modulus a digit array by a single digit.
     *
     * Note: It is OK for res and src1 to be the same pointer.
     */
    inline auto __digit_array_divide_modulus_m_1(natural_digit *quotient, std::size_t &quotient_num_digits,
                                                 natural_digit *remainder, std::size_t &remainder_num_digits,
                                                 const natural_digit *src1, std::size_t src1_num_digits,
                                                 const natural_digit *src2) -> void {
        auto v = src2[0];

        /*
         * Long divide.  Start with the most significant digit.
         */
        std::size_t q_num_digits = src1_num_digits;
        std::size_t i = src1_num_digits - 1;

        auto d = src1[i];
        auto q = d / v;
        auto r = d % v;

        quotient[i] = q;
        if (q == 0) {
            q_num_digits--;
        }

        quotient_num_digits = q_num_digits;

        /*
         * Loop over all the remaining digits.
         */
        while (i--) {
            q = src1[i];

            /*
             * We need to do a divide, but a large natural_double_digit will typically be
             * more than a single machine register in size.  The compiler typically won't be
             * able to generate the most optimal code in such cases so we have to resort to
             * inline assembler.
             *
             * The do/while loop here isn't actually a loop - it just lets us use break to
             * early-out.  The compiler will invariably recognize that only one path is ever
             * taken and optimize everything else away.
             */
            do {
                if (sizeof(natural_digit) == 8) {
#if defined(C8_USE_ASM) && defined(__x86_64__)
                    asm volatile (
                    "       divq    %[v]                                \n\t"
                            : [q] "+a" (q), [r] "+d" (r)
                            : [v] "r" (v)
                            : "cc"
                    );

                    break;
#endif
                }

                auto acc = (static_cast<natural_double_digit>(r) << natural_digit_bits) + static_cast<natural_double_digit>(q);
                q = static_cast<natural_digit>(acc / v);
                r = static_cast<natural_digit>(acc % v);
            } while (false);

            quotient[i] = q;
        }

        std::size_t r_num_digits = 0;
        if (r) {
            remainder[0] = r;
            r_num_digits = 1;
        }

        remainder_num_digits = r_num_digits;
    }

    /*
     * Subtract a shifted digit array from another.
     *
     * Returns true if the subtraction underflows, false if it does not.
     *
     * Unlike the more generic subtract operation above, this one is specifically only for use with
     * the divide/modulus operation.  This handles underflowing subtraction.
     */
    inline auto __digit_array_subtract_m_n_shifted(natural_digit *res, std::size_t res_num_digits,
                                                   const natural_digit *src2, std::size_t src2_num_digits,
                                                   std::size_t src2_shift) -> bool {
        /*
         * Subtract the first digit.
         */
        auto acc = static_cast<natural_double_digit>(res[src2_shift]) - static_cast<natural_double_digit>(src2[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[src2_shift] = static_cast<natural_digit>(acc);

        /*
         * Subtract the remaining digits.
         */
        std::size_t r_num_digits = src2_shift + 1;
        do {
            acc = static_cast<natural_double_digit>(res[r_num_digits]) - static_cast<natural_double_digit>(src2[r_num_digits - src2_shift]);
            acc -= static_cast<natural_double_digit>(acc_c & 1);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < (src2_num_digits + src2_shift));

        /*
         * Subtract any carries.
         */
        while (r_num_digits < res_num_digits) {
            acc = static_cast<natural_double_digit>(res[r_num_digits]);
            acc -= static_cast<natural_double_digit>(acc_c & 1);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        }

        res[r_num_digits] = acc_c;
        return acc_c ? true : false;
    }

    /*
     * Subtract a multiplied and shifted digit array from another.
     *
     * Returns true if the subtraction underflows, false if it does not.
     */
    inline auto __digit_array_subtract_multiplied_m_n_shifted(natural_digit *res, std::size_t res_num_digits,
                                                              const natural_digit *src2, std::size_t src2_num_digits,
                                                              natural_digit src2_multiplier, std::size_t src2_shift) -> bool {
        /*
         * Multiply and subtract the first digit.
         */
        auto macc = (static_cast<natural_double_digit>(src2[0]) * static_cast<natural_double_digit>(src2_multiplier));
        auto macc_c = static_cast<natural_digit>(macc >> natural_digit_bits);
        auto macc_d = static_cast<natural_digit>(macc);
        auto sacc = static_cast<natural_double_digit>(res[src2_shift]) - static_cast<natural_double_digit>(macc_d);
        auto sacc_c = static_cast<natural_digit>(sacc >> natural_digit_bits);
        res[src2_shift] = static_cast<natural_digit>(sacc);

        /*
         * Multiply and subtract the remaining digits of src2.
         */
        std::size_t r_num_digits = src2_shift + 1;
        do {
            macc = (static_cast<natural_double_digit>(src2[r_num_digits - src2_shift]) * static_cast<natural_double_digit>(src2_multiplier));
            macc += static_cast<natural_double_digit>(macc_c);
            macc_c = static_cast<natural_digit>(macc >> natural_digit_bits);
            macc_d = static_cast<natural_digit>(macc);
            sacc = static_cast<natural_double_digit>(res[r_num_digits]) - static_cast<natural_double_digit>(macc_d);
            sacc -= static_cast<natural_double_digit>(sacc_c & 1);
            sacc_c = static_cast<natural_digit>(sacc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(sacc);
        } while (r_num_digits < (src2_num_digits + src2_shift));

        /*
         * Subtract any remaining carries.
         */
        while (r_num_digits < res_num_digits) {
            sacc = static_cast<natural_double_digit>(res[r_num_digits]) - static_cast<natural_double_digit>(macc_c);
            macc_c = 0;
            sacc -= static_cast<natural_double_digit>(sacc_c & 1);
            sacc_c = static_cast<natural_digit>(sacc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(sacc);
        }

        res[r_num_digits] = sacc_c;
        return sacc_c ? true : false;
    }

    /*
     * Add a shifted digit array to another.
     *
     * Returns true if the addition has not yet cleared an underflow from a previous subtraction,
     * false if it has.
     *
     * Unlike the more generic add operation above, this one is specifically only for use with
     * the divide/modulus operation.  This handles compensating for underflowing subtraction.
     */
    inline auto __digit_array_add_m_n_shifted(natural_digit *res, std::size_t res_num_digits,
                                              const natural_digit *src2, std::size_t src2_num_digits,
                                              std::size_t src2_shift) -> bool {
        /*
         * Add the first digits together.
         */
        auto acc = static_cast<natural_double_digit>(res[src2_shift]) + static_cast<natural_double_digit>(src2[0]);
        auto acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
        res[src2_shift] = static_cast<natural_digit>(acc);

        std::size_t r_num_digits = src2_shift + 1;
        do {
            acc = static_cast<natural_double_digit>(res[r_num_digits]) + static_cast<natural_double_digit>(src2[r_num_digits - src2_shift]);
            acc += static_cast<natural_double_digit>(acc_c);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        } while (r_num_digits < (src2_num_digits + src2_shift));

        /*
         * Add the remaining digits and any carries.
         */
        while (r_num_digits < res_num_digits) {
            acc = static_cast<natural_double_digit>(res[r_num_digits]);
            acc += static_cast<natural_double_digit>(acc_c);
            acc_c = static_cast<natural_digit>(acc >> natural_digit_bits);
            res[r_num_digits++] = static_cast<natural_digit>(acc);
        }

        /*
         * If we had a borrow from an earlier subtract then account for that now.
         */
        acc_c += res[r_num_digits];
        res[r_num_digits] = acc_c;
        return acc_c ? true : false;
    }

    /*
     * Divide two digit arrays.
     *
     * This function requires quite a lot of temporary digit arrays and so can
     * consume quite large amounts of stack space.
     */
    inline auto __digit_array_divide_modulus_m_n(natural_digit *quotient, std::size_t &quotient_num_digits,
                                                 natural_digit *remainder, std::size_t &remainder_num_digits,
                                                 const natural_digit *src1, std::size_t src1_num_digits,
                                                 const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Normalize the divisor and dividend.  We want our divisor to be aligned such
         * that it's most significant digit has its top bit set.  This may seem a little odd,
         * but we want to ensure that any quotient estimates are as accurate as possible.
         */
        auto divisor_bits = __digit_array_size_bits(src2, src2_num_digits);
        auto divisor_digit_bits = divisor_bits & (natural_digit_bits - 1);
        unsigned int normalize_shift = static_cast<unsigned int>((natural_digit_bits - divisor_digit_bits) & (natural_digit_bits - 1));

        /*
         * By definition when we normalize the divisor it keeps the same number of digits.
         */
        natural_digit divisor[src2_num_digits];
        std::size_t divisor_num_digits;
        __digit_array_left_shift_m(divisor, divisor_num_digits, src2, src2_num_digits, 0, normalize_shift);

        /*
         * Our dividend may end up one digit larger after the normalization and we want one
         * extra beyond that for handling any overflows.
         */
        natural_digit dividend[src1_num_digits + 2];
        std::size_t dividend_num_digits;
        __digit_array_left_shift_m(dividend, dividend_num_digits, src1, src1_num_digits, 0, normalize_shift);
        dividend[dividend_num_digits] = 0;

        /*
         * At the outset we need to compare the first digit of our dividend with the first
         * digit of our divisor.  If the dividend is larger then our first result digit is 1,
         * otherwise it's 0.
         */
        std::size_t next_res_digit = dividend_num_digits - divisor_num_digits;

        /*
         * All of our quotient estimates are done with the most significant digit of our divisor.
         */
        auto divisor_most_sig_digit = divisor[divisor_num_digits - 1];

        /*
         * Our first estimate is trivial - the digit will either be 0 or 1.  We handle
         * this as a special case.
         */
        natural_digit q = 0;
        if (dividend[dividend_num_digits - 1] >= divisor_most_sig_digit) {
            q = 1;
            auto underflow = __digit_array_subtract_m_n_shifted(dividend, dividend_num_digits + 1,
                                                                divisor, divisor_num_digits, next_res_digit);
            if (C8_UNLIKELY(underflow)) {
                __digit_array_add_m_n_shifted(dividend, dividend_num_digits + 1,
                                              divisor, divisor_num_digits, next_res_digit);
                q = 0;
            }

        }

        quotient[next_res_digit] = q;

        /*
         * Now we run our "long divide" algorithm over the remaining digits.
         */
        while (next_res_digit--) {
            auto d_hi = dividend[dividend_num_digits - 1];

            /*
             * Is the upper digit of our dividend the same as the first digit our of divisor?  If
             * it is then we know the result is, at most, the largest possible digit value,
             * because we've already handled all situations in which the result would be larger.
             */
            if (C8_UNLIKELY(d_hi == divisor_most_sig_digit)) {
                q = static_cast<natural_digit>(-1);
            } else {
                /*
                 * Estimate the next digit of the result by dividing the most significant two
                 * digits of our dividend by the most significant digit of our divisor.
                 */
                q = dividend[dividend_num_digits - 2];

                /*
                 * We need to do a divide, but a large natural_double_digit will typically be
                 * more than a single machine register in size.  The compiler typically won't be
                 * able to generate the most optimal code in such cases so we have to resort to
                 * inline assembler.
                 *
                 * The do/while loop here isn't actually a loop - it just lets us use break to
                 * early-out.  The compiler will invariably recognize that only one path is ever
                 * taken and optimize everything else away.
                 */
                do {
                    if (sizeof(natural_digit) == 8) {
#if defined(C8_USE_ASM) && defined(__x86_64__)
                        asm volatile (
                        "       divq    %[divisor_most_sig_digit]       \n\t"
                                : [q] "+a" (q), [d_hi] "+d" (d_hi)
                                : [divisor_most_sig_digit] "r" (divisor_most_sig_digit)
                                : "cc"
                        );

                        break;
#endif
                    }

                    auto d_lo_d = static_cast<natural_double_digit>(q);
                    auto d_hi_d = static_cast<natural_double_digit>(d_hi);
                    auto d = static_cast<natural_double_digit>(d_hi_d << natural_digit_bits) + d_lo_d;
                    q = static_cast<natural_digit>(d / static_cast<natural_double_digit>(divisor_most_sig_digit));
                } while (false);
            }

            /*
             * Did we estimate that there was a non-zero digit?  If yes, then we want to refine
             * the estimate and subtract it from our dividend, but if we estimated zero then
             * we do nothing to our dividend.
             */
            if (C8_LIKELY(q)) {
                /*
                 * We now have an estimate of our next digit, but it's possible that our estimate
                 * may be anything up to 2 larger than the actual value.  As a way to think about this,
                 * consider a decimal divide of 4500 by 59.  If we divide "45" by "5" we estimate that
                 * the next result digit is 9, but actually it's 7.  The vast majority of times our
                 * estimate will be correct, however.
                 */
                auto underflow1 = __digit_array_subtract_multiplied_m_n_shifted(dividend, dividend_num_digits + 1,
                                                                                divisor, divisor_num_digits, q, next_res_digit);
                if (C8_UNLIKELY(underflow1)) {
                    auto underflow2 = __digit_array_add_m_n_shifted(dividend, dividend_num_digits + 1,
                                                                    divisor, divisor_num_digits, next_res_digit);
                    q--;
                    if (C8_UNLIKELY(underflow2)) {
                        __digit_array_add_m_n_shifted(dividend, dividend_num_digits + 1,
                                                      divisor, divisor_num_digits, next_res_digit);
                        q--;
                    }
                }
            }

            quotient[next_res_digit] = q;
            dividend_num_digits--;
        }

        /*
         * Calculate our resulting digits.
         */
        std::size_t q_num_digits = src1_num_digits - src2_num_digits + 1;
        if (!quotient[q_num_digits - 1]) {
            q_num_digits--;
        }

        quotient_num_digits = q_num_digits;

        while (dividend_num_digits) {
            if (dividend[dividend_num_digits - 1]) {
                break;
            }

            dividend_num_digits--;
        }

        std::size_t r_num_digits = 0;
        if (dividend_num_digits) {
             __digit_array_right_shift_m(remainder, r_num_digits, dividend, dividend_num_digits, 0, normalize_shift);
        }

        remainder_num_digits = r_num_digits;
    }
}

#endif // __C8___DIGIT_ARRAY_H

