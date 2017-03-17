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
     * Return the number of bits in this digit array.
     */
    auto count_bits_digit_array(const natural_digit *p, std::size_t p_num_digits) noexcept -> unsigned int {
        return __count_bits_digit_array(p, p_num_digits);
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
     * Compare two digit arrays and report their relative numerical size.
     */
    inline auto compare_digit_arrays(const natural_digit *src1, std::size_t src1_num_digits,
                                     const natural_digit *src2, std::size_t src2_num_digits) noexcept -> comparison {
        return __compare_digit_arrays(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Add a single digit to a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto add_digit_array_digit(natural_digit *res,
                                      const natural_digit *src, std::size_t src_num_digits,
                                      natural_digit v) -> std::size_t {
        /*
         * Does src have only one digit?  If yes, then just add that digit and v.
         */
        if (src_num_digits == 1) {
            return __add_digit_digit(res, src[0], v);
        }

        /*
         * Add v to the n digits of src.
         */
        return __add_digit_array_digit(res, src, src_num_digits, v);
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto add_digit_arrays(natural_digit *res,
                                 const natural_digit *src1, std::size_t src1_num_digits,
                                 const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        /*
         * Does src2 have only one digit?  If yes then add that single digit to src1
         */
        if (src2_num_digits == 1) {
            return add_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        /*
         * Does src1 have only one digit?  If yes then add that single digit to src2.
         */
        if (src1_num_digits == 1) {
            return __add_digit_array_digit(res, src2, src2_num_digits, src1[0]);
        }

        /*
         * Worst case scenario:  We're adding two arrays of digits.
         */
        return __add_digit_arrays(res, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Subtract a single digit from a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto subtract_digit_array_digit(natural_digit *res,
                                           const natural_digit *src, std::size_t src_num_digits,
                                           natural_digit v) -> std::size_t {
        /*
         * Does src have only one digit?  If yes, then subtract v from just that digit.
         */
        if (src_num_digits == 1) {
            if (src[0] < v) {
                throw not_a_number();
            }

            return __subtract_digit_digit(res, src[0], v);
        }

        /*
         * Subtract v from the n digits of this number.
         */
        return __subtract_digit_array_digit(res, src, src_num_digits, v);
    }

    /*
     * Subtract one digit array from another.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    inline auto subtract_digit_arrays(natural_digit *res,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        /*
         * Does src2 have only one digit?  If yes, then we can use faster approaches.
         */
        if (src2_num_digits == 1) {
            return subtract_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        /*
         * We should not have a negative result!
         */
        if (__compare_digit_arrays(src1, src1_num_digits, src2, src2_num_digits) == comparison::lt) {
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
        /*
         * Does src have only one digit?  If yes then use a faster approach.
         */
        if (src_num_digits == 1) {
            return __left_shift_digit(res, src[0], shift_digits, shift_bits);
        }

        /*
         * Worst case:  Shift our digit array.
         */
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
        /*
         * Does src have only one digit?  If yes, then use a faster approach.
         */
        if (src_num_digits == 1) {
            return __right_shift_digit(res, src[0], shift_bits);
        }

        /*
         * Worst case:  Shift our digit array.
         */
        return __right_shift_digit_array(res, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Multiply a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto multiply_digit_array_digit(natural_digit *res,
                                           const natural_digit *src, std::size_t src_num_digits,
                                           natural_digit v) -> std::size_t {
        /*
         * Does this number have only one digit?  If yes, then just multiply that digit and v.
         */
        if (src_num_digits == 1) {
            return __multiply_digit_digit(res, src[0], v);
        }

        /*
         * Multiply the n digits of this number by v.
         */
        return __multiply_digit_array_digit(res, src, src_num_digits, v);
    }

    /*
     * Multiply two digit arrays.
     */
    inline auto multiply_digit_arrays(natural_digit *res,
                                      const natural_digit *src1, std::size_t src1_num_digits,
                                      const natural_digit *src2, std::size_t src2_num_digits) -> std::size_t {
        /*
         * Does src2 have only one digit?  If yes, then we can use faster approaches.
         */
        if (src2_num_digits == 1) {
            return multiply_digit_array_digit(res, src1, src1_num_digits, src2[0]);
        }

        /*
         * Does src1 have only one digit?  If yes then multiply that digit and the n digits of src2.
         */
        if (src1_num_digits == 1) {
            return __multiply_digit_array_digit(res, src2, src2_num_digits, src1[0]);
        }

        /*
         * Worst case scenario:  We're multiplying two arrays of digits.
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
     * Divide/modulus a digit array by a single digit.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    inline auto divide_modulus_digit_array_digit(natural_digit *res, natural_digit &mod,
                                                 const natural_digit *src, std::size_t src_num_digits,
                                                 natural_digit v) -> std::size_t {
        /*
         * Does this number have only one digit?  If yes, then divide that digit by v.
         */
        if (src_num_digits == 1) {
            return __divide_modulus_digit_digit(res, mod, src[0], v);
        }

        return __divide_modulus_digit_array_digit(res, mod, src, src_num_digits, v);
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
            quotient_num_digits = divide_modulus_digit_array_digit(quotient, remainder[0], src1, src1_num_digits, src2[0]);
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

