/*
 * digit_array.h
 *
 * Implements generic digit array logic.
 */
#ifndef __C8_DIGIT_ARRAY_H
#define __C8_DIGIT_ARRAY_H

#include "c8.h"
#include "__digit_array.h"

#undef INLINE
#define INLINE inline __attribute__((always_inline))

namespace c8 {
    /*
     * Return the number of bits required by this digit array.
     */
    INLINE auto digit_array_size_bits(const natural_digit *p, std::size_t p_num_digits) noexcept -> std::size_t {
        return __digit_array_size_bits(p, p_num_digits);
    }

    /*
     * Copy (forwards) an array of digits.
     */
    INLINE auto digit_array_copy(natural_digit *res, const natural_digit *src, std::size_t src_num_digits) -> void {
        return __digit_array_copy(res, src, src_num_digits);
    }

    /*
     * Compare if digit array src1 is equal to digit array src2.
     */
    INLINE auto digit_array_compare_eq(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_eq(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is not equal to digit array src2.
     */
    INLINE auto digit_array_compare_ne(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_ne(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than digit array src2.
     */
    INLINE auto digit_array_compare_gt(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_gt(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is greater than, or equal to, digit array src2.
     */
    INLINE auto digit_array_compare_ge(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_ge(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than digit array src2.
     */
    INLINE auto digit_array_compare_lt(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_lt(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Compare if digit array src1 is less than, or equal to, digit array src2.
     */
    INLINE auto digit_array_compare_le(const natural_digit *src1, std::size_t src1_num_digits,
                                       const natural_digit *src2, std::size_t src2_num_digits) noexcept -> bool {
        return __digit_array_compare_le(src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Add two digit arrays.
     *
     * Note: It is OK for res and either src1, or src2, to be the same pointer.
     */
    INLINE auto digit_array_add(natural_digit *res, std::size_t &res_num_digits,
                                const natural_digit *src1, std::size_t src1_num_digits,
                                const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Are we adding zero?  If yes, then just duplicate src1.
         */
        if (src2_num_digits == 0) {
            if (res != src1) {
                __digit_array_copy(res, src1, src1_num_digits);
            }

            res_num_digits = src1_num_digits;
            return;
        }

        /*
         * Are we adding to zero?  If yes, then just duplicate src2.
         */
        if (src1_num_digits == 0) {
            if (res != src2) {
                __digit_array_copy(res, src2, src2_num_digits);
            }

            res_num_digits = src2_num_digits;
            return;
        }

        /*
         * Handle the various digit number permutations.
         */
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
    INLINE auto digit_array_subtract(natural_digit *res, std::size_t &res_num_digits,
                                     const natural_digit *src1, std::size_t src1_num_digits,
                                     const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Are we subtracting zero?  If yes, then just duplicate src1.
         */
        if (src2_num_digits == 0) {
            if (res != src1) {
                __digit_array_copy(res, src1, src1_num_digits);
            }

            res_num_digits = src1_num_digits;
            return;
        }

        /*
         * src1 must be greater than, or equal to src2.
         */
        if (__digit_array_compare_lt(src1, src1_num_digits, src2, src2_num_digits)) {
            throw not_a_number();
        }

        /*
         * Handle the various digit number permutations.
         */
        if (src2_num_digits == 1) {
            if (src1_num_digits == 1) {
                __digit_array_subtract_1_1(res, res_num_digits, src1, src2);
                return;
            }

            __digit_array_subtract_m_1(res, res_num_digits, src1, src1_num_digits, src2);
            return;
        }

        __digit_array_subtract_m_n(res, res_num_digits, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Left shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    INLINE auto digit_array_left_shift(natural_digit *res, std::size_t &res_num_digits,
                                       const natural_digit *src, std::size_t src_num_digits,
                                       std::size_t shift_digits, std::size_t shift_bits) -> void {
        /*
         * Are we shifting zero?  If yes, then our result is zero.
         */
        if (src_num_digits == 0) {
            res_num_digits = 0;
            return;
        }

        /*
         * Handle the various digit number permutations.
         */
        if (src_num_digits == 1) {
            __digit_array_left_shift_1(res, res_num_digits, src, shift_digits, shift_bits);
            return;
        }

        __digit_array_left_shift_m(res, res_num_digits, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Right shift a digit array.
     *
     * Note: It is OK for res and src to be the same pointer.
     */
    INLINE auto digit_array_right_shift(natural_digit *res, std::size_t &res_num_digits,
                                        const natural_digit *src, std::size_t src_num_digits,
                                        std::size_t shift_digits, std::size_t shift_bits) -> void {
        /*
         * Are we going to shift the result to zero?
         */
        if (src_num_digits <= shift_digits) {
            res_num_digits = 0;
            return;
        }

        /*
         * Handle the various digit number permutations.
         */
        if (src_num_digits == 1) {
            __digit_array_right_shift_1(res, res_num_digits, src, shift_bits);
            return;
        }

        __digit_array_right_shift_m(res, res_num_digits, src, src_num_digits, shift_digits, shift_bits);
    }

    /*
     * Multiply two digit arrays.
     *
     * Both src1_num_digits and src2_num_digits must be >= 1.
     */
    INLINE auto digit_array_multiply(natural_digit *res, std::size_t &res_num_digits,
                                     const natural_digit *src1, std::size_t src1_num_digits,
                                     const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Are we multiplying by zero?  If yes, the result is zero.
         */
        if (src2_num_digits == 0) {
            res_num_digits = 0;
            return;
        }

        /*
         * Are we multiplying zero?  If yes, the result is zero.
         */
        if (src1_num_digits == 0) {
            res_num_digits = 0;
            return;
        }

        /*
         * Handle the various digit number permutations.
         */
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

        __digit_array_multiply_m_n(res, res_num_digits, src1, src1_num_digits, src2, src2_num_digits);
    }

    /*
     * Divide two digit arrays.
     *
     * Both src1_num_digits and src2_num_digits must be >= 1.
     */
    INLINE auto digit_array_divide_modulus(natural_digit *quotient, std::size_t &quotient_num_digits,
                                           natural_digit *remainder, std::size_t &remainder_num_digits,
                                           const natural_digit *src1, std::size_t src1_num_digits,
                                           const natural_digit *src2, std::size_t src2_num_digits) -> void {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (src2_num_digits == 0) {
            throw divide_by_zero();
        }

        /*
         * Is our result going to be zero?
         */
        if (__digit_array_compare_lt(src1, src1_num_digits, src2, src2_num_digits)) {
            __digit_array_copy(remainder, src1, src1_num_digits);
            remainder_num_digits = src1_num_digits;
            quotient_num_digits = 0;
            return;
        }

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

