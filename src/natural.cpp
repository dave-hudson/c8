/*
 * natural.cpp
 */
#include <cctype>
#include <vector>

#include "c8.h"
#include "digit_array.h"

namespace c8 {
    const char digits_lower[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    const char digits_upper[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    /*
     * Reserve a number of digits in this natural number.
     */
    inline auto natural::reserve(std::size_t new_digits) -> void {
        /*
         * If our digit array is already large enough (which is very likely) then
         * we don't need to do anything.
         */
        if (C8_LIKELY(digits_size_ >= new_digits)) {
            return;
        }

        /*
         * Allocate a new digit array and update book-keeping info.
         */
        digits_size_ = new_digits;
        large_digits_ = std::make_unique<natural_digit[]>(new_digits);
        digits_ = large_digits_.get();
    }

    /*
     * Expand the number of digits in this natural number.
     */
    inline auto natural::expand(std::size_t new_digits) -> void {
        /*
         * If our digit array is already large enough (which is very likely) then
         * we don't need to do anything.
         */
        if (C8_LIKELY(digits_size_ >= new_digits)) {
            return;
        }

        /*
         * Replace the old digit array with the new one.
         */
        auto d = std::make_unique<natural_digit[]>(new_digits);
        auto d_ptr = d.get();
        digit_array_copy(d_ptr, digits_, num_digits_);

        digits_size_ = new_digits;
        large_digits_ = std::move(d);
        digits_ = d_ptr;
    }

    /*
     * Copy the contents of a natural number into this one.
     */
    auto natural::copy_digits(const natural &v) -> void {
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        digits_ = small_digits_;
        num_digits_ = v.num_digits_;
        if (C8_UNLIKELY(!num_digits_)) {
            return;
        }

        reserve(v.num_digits_);
        digit_array_copy(digits_, v.digits_, num_digits_);
    }

    /*
     * Steal the contents of a natural number into this one.
     */
    auto natural::steal_digits(natural &v) -> void {
        large_digits_ = std::move(v.large_digits_);

        /*
         * Are we currently using the default small buffer, or do we have one allocated?
         */
        if (C8_UNLIKELY(large_digits_.get())) {
            /*
             * We're using an allocated buffer so just move it.
             */
            digits_ = v.digits_;
            v.digits_ = v.small_digits_;
            digits_size_ = v.digits_size_;
            v.digits_size_ = sizeof(v.small_digits_) / sizeof(natural_digit);
        } else {
            /*
             * We're using the default buffer so copy the contents.
             */
            digit_array_copy(small_digits_, v.small_digits_, v.num_digits_);
            digits_ = small_digits_;
            digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        }

        num_digits_ = v.num_digits_;
        v.num_digits_ = 0;
    }

    /*
     * Construct a natural number from an unsigned long long integer.
     */
    natural::natural(unsigned long long v) noexcept {
        num_digits_ = 0;
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        digits_ = small_digits_;

        if (!v) {
            return;
        }

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
     * Copy constructor.
     */
    natural::natural(const natural &v) {
        copy_digits(v);
    }

    /*
     * Move constructor.
     */
    natural::natural(natural &&v) noexcept {
        steal_digits(v);
    }

    /*
     * Return true if this number is equal to another one, false if it's not.
     */
    auto natural::operator ==(const natural &v) const noexcept -> bool {
        return digit_array_compare_eq(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Return true if this number is not equal to another one, false if it's equal.
     */
    auto natural::operator !=(const natural &v) const noexcept -> bool {
        return digit_array_compare_ne(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Return true if this number is greater than another one, false if it's not.
     */
    auto natural::operator >(const natural &v) const noexcept -> bool {
        return digit_array_compare_gt(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Return true if this number is greater than, or equal to, another one, false if it's not.
     */
    auto natural::operator >=(const natural &v) const noexcept -> bool {
        return digit_array_compare_ge(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Return true if this number is less than another one, false if it's not.
     */
    auto natural::operator <(const natural &v) const noexcept -> bool {
        return digit_array_compare_lt(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Return true if this number is less than, or equal to, another one, false if it's not.
     */
    auto natural::operator <=(const natural &v) const noexcept -> bool {
        return digit_array_compare_le(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Copy assignment operator.
     */
    auto natural::operator =(const natural &v) -> natural & {
        /*
         * Are we assigning to ourself?  If we are then we don't need to do anything.
         */
        if (C8_UNLIKELY(this == &v)) {
            return *this;
        }

        /*
         * Delete the old contents of this natural number and copy the original's digits.
         */
        copy_digits(v);
        return *this;
    }

    /*
     * Move assignment operator.
     */
    auto natural::operator =(natural &&v) noexcept -> natural & {
        /*
         * Are we assigning to ourself?  If we are then we don't have to do anything.
         */
        if (C8_UNLIKELY(this == &v)) {
            return *this;
        }

        /*
         * Delete the old contents of this natural number and steal the original's digits.
         */
        steal_digits(v);
        return *this;
    }

    /*
     * Return the number of bits required to represent this natural number.
     */
    auto natural::size_bits() const noexcept -> std::size_t {
        return digit_array_size_bits(digits_, num_digits_);
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +(const natural &v) const -> natural {
        natural res;

        /*
         * Does v have zero digits?  If yes, then our result is just this number.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            res = *this;
            return res;
        }

        /*
         * Does this number have zero digits?  If yes, then our result is just v.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            res = v;
            return res;
        }

        /*
         * We need to reserve some space for our result.  Our result can only be
         * one digit larger than the largest number we have as an operand.
         */
        std::size_t max_num_digits = (v_num_digits > this_num_digits) ? v_num_digits : this_num_digits;
        res.reserve(max_num_digits + 1);
        res.num_digits_ = digit_array_add(res.digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return res;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +=(const natural &v) -> natural & {
        /*
         * Does v have zero digits?  If yes, then our result is just this number.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            return *this;
        }

        /*
         * Does this number have zero digits?  If yes, then our result is just v.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            *this = v;
            return *this;
        }

        /*
         * We need to reserve some space for our result.  Our result can only be
         * one digit larger than the largest number we have as an operand.
         */
        std::size_t max_num_digits = (v_num_digits > this_num_digits) ? v_num_digits : this_num_digits;
        expand(max_num_digits + 1);
        num_digits_ = digit_array_add(digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return *this;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -(const natural &v) const -> natural {
        natural res;

        /*
         * Does v have zero digits?  If yes, then our result is just this number.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            res = *this;
            return res;
        }

        /*
         * Does this number have zero digits?  If yes, then we have an exception.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            throw not_a_number();
        }

        res.reserve(this_num_digits);
        res.num_digits_ = digit_array_subtract(res.digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return res;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -=(const natural &v) -> natural & {
        /*
         * Does v have zero digits?  If yes, then our result is just this number.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            return *this;
        }

        /*
         * Does this number have zero digits?  If yes, then we have an exception.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            throw not_a_number();
        }

        num_digits_ = digit_array_subtract(digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return *this;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<(unsigned int count) const -> natural {
        natural res;

        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            return res;
        }

        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        res.reserve(this_num_digits + trailing_digits + 1);
        res.num_digits_ = digit_array_left_shift(res.digits_, digits_, this_num_digits, trailing_digits, digit_shift);
        return res;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<=(unsigned int count) -> natural & {
        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            return *this;
        }

        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        expand(this_num_digits + trailing_digits + 1);
        num_digits_ = digit_array_left_shift(digits_, digits_, this_num_digits, trailing_digits, digit_shift);
        return *this;
    }

    /*
     * Right shift this natural number by a bit count.
     */
    auto natural::operator >>(unsigned int count) const -> natural {
        natural res;

        std::size_t this_num_digits = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        /*
         * Does this number have fewer digits than we want to shift by?  If yes, then our result is zero.
         */
        if (this_num_digits <= trailing_digits) {
            return res;
        }

        res.reserve(this_num_digits - trailing_digits);
        res.num_digits_ = digit_array_right_shift(res.digits_, digits_, this_num_digits, trailing_digits, digit_shift);
        return res;
    }

    /*
     * Right shift this natural number by a bit count.
     */
    auto natural::operator >>=(unsigned int count) -> natural & {
        std::size_t this_num_digits = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        /*
         * Does this number have fewer digits than we want to shift by?  If yes, then our result is zero.
         */
        if (this_num_digits <= trailing_digits) {
            num_digits_ = 0;
            return *this;
        }

        num_digits_ = digit_array_right_shift(digits_, digits_, this_num_digits, trailing_digits, digit_shift);
        return *this;
    }

    /*
     * Multiply this natural number with another one.
     */
    auto natural::operator *(const natural &v) const -> natural {
        natural res;

        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            return res;
        }

        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            return res;
        }

        std::size_t res_num_digits = this_num_digits + v_num_digits;
        res.reserve(res_num_digits);
        res.num_digits_ = digit_array_multiply(res.digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return res;
    }

    /*
     * Multiply this natural number with another one.
     */
    auto natural::operator *=(const natural &v) -> natural & {
        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            return *this;
        }

        /*
         * Does this number have zero digits?  If yes, then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (!this_num_digits) {
            return *this;
        }

        std::size_t res_num_digits = this_num_digits + v_num_digits;
        expand(res_num_digits);
        num_digits_ = digit_array_multiply(digits_, digits_, this_num_digits, v.digits_, v_num_digits);
        return *this;
    }

    /*
     * Divide this natural number by another one, returning the quotient and remainder.
     */
    auto natural::divide_modulus(const natural &v) const -> std::pair<natural, natural> {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            throw divide_by_zero();
        }

        std::pair<natural, natural> p;

        /*
         * Is the result zero?  If yes then we're done.
         */
        std::size_t this_num_digits = num_digits_;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            p.second = *this;
            return p;
        }

        p.first.reserve(this_num_digits - v_num_digits + 1);
        p.second.reserve(v_num_digits);
        digit_array_divide_modulus(p.first.digits_, p.first.num_digits_, p.second.digits_, p.second.num_digits_,
                                   digits_, this_num_digits, v.digits_, v_num_digits);
        return p;
    }

    /*
     * Divide this natural number by another one, returning the quotient.
     */
    auto natural::operator /(const natural &v) const -> natural {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            throw divide_by_zero();
        }

        natural quotient;

        /*
         * Is the result zero?  If yes then we're done.
         */
        std::size_t this_num_digits = num_digits_;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            return quotient;
        }

        quotient.reserve(this_num_digits - v_num_digits + 1);
        natural_digit remainder_digits[v_num_digits];
        std::size_t remainder_num_digits;
        digit_array_divide_modulus(quotient.digits_, quotient.num_digits_, remainder_digits, remainder_num_digits,
                                   digits_, this_num_digits, v.digits_, v_num_digits);
        return quotient;
    }

    /*
     * Divide this natural number by another one, returning the quotient.
     */
    auto natural::operator /=(const natural &v) -> natural & {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            throw divide_by_zero();
        }

        /*
         * Is the result zero?  If yes then we're done.
         */
        std::size_t this_num_digits = num_digits_;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            num_digits_ = 0;
            return *this;
        }

        natural_digit remainder_digits[v_num_digits];
        std::size_t remainder_num_digits;
        digit_array_divide_modulus(digits_, num_digits_, remainder_digits, remainder_num_digits,
                                   digits_, this_num_digits, v.digits_, v_num_digits);
        return *this;
    }

    /*
     * Divide this natural number by another one, returning the remainder.
     */
    auto natural::operator %(const natural &v) const -> natural {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            throw divide_by_zero();
        }

        natural remainder;

        /*
         * Is the result zero?  If yes then we're done.
         */
        std::size_t this_num_digits = num_digits_;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            remainder = *this;
            return remainder;
        }

        natural_digit quotient_digits[this_num_digits - v_num_digits + 1];
        std::size_t quotient_num_digits;
        remainder.reserve(v_num_digits);
        digit_array_divide_modulus(quotient_digits, quotient_num_digits, remainder.digits_, remainder.num_digits_,
                                   digits_, this_num_digits, v.digits_, v_num_digits);
        return remainder;
    }

    /*
     * Divide this natural number by another one, returning the remainder.
     */
    auto natural::operator %=(const natural &v) -> natural & {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        std::size_t v_num_digits = v.num_digits_;
        if (!v_num_digits) {
            throw divide_by_zero();
        }

        /*
         * Is the result zero?  If yes then we're done.
         */
        std::size_t this_num_digits = num_digits_;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            return *this;
        }

        natural_digit quotient_digits[this_num_digits - v_num_digits + 1];
        std::size_t quotient_num_digits;
        digit_array_divide_modulus(quotient_digits, quotient_num_digits, digits_, num_digits_,
                                   digits_, this_num_digits, v.digits_, v_num_digits);
        return *this;
    }

    /*
     * Find the greatest common divisor of this and another natural number.
     */
    auto natural::gcd(const natural &v) const -> natural {
        natural smaller;

        std::size_t v_num_digits = v.num_digits_;
        if (!v.num_digits_) {
            smaller = *this;
            return smaller;
        }

        std::size_t this_num_digits = num_digits_;
        if (!num_digits_) {
            smaller = v;
            return smaller;
        }

        natural larger;
        if (digit_array_compare_lt(digits_, this_num_digits, v.digits_, v_num_digits)) {
            smaller = *this;
            larger = v;
        } else {
            smaller = v;
            larger = *this;
        }

        natural_digit quotient_digits[larger.num_digits_ - smaller.num_digits_ + 1];

        while (true) {
            /*
             * Essentially mod = larger % smaller, larger = smaller, smaller = mod.
             */
            natural mod;
            mod.reserve(smaller.num_digits_);

            std::size_t quotient_num_digits;
            digit_array_divide_modulus(quotient_digits, quotient_num_digits, mod.digits_, mod.num_digits_,
                                       larger.digits_, larger.num_digits_, smaller.digits_, smaller.num_digits_);
            if (!mod.num_digits_) {
                break;
            }

            larger.steal_digits(smaller);
            smaller.steal_digits(mod);
        }

        return smaller;
    }

    /*
     * Convert this natural number to an unsigned long long.
     */
    auto natural::to_unsigned_long_long() const -> unsigned long long {
        /*
         * Will this number fit in an unsigned long long?  If not then throw an
         * exception.
         */
        if (size_bits() > (8 * sizeof(long long))) {
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
        std::size_t v_num_digits = v.num_digits_;
        if (v_num_digits == 0) {
            outstr << '0';
            return outstr;
        }

        unsigned int base = 10;
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

        natural nbase = base;

        std::vector<char> res;
        auto rem = v;
        do {
            std::pair<natural, natural> qm = rem.divide_modulus(nbase);
            unsigned int d = 0;
            if (!qm.second.is_zero()) {
                d = qm.second.digits_[0];
            }

            res.emplace_back(digits[d]);

            rem = std::move(qm.first);
        } while (!rem.is_zero());

        std::size_t res_sz = res.size();
        for (std::size_t i = 0; i < res_sz; i++) {
            outstr << res[res_sz - 1 - i];
        }

        return outstr;
    }
}

