/*
 * digit_array.h
 */
#ifndef __C8___DIGIT_ARRAY_H
#define __C8___DIGIT_ARRAY_H

#include "c8.h"

namespace c8 {
    /*
     * Return the number of bits in this digit array.
     */
    auto __count_bits_digit_array(const natural_digit *p, std::size_t p_num_digits) noexcept -> unsigned int {
        /*
         * If we have no digits then this is a simple (special) case.
         */
        if (C8_UNLIKELY(p_num_digits == 0)) {
            return 0;
        }

        /*
         * We can account for trailing digits easily, but the most significant digit is
         * more tricky.  We use __builtin_clz() to count the leadign zeros of the digit,
         * but if the size of a digit is smaller than the size of an integer (which is
         * what __builtin_clz() uses) then we must compensate for the extra zeros that
         * it returns.
         */
        natural_digit d = p[p_num_digits - 1];
        auto c = (sizeof(int) / sizeof(natural_digit)) - 1;
        auto clz = static_cast<unsigned int>(__builtin_clz(d));
        return static_cast<unsigned int>((p_num_digits + c) * natural_digit_bits) - clz;
    }

    /*
     * Zero an array of digits.
     */
    inline auto __zero_digit_array(natural_digit *p, std::size_t p_num_digits) -> void {
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
    inline auto __copy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
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
    inline auto __rcopy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
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
    inline auto __compare_eq_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
            auto a = src1[i];
            auto b = src2[i];
            if (a != b) {
                return false;
            }
        }

