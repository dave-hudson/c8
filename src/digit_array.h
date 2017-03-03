/*
 * digit_array.h
 */
#ifndef __C8_DIGIT_ARRAY_H
#define __C8_DIGIT_ARRAY_H

#include "natural.h"

namespace c8 {
    /*
     * Zero an array of digits.
     */
    inline auto zero_digit_array(natural_digit *p, std::size_t p_num_digits) -> void {
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
    inline auto copy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
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
    inline auto rcopy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
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

    inline auto compare_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits, const natural_digit *src2, std::size_t src2_num_digits) noexcept -> comparison {
        /*
         * If our sizes differ then this is really easy!
         */
        if (src1_num_digits > src2_num_digits) {
            return comparison::gt;
        }

        if (src1_num_digits < src2_num_digits) {
            return comparison::lt;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = src1_num_digits;
        while (i--) {
            auto a = src1[i];
            auto b = src2[i];
            if (a > b) {
                return comparison::gt;
            }

            if (a < b) {
                return comparison::lt;
            }
        }

        return comparison::eq;
    }

    /*
     * Add a single digit to a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto add_digit_array_digit(natural_digit *res, const natural_digit *src, std::size_t src_num_digits, natural_digit v) -> std::size_t {
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
    inline auto add_digit_arrays(natural_digit *res, const natural_digit *src1, std::size_t src1_num_digits, const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < src2_num_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            auto b = static_cast<natural_double_digit>(src2[i]);
            acc += (a + b);
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = src2_num_digits; i < src1_num_digits; i++) {
            acc += static_cast<natural_double_digit>(src1[i]);
            res[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        std::size_t res_num_digits = src1_num_digits;
        if (C8_UNLIKELY(acc)) {
            res[res_num_digits++] = static_cast<natural_digit>(acc);
        }

        return res_num_digits;
    }

    /*
     * Subtract a single digit from a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto subtract_digit_array_digit(natural_digit *res, const natural_digit *src, std::size_t src_num_digits, natural_digit v) -> std::size_t {
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
    inline auto subtract_digit_arrays(natural_digit *res, const natural_digit *src1, std::size_t src1_num_digits, const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
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
         * easy way to estimate this numbers.
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
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto left_shift_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits, std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t res_num_digits = src_num_digits + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            rcopy_digit_array(&res[shift_digits], src, src_num_digits);
            zero_digit_array(res, shift_digits);

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

        zero_digit_array(res, shift_digits);

        return res_num_digits;
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto right_shift_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits, std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t res_num_digits = src_num_digits - (shift_digits + 1);

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            res_num_digits++;
            copy_digit_array(res, &src[shift_digits], res_num_digits);

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
     * Multiply a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto multiply_digit_array_digit(natural_digit *res, const natural_digit *src, std::size_t src_num_digits, natural_digit v) -> std::size_t {
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
    inline auto multiply_digit_arrays(natural_digit *res, const natural_digit *src1, std::size_t src1_num_digits, const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
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
     * Divide/modulus a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto divide_modulus_digit_array_digit(natural_digit *res, natural_digit &mod, const natural_digit *src, std::size_t src_num_digits, natural_digit v) -> std::size_t {
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
}

#endif // __C8_DIGIT_ARRAY_H

