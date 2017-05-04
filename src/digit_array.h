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
    auto size_bits_digit_array(const natural_digit *p, std::size_t p_num_digits) noexcept -> std::size_t {
        return __size_bits_digit_array(p, p_num_digits);
    }

    /*
     * Zero an array of digits.
     */
    inline auto zero_digit_array(natural_digit *p, std::size_t p_num_digits) -> void {
        return __zero_digit_array(p, p_num_digits);
    }

    /*
     * Copy (forwards) an array of digits.
     */
    inline auto copy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        return __copy_digit_array(res, src, src_num_digits);
    }

    /*
     * Reverse copy an array of digits.
     */
    inline auto rcopy_digit_array(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        return __rcopy_digit_array(res, src, src_num_digits);
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    inline auto compare_eq_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_eq_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is not equal to digit array src2.
     */
    inline auto compare_ne_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_ne_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than digit array src2.
     */
    inline auto compare_gt_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_gt_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than, or equal to, digit array src2.
     */
    inline auto compare_ge_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_ge_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than digit array src2.
     */
    inline auto compare_lt_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_lt_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than, or equal to, digit array src2.
     */
    inline auto compare_le_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                        const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __compare_le_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto add_digit_arrays(natural_digit *res,
                                 const natural_digit *src1, std::size_t src1_num_digits,
                                 const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                return __add_digit_digit(res, src1[0], src2[0]);
            }

            return __add_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        if (src1_num_digits == 1) {
            return __add_digit_array_digit(res, src2, src2_num_digits, src1[0]);
        }

        return __add_digit_arrays(res, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto subtract_digit_arrays(natural_digit *res,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                /*
                 * If our result would be negative then throw an exception.
                 */
                if (src1[0] < src2[0]) {
                    throw not_a_number();
                }

                return __subtract_digit_digit(res, src1[0], src2[0]);
            }

            return __subtract_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        /*
         * We should not have a negative result!
         */
        if (__compare_lt_digit_arrays(src1, src1_num_digits, src2, src2_num_digits)) {
            throw not_a_number();
        }

        return __subtract_digit_arrays(res, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto left_shift_digit_array(natural_digit *res,
                                       const natural_digit *src, std::size_t src_num_digits,
                                       std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        if (src_num_digits == 1) {
            return __left_shift_digit(res, src[0], shift_digits, shift_bits);
        }

        return __left_shift_digit_array(res, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto right_shift_digit_array(natural_digit *res,
                                        const natural_digit *src, std::size_t src_num_digits,
                                        std::size_t shift_digits, std::size_t shift_bits) -> std::size_t {
        if (src_num_digits == 1) {
            return __right_shift_digit(res, src[0], shift_bits);
        }

        return __right_shift_digit_array(res, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto multiply_digit_arrays(natural_digit *res,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                return __multiply_digit_digit(res, src1[0], src2[0]);
            }

            return __multiply_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        if (src1_num_digits == 1) {
            return __multiply_digit_array_digit(res, src2, src2_num_digits, src1[0]);
        }

        /*
         * Worst case scenario:  We're multiplying two arrays of digits.  If we're going to
         * update in place then we actually have to copy the source array because we'll
         * overwrite it.
         */
        auto src1_1 = src1;
        natural_digit src1_copy[src1_num_digits];
        if (res == src1) {
            __copy_digit_array(src1_copy, src1, src1_num_digits);
            src1_1 = src1_copy;
        }

        return __multiply_digit_arrays(res, src1_1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Divide two digit arrays.
     */
    inline auto divide_modulus_digit_arrays(natural_digit *quotient, std::size_t &quotient_num_digits,
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
                quotient_num_digits = __divide_modulus_digit_digit(quotient, remainder[0], src1[0], src2[0]);
            } else {
                quotient_num_digits = __divide_modulus_digit_array_digit(quotient, remainder[0],
                                                                         src1, src1_num_digits, src2[0]);
            }

            remainder_num_digits = 0;
            if (remainder[0]) {
                remainder_num_digits = 1;
            }

            return;
        }

        __divide_modulus_digit_arrays(quotient, quotient_num_digits, remainder, remainder_num_digits,
                                      src1, src1_num_digits, src2, src2_num_digits);
    }
}

#endif // __C8_DIGIT_ARRAY_H