        return true;
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    inline auto __compare_ne_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
            auto a = src1[i];
            auto b = src2[i];
            if (a != b) {
                return true;
            }
        }

        return false;
    }

    /*
     * Compare if digit array src1 is greater than digit array src2.
     */
    inline auto __compare_gt_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
    inline auto __compare_ge_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
    inline auto __compare_lt_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
    inline auto __compare_le_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
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
    inline auto __add_digit_digit(natural_digit *res, natural_digit v1, natural_digit v2) -> std::size_t {
        natural_double_digit r = static_cast<natural_double_digit>(v1);
        r += static_cast<natural_double_digit>(v2);

        res[0] = static_cast<natural_digit>(r);
        r >>= natural_digit_bits;
        if (C8_LIKELY(!r)) {
            return 1;
        }

        res[1] = static_cast<natural_digit>(r);
        return 2;
    }

    /*
     * Add a single digit to a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __add_digit_array_digit(natural_digit *res,
                                        const natural_digit *src, std::size_t src_num_digits,
                                        natural_digit v) -> std::size_t {
        /*
         * Is this number zero?  If yes then just construct the result.
         */
        if (C8_UNLIKELY(!src_num_digits)) {
            res[0] = v;
            return 1;
        }

        /*
         * Add the first digit.
         */
        auto a = src[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(v));
        res[0] = static_cast<natural_digit>(acc);
        acc >>= natural_digit_bits;

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < src_num_digits; i++) {
            acc += static_cast<natural_double_digit>(src[i]);
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * We may have a final carry digit, so handle that if it exists.
         */
        std::size_t res_num_digits = src_num_digits;
        if (C8_UNLIKELY(acc)) {
            res[res_num_digits++] = static_cast<natural_digit>(acc);
        }

        return res_num_digits;
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto __add_digit_arrays(natural_digit *res,
                                   const natural_digit *src1, std::size_t src1_num_digits,
                                   const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
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
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_num_digits; i++) {
            auto a = static_cast<natural_double_digit>(larger[i]);
            auto b = static_cast<natural_double_digit>(smaller[i]);
            acc += (a + b);
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_num_digits; i < larger_num_digits; i++) {
            acc += static_cast<natural_double_digit>(larger[i]);
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * We may have a final carry digit, so handle that if it exists.
         */
        std::size_t res_num_digits = larger_num_digits;
        if (C8_UNLIKELY(acc)) {
            res[res_num_digits++] = static_cast<natural_digit>(acc);
        }

        return res_num_digits;
    }

    /*
     * Subtract a single digit from another single digit.
     */
    inline auto __subtract_digit_digit(natural_digit *res, natural_digit v1, natural_digit v2) -> std::size_t {
        natural_digit r = v1 - v2;
        if (!r) {
            return 0;
        }

        res[0] = r;
        return 1;
    }

    /*
     * Subtract a single digit from a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __subtract_digit_array_digit(natural_digit *res,
                                             const natural_digit *src, std::size_t src_num_digits,
                                             natural_digit v) -> std::size_t {
        /*
         * Is this number zero?  If yes then just construct the result.  We know
         * that we cannot have a negative result so we don't need to worry about
         * throwing exceptions.
         */
        if (C8_UNLIKELY(!src_num_digits)) {
            return 0;
        }

        /*
         * Subtract the digits from this number's lowest digit.
         */
        auto a = static_cast<natural_double_digit>(src[0]);
        natural_double_digit acc = (a - static_cast<natural_double_digit>(v));
        res[0] = static_cast<natural_digit>(acc);
        acc = (acc >> natural_digit_bits) & 1;

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < src_num_digits; i++) {
            acc = static_cast<natural_double_digit>(src[i]) - acc;
            res[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t res_num_digits = src_num_digits;
        if (!res[res_num_digits - 1]) {
            res_num_digits--;
        }

        return res_num_digits;
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto __subtract_digit_arrays(natural_digit *res,
                                        const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < src2_num_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            auto b = static_cast<natural_double_digit>(src2[i]);
            acc = a - b - acc;
            res[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = src2_num_digits; i < src1_num_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            acc = a - acc;
            res[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Calculate the number of resulting digits.  With subtraction there's no
         * easy way to estimate this.
         */
        std::size_t res_num_digits = src1_num_digits;
        while (res_num_digits--) {
            if (res[res_num_digits]) {
                break;
            }
        }

        return res_num_digits + 1;
    }

    /*
     * Left shift a digit.
     */
    inline auto __left_shift_digit(natural_digit *res,natural_digit v,
                                   std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t res_num_digits = 1 + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            res[shift_digits] = v;
            __zero_digit_array(res, shift_digits);
            return res_num_digits;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        auto d = static_cast<natural_double_digit>(v) << shift_bits;
        res[shift_digits] = static_cast<natural_digit>(d);
        auto d_hi = static_cast<natural_digit>(d >> natural_digit_bits);
        if (d_hi) {
            res[res_num_digits++] = d_hi;
        }

        __zero_digit_array(res, shift_digits);
        return res_num_digits;
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __left_shift_digit_array(natural_digit *res,
                                         const natural_digit *src, std::size_t src_num_digits,
                                         std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t res_num_digits = src_num_digits + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            __rcopy_digit_array(&res[shift_digits], src, src_num_digits);
            __zero_digit_array(res, shift_digits);
            return res_num_digits;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        natural_digit d = src[src_num_digits - 1];
        natural_digit d_hi = d >> (natural_digit_bits - shift_bits);
        if (d_hi) {
            res[res_num_digits++] = d_hi;
        }

        for (std::size_t i = src_num_digits - 1; i > 0; i--) {
            d_hi = d << shift_bits;
            d = src[i - 1];
            res[i + shift_digits] = d_hi | (d >> (natural_digit_bits - shift_bits));
        }

        res[shift_digits] = d << shift_bits;
        __zero_digit_array(res, shift_digits);
        return res_num_digits;
    }

    /*
     * Right shift a digit.
     */
    inline auto __right_shift_digit(natural_digit *res, natural_digit v, std::size_t shift_bits) -> std::size_t {
        auto r = v >> shift_bits;
        if (!r) {
            return 0;
        }

        res[0] = r;
        return 1;
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __right_shift_digit_array(natural_digit *res,
                                          const natural_digit *src, std::size_t src_num_digits,
                                          std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t res_num_digits = src_num_digits - (shift_digits + 1);

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            res_num_digits++;
            __copy_digit_array(res, &src[shift_digits], res_num_digits);
            return res_num_digits;
        }

        /*
         * Shift the original value and insert in the result.
         */
        natural_digit d_lo = src[shift_digits] >> shift_bits;
        for (std::size_t i = 1; i <= res_num_digits; i++) {
            natural_digit d = src[i + shift_digits];
            res[i - 1] = d_lo | (d << (natural_digit_bits - shift_bits));
            d_lo = d >> shift_bits;
        }

        if (d_lo) {
            res[res_num_digits++] = d_lo;
        }

        return res_num_digits;
    }

    /*
     * Multiply two non-zero single digits.
     */
    inline auto __multiply_digit_digit(natural_digit *res, natural_digit v1, natural_digit v2) -> std::size_t {
        natural_double_digit r = static_cast<natural_double_digit>(v1);
        r *= static_cast<natural_double_digit>(v2);

        res[0] = static_cast<natural_digit>(r);
        r >>= natural_digit_bits;
        if (C8_LIKELY(!r)) {
            return 1;
        }

        res[1] = static_cast<natural_digit>(r);
        return 2;
    }

    /*
     * Multiply a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __multiply_digit_array_digit(natural_digit *res,
                                             const natural_digit *src, std::size_t src_num_digits,
                                             natural_digit v) -> std::size_t {
        std::size_t res_num_digits = src_num_digits;

        /*
         * Long multiply.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < src_num_digits; i++) {
            auto a = src[i];
            acc = acc + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(v));
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res[res_num_digits++] = static_cast<natural_digit>(acc);
        }

        return res_num_digits;
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto __multiply_digit_arrays(natural_digit *res,
                                        const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
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
        std::size_t res_num_digits = src1_num_digits + src2_num_digits - 1;
        natural_double_digit acc1 = 0;
        std::size_t tj_lim = src2_num_digits - 1;

        for (std::size_t res_column = 0; res_column < res_num_digits; res_column++) {
            std::size_t tj = (res_column < tj_lim) ? res_column : tj_lim;
            std::size_t ti = res_column - tj;
            std::size_t num_multiplies = ((src1_num_digits - ti) < (tj + 1)) ? (src1_num_digits - ti) : (tj + 1);
            auto acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(acc1));
            acc1 >>= natural_digit_bits;
            for (std::size_t j = 0; j < num_multiplies; j++) {
                auto a = static_cast<natural_double_digit>(src1[ti++]);
                auto b = static_cast<natural_double_digit>(src2[tj--]);
                natural_double_digit d0 = acc0 + (a * b);
                acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(d0));
                acc1 += (d0 >> natural_digit_bits);
            }

            res[res_column] = static_cast<natural_digit>(acc0);
        }

        /*
         * We may have an extra upper digit, so account for this.
         */
        if (acc1) {
            res[res_num_digits++] = static_cast<natural_digit>(acc1);
        }

        return res_num_digits;
    }

    /*
     * Multiply a digit array by a single digit and left shift by an integer number of digits.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __multiply_digit_array_digit_and_left_shift(natural_digit *res,
                                                            const natural_digit *src, std::size_t src_num_digits,
                                                            natural_digit v, std::size_t shift_digits) -> std::size_t {
        __zero_digit_array(res, shift_digits);
        std::size_t res_num_digits = src_num_digits + shift_digits;

        /*
         * Long multiply, but include the digit shift in the result.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < src_num_digits; i++) {
            auto a = src[i];
            acc = acc + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(v));
            res[shift_digits + i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res[res_num_digits++] = static_cast<natural_digit>(acc);
        }

        return res_num_digits;
    }

    /*
     * Divide/modulus a single digit by another single digit.
     */
    inline auto __divide_modulus_digit_digit(natural_digit *res, natural_digit &mod,
                                             natural_digit v1, natural_digit v2) -> std::size_t {
        natural_digit d = v1 / v2;
        mod = v1 % v2;
        if (!d) {
            return 0;
        }

        res[0] = d;
        return 1;
    }

    /*
     * Divide/modulus a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto __divide_modulus_digit_array_digit(natural_digit *res, natural_digit &mod,
                                                   const natural_digit *src, std::size_t src_num_digits,
                                                   natural_digit v) -> std::size_t {
        /*
         * Now we run a long divide algorithm.
         */
        natural_double_digit acc = 0;
        std::size_t i = src_num_digits;
        while (i--) {
            auto a = src[i];
            acc = static_cast<natural_double_digit>(acc << natural_digit_bits) + static_cast<natural_double_digit>(a);
            natural_double_digit q = acc / v;
            acc = acc % v;
            res[i] = static_cast<natural_digit>(q);
        }

        mod = static_cast<natural_digit>(acc);

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t res_num_digits = src_num_digits;
        if (!res[res_num_digits - 1]) {
            res_num_digits--;
        }

        return res_num_digits;
    }

    /*
     * Divide two digit arrays.
     *
     * This function requires quite a lot of temporary digit arrays and so can
     * consume quite large amounts of stack space.
     */
    inline auto __divide_modulus_digit_arrays(natural_digit *quotient, std::size_t &quotient_num_digits,
                                              natural_digit *remainder, std::size_t &remainder_num_digits,
                                              const natural_digit *src1, std::size_t src1_num_digits,
                                              const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Normalize the divisor and dividend.  We want our divisor to be aligned such
         * that it's most significant digit has its top bit set.  This may seem a little odd,
         * but we want to ensure that any quotient estimates are as accurate as possible.
         */
        auto divisor_bits = __count_bits_digit_array(src2, src2_num_digits);
        auto divisor_digit_bits = divisor_bits & (natural_digit_bits - 1);
        unsigned int normalize_shift = static_cast<unsigned int>((natural_digit_bits - divisor_digit_bits) & (natural_digit_bits - 1));

        /*
         * By definition when we normalize the divisor it keeps the same number of digits.
         */
        natural_digit divisor[src2_num_digits];
        auto divisor_num_digits = __left_shift_digit_array(divisor, src2, src2_num_digits, 0, normalize_shift);

        /*
         * Our dividend may end up one digit larger after the normalization.
         */
        natural_digit dividend[src1_num_digits + 1];
        auto dividend_num_digits = __left_shift_digit_array(dividend, src1, src1_num_digits, 0, normalize_shift);

        /*
         * We need a temporary to hold various partial computations.  This needs to be
         * as larger at the dividend.
         */
        natural_digit t1[src1_num_digits + 1];

        quotient_num_digits = src1_num_digits - src2_num_digits + 1;
        __zero_digit_array(quotient, quotient_num_digits);

        /*
         * Now we run a long divide algorithm.
         */
        auto upper_div_digit = divisor[divisor_num_digits - 1];
        while (true) {
            std::size_t i = dividend_num_digits - 1;
            std::size_t next_res_digit = i - divisor_num_digits;

            /*
             * We know that our divisor has been shifted so that the most significant digit has
             * its top bit set.  This means that the quotient for our next digit can only be 0 or 1.
             * If we compare the most significant digit of our dividend with that of the divisor
             * we can see if it's possibly 1.
             */
            std::size_t t1_num_digits;
            auto d_hi = dividend[i];
            if (d_hi >= upper_div_digit) {
                /*
                 * Our next quotient digit is probably a 1, but we have to be sure.  It's possible
                 * that the subsequent digits of the divisor are large enough that it's actually
                 * still zero, but in that case our next digit will be as large as it can be.
                 */
                t1_num_digits = __left_shift_digit_array(t1, divisor, divisor_num_digits, (next_res_digit + 1), 0);
                if (__compare_le_digit_arrays(t1, t1_num_digits, dividend, dividend_num_digits)) {
                    /*
                     * Our result was 1.
                     */
                    quotient[next_res_digit + 1] = 1;
                } else {
                    /*
                     * Our digit was actually 0 after all, so we know definitively that the next
                     * digit is it's maximum possible size.
                     */
                    const auto q = static_cast<natural_digit>(-1);
                    t1_num_digits = __multiply_digit_array_digit_and_left_shift(t1, divisor, divisor_num_digits,
                                                                                q, next_res_digit);
                    quotient[next_res_digit] = q;
                }
            } else {
                /*
                 * Estimate the next digit of the result.
                 */
                natural_double_digit d_lo_d = static_cast<natural_double_digit>(dividend[i - 1]);
                natural_double_digit d_hi_d = static_cast<natural_double_digit>(d_hi);
                natural_double_digit d = static_cast<natural_double_digit>(d_hi_d << natural_digit_bits) + d_lo_d;
                auto q = static_cast<natural_digit>(d / static_cast<natural_double_digit>(upper_div_digit));
                t1_num_digits = __multiply_digit_array_digit_and_left_shift(t1, divisor, divisor_num_digits,
                                                                            q, next_res_digit);

                /*
                 * It's possible that our estimate might be slightly too large, so we have
                 * to evaluate it on the basis of the full divisor, not just the shifted most
                 * significant digit.  This may mean we reduce our estimate slightly.
                 */
                if (C8_UNLIKELY(__compare_gt_digit_arrays(t1, t1_num_digits, dividend, dividend_num_digits))) {
                    q--;
                    t1_num_digits = __multiply_digit_array_digit_and_left_shift(t1, divisor, divisor_num_digits,
                                                                                q, next_res_digit);
                }

                quotient[next_res_digit] = q;
            }

            dividend_num_digits = __subtract_digit_arrays(dividend, dividend, dividend_num_digits, t1, t1_num_digits);
            if (C8_UNLIKELY(__compare_le_digit_arrays(dividend, dividend_num_digits, divisor, divisor_num_digits))) {
                break;
            }
        }

        /*
         * Calculate our resulting digits.
         */
        if (!quotient[quotient_num_digits - 1]) {
            quotient_num_digits--;
        }

        remainder_num_digits = 0;
        if (dividend_num_digits) {
            remainder_num_digits = __right_shift_digit_array(remainder, dividend, dividend_num_digits, 0, normalize_shift);
        }
    }
}

#endif // __C8___DIGIT_ARRAY_H

