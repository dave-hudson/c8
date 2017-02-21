/*
 * natural.cpp
 */
#include <cctype>

#include "natural.h"

namespace c8 {
    const char digits_lower[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    const char digits_upper[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    /*
     * Construct a natural number from an unsigned long long integer.
     */
    natural::natural(unsigned long long v) {
        num_digits_ = 0;
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        digits_ = small_digits_;
        delete_on_final_ = false;

        if (!v) {
            return;
        }

        reserve(sizeof(unsigned long long) / sizeof(natural_digit));

        natural_digit *this_digits = digits_;

        std::size_t i = 0;
        do {
            natural_digit m = static_cast<natural_digit>(-1);
            this_digits[i++] = static_cast<natural_digit>(v & m);
            v >>= natural_digit_bits;
        } while (v);

        num_digits_ = i;
    }

    /*
     * Construct a natural number from a string.
     *
     * The string representation can have the usual C++-like hex, octal, or decimal
     * representations.
     */
    natural::natural(const std::string &v) {
        num_digits_ = 0;
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        digits_ = small_digits_;
        delete_on_final_ = false;

        std::size_t v_sz = v.size();
        if (C8_UNLIKELY(v_sz == 0)) {
            throw invalid_argument("zero size string");
        }

        natural res;

        /*
         * Is our first character a '0'?  If it is then we may have an octal or hex number.
         */
        std::size_t idx = 0;
        natural_digit base = 10;
        if (v[0] == '0') {
            idx = 1;
            base = 8;
            if (v_sz > 1) {
                auto c2 = v[1];
                if ((c2 == 'x') || (c2 == 'X')) {
                    idx = 2;
                    base = 16;
                }
            }
        }

        /*
         * We now know the base we're using and the starting index.
         */
        for (std::size_t i = idx; i < v_sz; i++) {
            char c = v[i];
            if (!isdigit(c)) {
                if (base != 16) {
                    throw invalid_argument("invalid digit");
                }

                if (!isxdigit(c)) {
                    throw invalid_argument("invalid digit");
                }

                c = static_cast<char>(tolower(c));
                c = static_cast<char>(c - ('a' - '0' - 10));
            }

            if (base == 8) {
                if (c >= '8') {
                    throw invalid_argument("invalid digit");
                }
            }

            res *= base;
            res += static_cast<natural_digit>(c - '0');
        }

        *this = std::move(res);
    }

    /*
     * Add a natural digit to this one.
     */
    auto natural::operator +(natural_digit v) const -> natural {
        std::size_t this_sz = num_digits_;

        /*
         * Is this number zero?  If yes then just construct the result.
         */
        if (C8_UNLIKELY(!this_sz)) {
            return natural(v);
        }

        natural res;
        res.reserve(this_sz + 1);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Add the first digit.
         */
        auto a = this_digits[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(v));
        res_digits[0] = static_cast<natural_digit>(acc);
        acc >>= natural_digit_bits;

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = acc + static_cast<natural_double_digit>(a);
            res_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (C8_UNLIKELY(acc)) {
            res_digits[this_sz++] = static_cast<natural_digit>(acc);
        }

        res.num_digits_ = this_sz;

        return res;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +(const natural &v) const -> natural {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

        const natural_digit *larger_digits;
        const natural_digit *smaller_digits;
        std::size_t larger_sz;
        std::size_t smaller_sz;
        if (this_sz >= v_sz) {
            larger_digits = digits_;
            smaller_digits = v.digits_;
            larger_sz = this_sz;
            smaller_sz = v_sz;
        } else {
            larger_digits = v.digits_;
            smaller_digits = digits_;
            larger_sz = v_sz;
            smaller_sz = this_sz;
        }

        natural res;
        res.reserve(larger_sz + 1);

        natural_digit *res_digits = res.digits_;

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_sz; i++) {
            auto a = smaller_digits[i];
            auto b = larger_digits[i];
            acc = acc + (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(b));
            res_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_sz; i < larger_sz; i++) {
            auto b = larger_digits[i];
            acc = acc + static_cast<natural_double_digit>(b);
            res_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (C8_UNLIKELY(acc)) {
            res_digits[larger_sz++] = static_cast<natural_digit>(acc);
        }

        res.num_digits_ = larger_sz;

        return res;
    }

    /*
     * Add a natural digit to this number.
     */
    auto natural::operator +=(natural_digit v) -> natural & {
        std::size_t this_sz = num_digits_;

        expand(this_sz + 1);

        natural_digit *this_digits = digits_;

        /*
         * Is this number zero?  If yes then just construct the result.
         */
        if (C8_UNLIKELY(!this_sz)) {
            this_digits[0] = v;
            num_digits_ = 1;
            return *this;
        }

        /*
         * Add the first digit.
         */
        natural_digit a = this_digits[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(v));
        this_digits[0] = static_cast<natural_digit>(acc);
        acc >>= natural_digit_bits;

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = acc + static_cast<natural_double_digit>(a);
            this_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (C8_UNLIKELY(acc)) {
            this_digits[this_sz++] = static_cast<natural_digit>(acc);
        }

        num_digits_ = this_sz;

        return *this;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +=(const natural &v) -> natural & {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;
        const natural_digit *larger_digits;
        const natural_digit *smaller_digits;
        std::size_t larger_sz;
        std::size_t smaller_sz;
        if (this_sz >= v_sz) {
            larger_digits = digits_;
            smaller_digits = v.digits_;
            larger_sz = this_sz;
            smaller_sz = v_sz;
        } else {
            larger_digits = v.digits_;
            smaller_digits = digits_;
            larger_sz = v_sz;
            smaller_sz = this_sz;
        }

        expand(larger_sz + 1);

        natural_digit *this_digits = digits_;

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_sz; i++) {
            auto a = smaller_digits[i];
            auto b = larger_digits[i];
            acc = acc + (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(b));
            this_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_sz; i < larger_sz; i++) {
            auto b = larger_digits[i];
            acc = acc + static_cast<natural_double_digit>(b);
            this_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (C8_UNLIKELY(acc)) {
            this_digits[larger_sz++] = static_cast<natural_digit>(acc);
        }

        num_digits_ = larger_sz;

        return *this;
    }

    /*
     * Subtract a natural digit from this natural number.
     */
    auto natural::operator -(natural_digit v) const -> natural {
        std::size_t this_sz = num_digits_;
        natural res;

        /*
         * Is this number zero?  If yes, then it's either an exception or a zero result.
         */
        if (C8_UNLIKELY(!this_sz)) {
            if (C8_LIKELY(!v)) {
                return res;
            }

            throw not_a_number();
        }

        res.reserve(this_sz);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Subtract the digits from this number's lowest digit.
         */
        auto a = this_digits[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(v));
        res_digits[0] = static_cast<natural_digit>(acc);
        acc = (acc >> natural_digit_bits) & 1;

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            res_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (C8_UNLIKELY(acc)) {
            throw not_a_number();
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t j = this_sz;
        if (!res_digits[j - 1]) {
            j--;
        }

        res.num_digits_ = j;

        return res;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -(const natural &v) const -> natural {
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

        natural res;

        /*
         * Is this number zero?  If yes, then it's either an exception or a zero result.
         */
        if (C8_UNLIKELY(!this_sz)) {
            if (C8_LIKELY(!v_sz)) {
                return res;
            }

            throw not_a_number();
        }

        res.reserve(this_sz);

        const natural_digit *this_digits = digits_;
        const natural_digit *v_digits = v.digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < v_sz; i++) {
            auto a = this_digits[i];
            auto b = v_digits[i];
            acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(b)) - acc;
            res_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = v_sz; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            res_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (C8_UNLIKELY(acc)) {
            throw not_a_number();
        }

        /*
         * Calculate our resulting digits.
         */
        std::size_t j = this_sz;
        while (j--) {
            if (res_digits[j]) {
                break;
            }
        }

        res.num_digits_ = j + 1;

        return res;
    }

    /*
     * Subtract a natural digit from this natural number.
     */
    auto natural::operator -=(natural_digit v) -> natural & {
        std::size_t this_sz = num_digits_;

        /*
         * Is this number zero?  If yes, then it's either an exception or a zero result.
         */
        if (C8_UNLIKELY(!this_sz)) {
            if (C8_LIKELY(!v)) {
                return *this;
            }

            throw not_a_number();
        }

        natural_digit *this_digits = digits_;

        /*
         * Subtract the digits from this number's lowest digit.
         */
        auto a = this_digits[0];
        natural_double_digit acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(v));
        this_digits[0] = static_cast<natural_digit>(acc);
        acc = (acc >> natural_digit_bits) & 1;

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = 1; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            this_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (C8_UNLIKELY(acc)) {
            throw not_a_number();
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t j = this_sz;
        if (!this_digits[j - 1]) {
            j--;
        }

        num_digits_ = j;

        return *this;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -=(const natural &v) -> natural & {
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

        natural_digit *this_digits = digits_;
        const natural_digit *v_digits = v.digits_;

        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < v_sz; i++) {
            auto a = this_digits[i];
            auto b = v_digits[i];
            acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(b)) - acc;
            this_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = v_sz; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            this_digits[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (C8_UNLIKELY(acc)) {
            throw not_a_number();
        }

        /*
         * Calculate our resulting digits.
         */
        std::size_t j = this_sz;
        while (j--) {
            if (this_digits[j]) {
                break;
            }
        }

        num_digits_ = j + 1;

        return *this;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<(unsigned int count) const -> natural {
        std::size_t this_sz = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;

        /*
         * If our value is zero then return 0.
         */
        if (C8_UNLIKELY(!this_sz)) {
            return res;
        }

        std::size_t new_sz = this_sz + trailing_digits;
        res.reserve(new_sz + 1);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(digit_shift == 0)) {
            res.num_digits_ = new_sz;
            copy_digit_array(&res_digits[trailing_digits], digits_, this_sz);
            zero_digit_array(res_digits, trailing_digits);

            return res;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        natural_double_digit acc = (static_cast<natural_double_digit>(this_digits[this_sz - 1]) << digit_shift);
        if (acc >> natural_digit_bits) {
            res_digits[new_sz++] = static_cast<natural_digit>(acc >> natural_digit_bits);
        }

        for (std::size_t i = this_sz - 1; i > 0; i--) {
            acc <<= natural_digit_bits;
            acc |= (static_cast<natural_double_digit>(this_digits[i - 1]) << digit_shift);
            res_digits[i + trailing_digits] = static_cast<natural_digit>(acc >> natural_digit_bits);
        }

        res_digits[trailing_digits] = static_cast<natural_digit>(acc);
        res.num_digits_ = new_sz;

        zero_digit_array(res_digits, trailing_digits);

        return res;
    }

    /*
     * Right shift this natural number by a bit count.
     */
    auto natural::operator >>(unsigned int count) const -> natural {
        std::size_t this_sz = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;

        /*
         * If our shift is more than the total number of bits that we had then return 0.
         */
        if (C8_UNLIKELY(this_sz <= trailing_digits)) {
            return res;
        }

        std::size_t new_sz = this_sz - (trailing_digits + 1);
        res.reserve(new_sz + 1);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Are we shifting by whole digits?
         */
        if (C8_UNLIKELY(digit_shift == 0)) {
            new_sz++;
            res.num_digits_ = new_sz;
            copy_digit_array(res_digits, &digits_[trailing_digits], new_sz);

            return res;
        }

        /*
         * Shift the original value and insert in the result.
         */
        natural_double_digit acc = static_cast<natural_double_digit>(this_digits[trailing_digits]) >> digit_shift;
        for (std::size_t i = 1; i <= new_sz; i++) {
            acc |= (static_cast<natural_double_digit>(this_digits[i + trailing_digits]) << (natural_digit_bits - digit_shift));
            res_digits[i - 1] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res_digits[new_sz++] = static_cast<natural_digit>(acc);
        }

        res.num_digits_ = new_sz;

        return res;
    }

    /*
     * Multiply this natural number with a single digit.
     */
    auto natural::operator *(natural_digit v) const -> natural {
        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_sz = num_digits_;
        if (C8_UNLIKELY(!this_sz)) {
            natural res;
            return res;
        }

        if (C8_UNLIKELY(!v)) {
            natural res;
            return res;
        }

        /*
         * Estimate our output size in digits.
         */
        natural res;
        res.reserve(this_sz + 1);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Long multiply.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = acc + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(v));
            res_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res_digits[this_sz++] = static_cast<natural_digit>(acc);
        }

        res.num_digits_ = this_sz;

        return res;
    }

    /*
     * Multiply this natural number with another one.
     */
    auto natural::operator *(const natural &v) const -> natural {
        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_sz = num_digits_;
        if (C8_UNLIKELY(!this_sz)) {
            natural res;
            return res;
        }

        std::size_t v_sz = v.num_digits_;
        if (C8_UNLIKELY(!v_sz)) {
            natural res;
            return res;
        }

        const natural_digit *v_digits = v.digits_;

        /*
         * Are we multiplying by a single digit?  If yes, then use the fast version
         */
        if (C8_UNLIKELY(v_sz == 1)) {
            return *this * v_digits[0];
        }

        /*
         * Estimate our output size in digits.
         */
        std::size_t res_sz = this_sz + v_sz;

        natural res;
        res.reserve(res_sz);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Comba multiply.
         */
        natural_digit c0 = 0;
        natural_digit c1 = 0;
        natural_digit c2 = 0;
        std::size_t tj_lim = v_sz - 1;

        for (std::size_t i = 0; i < res_sz; i++) {
            std::size_t tj = (i < tj_lim) ? i : tj_lim;
            std::size_t ti = i - tj;
            std::size_t j_lim = ((this_sz - ti) < (tj + 1)) ? (this_sz - ti) : (tj + 1);
            c0 = c1;
            c1 = c2;
            c2 = 0;
            for (std::size_t j = 0; j < j_lim; j++) {
                natural_digit a = this_digits[ti++];
                natural_digit b = v_digits[tj--];
                natural_double_digit d0 = static_cast<natural_double_digit>(c0) + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(b));
                c0 = static_cast<natural_digit>(d0);
                natural_double_digit d1 = static_cast<natural_double_digit>(c1) + (d0 >> natural_digit_bits);
                c1 = static_cast<natural_digit>(d1);
                c2 += static_cast<natural_digit>((d1 >> natural_digit_bits));
            }

            res_digits[i] = c0;
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t j = res_sz;
        if (!res_digits[j - 1]) {
            j--;
        }

        res.num_digits_ = j;

        return res;
    }

    /*
     * Multiply this natural number with a single digit.
     */
    auto natural::operator *=(natural_digit v) -> natural & {
        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_sz = num_digits_;
        if (C8_UNLIKELY(!this_sz)) {
            return *this;
        }

        if (C8_UNLIKELY(!v)) {
            num_digits_ = 0;
            return *this;
        }

        /*
         * Estimate our output size in digits.
         */
        expand(this_sz + 1);

        natural_digit *this_digits = digits_;

        /*
         * Long multiply.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < this_sz; i++) {
            auto a = this_digits[i];
            acc = acc + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(v));
            this_digits[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            this_digits[this_sz++] = static_cast<natural_digit>(acc);
        }

        num_digits_ = this_sz;

        return *this;
    }

    /*
     * Divide this natural number by a natural digit, returning the quotient and modulus.
     */
    auto natural::divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit> {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (C8_UNLIKELY(!v)) {
            throw divide_by_zero();
        }

        natural res;
        std::size_t this_sz = num_digits_;
        if (C8_UNLIKELY(!this_sz)) {
            return std::make_pair(std::move(res), 0);
        }

        res.reserve(this_sz);

        const natural_digit *this_digits = digits_;
        natural_digit *res_digits = res.digits_;

        /*
         * Now we run a long divide algorithm.
         */
        natural_double_digit acc = 0;
        std::size_t i = this_sz;
        while (i--) {
            auto a = this_digits[i];
            acc = static_cast<natural_double_digit>(acc << natural_digit_bits) + static_cast<natural_double_digit>(a);
            natural_double_digit q = acc / v;
            acc = acc % v;
            res_digits[i] = static_cast<natural_digit>(q);
        }

        /*
         * We may have a zero upper digit so account for this.
         */
        std::size_t j = this_sz;
        if (!res_digits[j - 1]) {
            j--;
        }

        res.num_digits_ = j;

        return std::make_pair(std::move(res), static_cast<natural_digit>(acc));
    }

    /*
     * Divide this natural number by another one, returning the quotient and modulus.
     */
    auto natural::divide_modulus(const natural &v) const -> std::pair<natural, natural> {
        natural res;

        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (C8_UNLIKELY(!v.num_digits_)) {
            throw divide_by_zero();
        }

        /*
         * Are we dividing by a single digit?  If yes, then use the fast version
         * of divide_modulus!
         */
        if (C8_UNLIKELY(v.num_digits_ == 1)) {
            return divide_modulus(v.digits_[0]);
        }

        /*
         * Is the result zero?  If yes then we're done.
         */
        if (C8_UNLIKELY(*this < v)) {
            return std::make_pair(std::move(res), *this);
        }

        /*
         * Normalize the divisor and quotient.  We want our quotient to be aligned such
         * that its top bit is set within a natural_digit.  This may seem a little odd, but
         * we want to ensure that any quotient estimation is as accurate as possible.
         */
        auto divisor_bits = v.count_bits();
        auto divisor_digit_bits = divisor_bits & (natural_digit_bits - 1);
        unsigned int normalize_shift = static_cast<unsigned int>((natural_digit_bits - divisor_digit_bits) & (natural_digit_bits - 1));
        natural remaining = *this << normalize_shift;
        natural divisor = v << normalize_shift;

        std::size_t remaining_sz = remaining.num_digits_;
        std::size_t divisor_sz = divisor.num_digits_;

        res.reserve(remaining_sz - divisor_sz + 1);

        const natural_digit *divisor_digits = divisor.digits_;
        const natural_digit *remaining_digits = remaining.digits_;
        natural_digit *res_digits = res.digits_;
        zero_digit_array(res_digits, (remaining_sz - divisor_sz + 1));

        /*
         * Now we run a long divide algorithm.
         */
        std::size_t i = remaining_sz;
        auto upper_div_digit = divisor_digits[divisor_sz - 1];
        while (true) {
            /*
             * Iterate through our dividend from the most significant digit to the least.
             */
            i--;

            /*
             * We know that our divisor has been shifted so that the most significant digit has
             * its top bit set.  This means that the quotient for our next digit can only be 0 or 1.
             * If we compare the most significant digit of our remainder with that of the divisor
             * we can see if it's possibly 1.
             */
            auto d_hi = remaining_digits[i];
            if (d_hi >= upper_div_digit) {
                /*
                 * Our next quotient digit is probably a 1, but we have to be sure.  It's possible
                 * that the subsequent digits of the divisor are large enough that it's actually
                 * still zero, but in that case our next digit will be as large as it can be.
                 */
                natural_digit q = 1;
                natural m = divisor << static_cast<unsigned int>((i - divisor_sz + 1) * natural_digit_bits);
                if (remaining >= m) {
                    /*
                     * Our result was 1.  While we now know this digit, subtracting "m" may
                     * still leave us with a non-zero digit so we want to re-evaluate this
                     * one again.
                     */
                    i++;
                } else {
                    /*
                     * Our digit was actually 0 after all, so we know definitively that the next
                     * digit is it's maximum possible size.
                     */
                    q = static_cast<natural_digit>(-1);
                    m -= (divisor << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits));
                }

                res_digits[i - divisor_sz] = q;
                remaining -= m;
                if (remaining < divisor) {
                    break;
                }

                if (i > remaining.num_digits_) {
                    i = remaining.num_digits_;
                }

                continue;
            }

            /*
             * Estimate the next digit of the result.
             */
            natural_double_digit d_lo_d = static_cast<natural_double_digit>(remaining_digits[i - 1]);
            natural_double_digit d_hi_d = static_cast<natural_double_digit>(d_hi);
            natural_double_digit d = static_cast<natural_double_digit>(d_hi_d << natural_digit_bits) + d_lo_d;
            natural_digit q = static_cast<natural_digit>(d / static_cast<natural_double_digit>(upper_div_digit));

            natural m = (divisor * q) << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits);

            /*
             * It's possible that our estimate might be slightly too large, so we have
             * to evaluate it on the basis of the full divisor, not just the shifted, most
             * significant digit.  This may mean we reduce our estimate slightly.
             */
            if (m > remaining) {
                m -= (divisor << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits));
                q--;
            }

            res_digits[i - divisor_sz] = q;
            remaining -= m;
            if (remaining < divisor) {
                break;
            }

            if (i > remaining.num_digits_) {
                i = remaining.num_digits_;
            }
        }

        remaining >>= normalize_shift;

        /*
         * Calculate our resulting digits.
         */
        std::size_t j = remaining_sz - divisor_sz + 1;
        while (j--) {
            if (res_digits[j]) {
                break;
            }
        }

        res.num_digits_ = j + 1;

        return std::make_pair(std::move(res), std::move(remaining));
    }

    /*
     * Compare this natural number with another one.
     */
    auto natural::compare(const natural &v) const noexcept -> comparison {
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

    /*
     * Find the greatest common divisor of this and another natural number.
     */
    auto natural::gcd(const natural &v) const -> natural {
        if (C8_UNLIKELY(v.is_zero())) {
            return *this;
        }

        if (C8_UNLIKELY(is_zero())) {
            return v;
        }

        natural smaller;
        natural larger;
        if (*this < v) {
            smaller = *this;
            larger = v;
        } else {
            smaller = v;
            larger = *this;
        }

        do {
            natural mod = larger % smaller;
            larger = std::move(smaller);
            smaller = std::move(mod);
        } while (!smaller.is_zero());

        return larger;
    }

    /*
     * Convert this natural number to an unsigned long long.
     */
    auto natural::to_unsigned_long_long() const -> unsigned long long {
        /*
         * Will this number fit in an unsigned long long?  If not then throw an
         * exception.
         */
        if (C8_UNLIKELY(count_bits() > (8 * sizeof(long long)))) {
            throw overflow_error();
        }

        /*
         * Convert the value to our result format.
         */
        unsigned long long res = 0;
        std::size_t sz = num_digits_;
        if (sz > (sizeof(unsigned long long) / sizeof(natural_digit))) {
            sz = sizeof(unsigned long long) / sizeof(natural_digit);
        }

        const natural_digit *this_digits = digits_;
        for (unsigned int i = 0; i < sz; i++) {
            res |= (static_cast<unsigned long long>(this_digits[i]) << (i * natural_digit_bits));
        }

        return res;
    }

    /*
     * << operator to print a natural.
     */
    auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream & {
        std::size_t v_sz = v.num_digits_;
        if (v_sz == 0) {
            outstr << '0';
            return outstr;
        }

        natural_digit base = 10;
        auto flags = outstr.flags();
        if (flags & std::ios_base::hex) {
            base = 16;
        } else if (flags & std::ios_base::oct) {
            base = 8;
        }

        const char *digits = digits_lower;
        if (flags & std::ios_base::uppercase) {
            digits = digits_upper;
        }

        if (flags & std::ios_base::showbase) {
            if (base != 10) {
                outstr << '0';
                if (base == 16) {
                    outstr << ((flags & std::ios_base::uppercase) ? 'X' : 'x');
                }
            }
        }

        std::vector<char> res;
        auto rem = v;
        do {
            std::pair<natural, natural_digit> qm = rem.divide_modulus(base);
            natural_digit mod = qm.second;
            res.emplace_back(digits[mod]);

            rem = std::move(qm.first);
        } while (!is_zero(rem));

        std::size_t res_sz = res.size();
        for (std::size_t i = 0; i < res_sz; i++) {
            outstr << res[res_sz - 1 - i];
        }

        return outstr;
    }
}

