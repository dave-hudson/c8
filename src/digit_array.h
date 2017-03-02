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
    inline auto zero_digit_array(natural_digit *p, std::size_t num_digits) -> void {
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
     * Copy (forwards) an array of digits.
     */
    inline auto copy_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_digits) -> void {
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
    inline auto rcopy_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_digits) -> void {
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

    inline auto compare_digit_arrays(const natural_digit *src1, std::size_t num_src1_digits, const natural_digit *src2, std::size_t num_src2_digits) noexcept -> comparison {
        /*
         * If our sizes differ then this is really easy!
         */
        if (num_src1_digits > num_src2_digits) {
            return comparison::gt;
        }

        if (num_src1_digits < num_src2_digits) {
            return comparison::lt;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        std::size_t i = num_src1_digits;
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
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto add_digit_array_digit(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, natural_digit v) -> std::size_t {
        /*
         * Is this number zero?  If yes then just construct the result.
         */
        if (C8_UNLIKELY(!num_src_digits)) {
            dest[0] = v;
            return 1;
        }

        /*
         * Add the first digit.
         */
        auto a = src[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(v));
        dest[0] = static_cast<natural_digit>(acc);
        acc >>= natural_digit_bits;

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < num_src_digits; i++) {
            acc += static_cast<natural_double_digit>(src[i]);
            dest[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        std::size_t num_dest_digits = num_src_digits;
        if (C8_UNLIKELY(acc)) {
            dest[num_dest_digits++] = static_cast<natural_digit>(acc);
        }

        return num_dest_digits;
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for dest and either src1, or src2, to be the same pointer.
     */
    inline auto add_digit_arrays(natural_digit *dest, const natural_digit *src1, std::size_t num_src1_digits, const natural_digit *src2, std::size_t num_src2_digits) -> std::size_t {
        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < num_src2_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            auto b = static_cast<natural_double_digit>(src2[i]);
            acc += (a + b);
            dest[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = num_src2_digits; i < num_src1_digits; i++) {
            acc += static_cast<natural_double_digit>(src1[i]);
            dest[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        std::size_t num_dest_digits = num_src1_digits;
        if (C8_UNLIKELY(acc)) {
            dest[num_dest_digits++] = static_cast<natural_digit>(acc);
        }

        return num_dest_digits;
    }

    /*
     * Subtract a single digit from a digit array.
     *
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto subtract_digit_array_digit(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, natural_digit v) -> std::size_t {
        /*
         * Is this number zero?  If yes then just construct the result.  We know
         * that we cannot have a negative result so we don't need to worry about
         * throwing exceptions.
         */
        if (C8_UNLIKELY(!num_src_digits)) {
            return 0;
        }

        /*
         * Subtract the digits from this number's lowest digit.
         */
        auto a = static_cast<natural_double_digit>(src[0]);
        natural_double_digit acc = (a - static_cast<natural_double_digit>(v));
        dest[0] = static_cast<natural_digit>(acc);
        acc = (acc >> natural_digit_bits) & 1;

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < num_src_digits; i++) {
            acc = static_cast<natural_double_digit>(src[i]) - acc;
            dest[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t num_dest_digits = num_src_digits;
        if (!dest[num_dest_digits - 1]) {
            num_dest_digits--;
        }

        return num_dest_digits;
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for dest and either src1, or src2, to be the same pointer.
     */
    inline auto subtract_digit_arrays(natural_digit *dest, const natural_digit *src1, std::size_t num_src1_digits, const natural_digit *src2, std::size_t num_src2_digits) -> std::size_t {
        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < num_src2_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            auto b = static_cast<natural_double_digit>(src2[i]);
            acc = a - b - acc;
            dest[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = num_src2_digits; i < num_src1_digits; i++) {
            auto a = static_cast<natural_double_digit>(src1[i]);
            acc = a - acc;
            dest[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Calculate the number of resulting digits.  With subtraction there's no
         * easy way to estimate this numbers.
         */
        std::size_t num_dest_digits = num_src1_digits;
        while (num_dest_digits--) {
            if (dest[num_dest_digits]) {
                break;
            }
        }

        return num_dest_digits + 1;
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto left_shift_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t num_dest_digits = num_src_digits + shift_digits;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            rcopy_digit_array(&dest[shift_digits], src, num_src_digits);
            zero_digit_array(dest, shift_digits);

            return num_dest_digits;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        natural_digit d = src[num_src_digits - 1];
        natural_digit d_hi = d >> (natural_digit_bits - shift_bits);
        if (d_hi) {
            dest[num_dest_digits++] = d_hi;
        }

        for (std::size_t i = num_src_digits - 1; i > 0; i--) {
            d_hi = d << shift_bits;
            d = src[i - 1];
            dest[i + shift_digits] = d_hi | (d >> (natural_digit_bits - shift_bits));
        }

        dest[shift_digits] = d << shift_bits;

        zero_digit_array(dest, shift_digits);

        return num_dest_digits;
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto right_shift_digit_array(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        std::size_t num_dest_digits = num_src_digits - (shift_digits + 1);

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(shift_bits == 0)) {
            num_dest_digits++;
            copy_digit_array(dest, &src[shift_digits], num_dest_digits);

            return num_dest_digits;
        }

        /*
         * Shift the original value and insert in the result.
         */
        natural_digit d_lo = src[shift_digits] >> shift_bits;
        for (std::size_t i = 1; i <= num_dest_digits; i++) {
            natural_digit d = src[i + shift_digits];
            dest[i - 1] = d_lo | (d << (natural_digit_bits - shift_bits));
            d_lo = d >> shift_bits;
        }

        if (d_lo) {
            dest[num_dest_digits++] = d_lo;
        }

        return num_dest_digits;
    }

    /*
     * Multiply a digit array by a single digit.
     *
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto multiply_digit_array_digit(natural_digit *dest, const natural_digit *src, std::size_t num_src_digits, natural_digit v) -> std::size_t {
        std::size_t num_dest_digits = num_src_digits;

        /*
         * Long multiply.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < num_src_digits; i++) {
            auto a = src[i];
            acc = acc + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(v));
            dest[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            dest[num_dest_digits++] = static_cast<natural_digit>(acc);
        }

        return num_dest_digits;
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto multiply_digit_arrays(natural_digit *dest, const natural_digit *src1, std::size_t num_src1_digits, const natural_digit *src2, std::size_t num_src2_digits) -> std::size_t {
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
        std::size_t num_dest_digits = num_src1_digits + num_src2_digits - 1;
        natural_double_digit acc1 = 0;
        std::size_t tj_lim = num_src2_digits - 1;

        for (std::size_t dest_column = 0; dest_column < num_dest_digits; dest_column++) {
            std::size_t tj = (dest_column < tj_lim) ? dest_column : tj_lim;
            std::size_t ti = dest_column - tj;
            std::size_t num_multiplies = ((num_src1_digits - ti) < (tj + 1)) ? (num_src1_digits - ti) : (tj + 1);
            auto acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(acc1));
            acc1 >>= natural_digit_bits;
            for (std::size_t j = 0; j < num_multiplies; j++) {
                auto a = static_cast<natural_double_digit>(src1[ti++]);
                auto b = static_cast<natural_double_digit>(src2[tj--]);
                natural_double_digit d0 = acc0 + (a * b);
                acc0 = static_cast<natural_double_digit>(static_cast<natural_digit>(d0));
                acc1 += (d0 >> natural_digit_bits);
            }

            dest[dest_column] = static_cast<natural_digit>(acc0);
        }

        /*
         * We may have an extra upper digit, so account for this.
         */
        if (acc1) {
            dest[num_dest_digits++] = static_cast<natural_digit>(acc1);
        }

        return num_dest_digits;
    }

    /*
     * Divide/modulus a digit array by a single digit.
     *
     * Note: It is OK for dest and src to be the same pointer.
     */
    inline auto divide_modulus_digit_array_digit(natural_digit *dest, natural_digit &mod, const natural_digit *src, std::size_t num_src_digits, natural_digit v) -> std::size_t {
        /*
         * Now we run a long divide algorithm.
         */
        natural_double_digit acc = 0;
        std::size_t i = num_src_digits;
        while (i--) {
            auto a = src[i];
            acc = static_cast<natural_double_digit>(acc << natural_digit_bits) + static_cast<natural_double_digit>(a);
            natural_double_digit q = acc / v;
            acc = acc % v;
            dest[i] = static_cast<natural_digit>(q);
        }

        mod = static_cast<natural_digit>(acc);

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t num_dest_digits = num_src_digits;
        if (!dest[num_dest_digits - 1]) {
            num_dest_digits--;
        }

        return num_dest_digits;
    }
}

#endif // __C8_DIGIT_ARRAY_H

