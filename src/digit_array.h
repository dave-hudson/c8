/*
 * digit_array.h
 *
 * Implements generic digit array logic.
 */
#ifndef __C8_DIGIT_ARRAY_H
#define __C8_DIGIT_ARRAY_H

#include "c8.h"
#include "__digit_array.h"

namespace c8 {
    /*
     * Return the number of bits required by this digit array.
     */
    auto digit_array_size_bits(const natural_digit *p, std::size_t p_num_digits) noexcept -> std::size_t {
        return __digit_array_size_bits(p, p_num_digits);
    }

    /*
     * Copy (forwards) an array of digits.
     */
    inline auto digit_array_copy(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        return __digit_array_copy(res, src, src_num_digits);
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    inline auto digit_array_compare_eq(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_eq(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is not equal to digit array src2.
     */
    inline auto digit_array_compare_ne(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_ne(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than digit array src2.
     */
    inline auto digit_array_compare_gt(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_gt(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than, or equal to, digit array src2.
     */
    inline auto digit_array_compare_ge(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_ge(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than digit array src2.
     */
    inline auto digit_array_compare_lt(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_lt(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than, or equal to, digit array src2.
     */
    inline auto digit_array_compare_le(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_le(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto digit_array_add(natural_digit *res, std::size_t &res_num_digits,
                                const natural_digit *src1, std::size_t src1_num_digits,
                                const natural_digit *src2, std::size_t src2_num_digits) -> void {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                __digit_array_add_1_1(res, res_num_digits, src1, src2);
                return;
            }

            __digit_array_add_m_1(res, res_num_digits, src1, src1_num_digits, src2);
            return;
        }

        if (src1_num_digits == 1) {
            __digit_array_add_m_1(res, res_num_digits, src2, src2_num_digits, src1);
            return;
        }

        __digit_array_add_m_n(res, res_num_digits, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto digit_array_subtract(natural_digit *res, std::size_t &res_num_digits,
                                     const natural_digit *src1, std::size_t src1_num_digits,
                                     const natural_digit *src2, std::size_t src2_num_digits) -> void {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                /*
                 * If our result would be negative then throw an exception.
                 */
                if (src1[0] < src2[0]) {
                    throw not_a_number();
                }

                __digit_array_subtract_1_1(res, res_num_digits, src1, src2);
                return;
            }

            __digit_array_subtract_m_1(res, res_num_digits, src1, src1_num_digits, src2);
            return;
        }

        /*
         * We should not have a negative result!
         */
        if (__digit_array_compare_lt(src1, src1_num_digits, src2, src2_num_digits)) {
            throw not_a_number();
        }

        __digit_array_subtract_m_n(res, res_num_digits, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto digit_array_left_shift(natural_digit *res, std::size_t &res_num_digits,
                                       const natural_digit *src, std::size_t src_num_digits,
                                       std::size_t shift_digits, std::size_t shift_bits) -> void {
        if (src_num_digits == 1) {
            __digit_array_left_shift_1(res, res_num_digits, src, shift_digits, shift_bits);
            return;
        }

        return __digit_array_left_shift_m(res, res_num_digits, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto digit_array_right_shift(natural_digit *res, std::size_t &res_num_digits,
                                        const natural_digit *src, std::size_t src_num_digits,
                                        std::size_t shift_digits, std::size_t shift_bits) -> void {
        if (src_num_digits == 1) {
            __digit_array_right_shift_1(res, res_num_digits, src, shift_bits);
            return;
        }

        return __digit_array_right_shift_m(res, res_num_digits, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto digit_array_multiply(natural_digit *res, std::size_t &res_num_digits,
                                     const natural_digit *src1, std::size_t src1_num_digits,
                                     const natural_digit *src2, std::size_t src2_num_digits) -> void {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                __digit_array_multiply_1_1(res, res_num_digits, src1, src2);
                return;
            }

            __digit_array_multiply_m_1(res, res_num_digits, src1, src1_num_digits, src2);
            return;
        }

        if (src1_num_digits == 1) {
            __digit_array_multiply_m_1(res, res_num_digits, src2, src2_num_digits, src1);
            return;
        }

        /*
         * Worst case scenario:  We're multiplying two arrays of digits.  If we're going to
         * update in place then we actually have to copy the source array because we'll
         * overwrite it.
         */
        auto src1_1 = src1;
        natural_digit src1_copy[src1_num_digits];
        if (res == src1) {
            __digit_array_copy(src1_copy, src1, src1_num_digits);
            src1_1 = src1_copy;
        }

        __digit_array_multiply_m_n(res, res_num_digits, src1_1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Divide two digit arrays.
     */
    inline auto digit_array_divide_modulus(natural_digit *quotient, std::size_t &quotient_num_digits,
                                           natural_digit *remainder, std::size_t &remainder_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Does, our divisor src2, only have one digit?  If yes, then use the fast version
         * of divide_modulus.
         */
        if (src2_num_digits == 1) {
            /*
             * Does this number have only one digit?  If yes, then divide that digit by v.
             */
            if (src1_num_digits == 1) {
                __digit_array_divide_modulus_1_1(quotient, quotient_num_digits, remainder, remainder_num_digits,
                                                 src1, src2);
                return;
            }

            __digit_array_divide_modulus_m_1(quotient, quotient_num_digits, remainder, remainder_num_digits,
                                             src1, src1_num_digits, src2);
            return;
        }

        __digit_array_divide_modulus_m_n(quotient, quotient_num_digits, remainder, remainder_num_digits,
                                         src1, src1_num_digits, src2, src2_num_digits);
    }
}

#endif // __C8_DIGIT_ARRAY_H

