/*
 * natural.cpp
 */
#include <cctype>

#include "natural.h"
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
        if (C8_LIKELY(digits_size_ >= new_digits)) {
            return;
        }

        digits_size_ = new_digits;
        delete_digits_on_final_ = true;
        digits_ = new natural_digit[new_digits];
    }

    /*
     * Expand the number of digits in this natural number.
     */
    inline auto natural::expand(std::size_t new_digits) -> void {
        if (C8_LIKELY(digits_size_ >= new_digits)) {
            return;
        }

        auto d = new natural_digit[new_digits];
        copy_digit_array(d, digits_, num_digits_);

        delete_digits();
        digits_size_ = new_digits;
        delete_digits_on_final_ = true;
        digits_ = d;
    }

    /*
     * Copy the contents of a natural number into this one.
     */
    auto natural::copy_digits(const natural &v) -> void {
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        delete_digits_on_final_ = false;
        digits_ = small_digits_;
        num_digits_ = v.num_digits_;
        if (C8_UNLIKELY(!num_digits_)) {
            return;
        }

        reserve(v.num_digits_);
        copy_digit_array(digits_, v.digits_, num_digits_);
    }

    /*
     * Steal the contents of a natural number into this one.
     */
    auto natural::steal_digits(natural &v) -> void {
        /*
         * Are we currently using the default small buffer?  If we are then we
         * need to deep copy it.
         */
        if (C8_LIKELY(v.digits_ == v.small_digits_)) {
            copy_digit_array(small_digits_, v.small_digits_, v.num_digits_);
            digits_ = small_digits_;
        } else {
            /*
             * We aren't using the default buffer so we can shallow copy instead.
             */
            digits_ = v.digits_;
        }

        num_digits_ = v.num_digits_;
        v.num_digits_ = 0;
        digits_size_ = v.digits_size_;
        v.digits_size_ = sizeof(v.small_digits_) / sizeof(natural_digit);
        v.digits_ = v.small_digits_;
        delete_digits_on_final_ = v.delete_digits_on_final_;
        v.delete_digits_on_final_ = false;
    }

    /*
     * Construct a natural number from an unsigned long long integer.
     */
    natural::natural(unsigned long long v) {
        num_digits_ = 0;
        digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
        digits_ = small_digits_;
        delete_digits_on_final_ = false;

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
        delete_digits_on_final_ = false;

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
     * Compare this natural number with another one.
     */
    auto natural::compare(const natural &v) const noexcept -> comparison {
        return compare_digit_arrays(digits_, num_digits_, v.digits_, v.num_digits_);
    }

    /*
     * Add a natural digit to this one.
     */
    auto natural::operator +(natural_digit v) const -> natural {
        std::size_t this_num_digits = num_digits_;

        natural res;
        res.reserve(this_num_digits + 1);
        res.num_digits_ = add_digit_array_digit(res.digits_, digits_, this_num_digits, v);

        return res;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +(const natural &v) const -> natural {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_num_digits = num_digits_;
        std::size_t v_num_digits = v.num_digits_;

        const natural_digit *larger_digits;
        const natural_digit *smaller_digits;
        std::size_t larger_num_digits;
        std::size_t smaller_num_digits;
        if (this_num_digits >= v_num_digits) {
            larger_digits = digits_;
            smaller_digits = v.digits_;
            larger_num_digits = this_num_digits;
            smaller_num_digits = v_num_digits;
        } else {
            larger_digits = v.digits_;
            smaller_digits = digits_;
            larger_num_digits = v_num_digits;
            smaller_num_digits = this_num_digits;
        }

        natural res;
        res.reserve(larger_num_digits + 1);
        res.num_digits_ = add_digit_arrays(res.digits_, larger_digits, larger_num_digits, smaller_digits, smaller_num_digits);

        return res;
    }

    /*
     * Add a natural digit to this number.
     */
    auto natural::operator +=(natural_digit v) -> natural & {
        std::size_t this_num_digits = num_digits_;

        expand(this_num_digits + 1);
        num_digits_ = add_digit_array_digit(digits_, digits_, this_num_digits, v);

        return *this;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +=(const natural &v) -> natural & {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_num_digits = num_digits_;
        std::size_t v_num_digits = v.num_digits_;
        const natural_digit *larger_digits;
        const natural_digit *smaller_digits;
        std::size_t larger_num_digits;
        std::size_t smaller_num_digits;
        if (this_num_digits >= v_num_digits) {
            larger_digits = digits_;
            smaller_digits = v.digits_;
            larger_num_digits = this_num_digits;
            smaller_num_digits = v_num_digits;
        } else {
            larger_digits = v.digits_;
            smaller_digits = digits_;
            larger_num_digits = v_num_digits;
            smaller_num_digits = this_num_digits;
        }

        expand(larger_num_digits + 1);
        num_digits_ = add_digit_arrays(digits_, larger_digits, larger_num_digits, smaller_digits, smaller_num_digits);

        return *this;
    }

    /*
     * Subtract a natural digit from this natural number.
     */
    auto natural::operator -(natural_digit v) const -> natural {
        std::size_t this_num_digits = num_digits_;

        /*
         * We should not have a negative result!
         */
        if (C8_UNLIKELY(this_num_digits == 1)) {
            if (C8_UNLIKELY(digits_[0] < v)) {
                throw not_a_number();
            }
        } else if (C8_UNLIKELY(!this_num_digits)) {
            if (C8_UNLIKELY(v)) {
                throw not_a_number();
            }
        }

        natural res;
        res.reserve(this_num_digits);
        res.num_digits_ = subtract_digit_array_digit(res.digits_, digits_, this_num_digits, v);

        return res;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -(const natural &v) const -> natural {
        std::size_t this_num_digits = num_digits_;
        std::size_t v_num_digits = v.num_digits_;

        /*
         * We should not have a negative result!
         */
        if (C8_UNLIKELY(compare_digit_arrays(digits_, this_num_digits, v.digits_, v_num_digits) == comparison::lt)) {
            throw not_a_number();
        }

        natural res;
        res.reserve(this_num_digits);
        res.num_digits_ = subtract_digit_arrays(res.digits_, digits_, this_num_digits, v.digits_, v_num_digits);

        return res;
    }

    /*
     * Subtract a natural digit from this natural number.
     */
    auto natural::operator -=(natural_digit v) -> natural & {
        std::size_t this_num_digits = num_digits_;

        /*
         * We should not have a negative result!
         */
        if (C8_UNLIKELY(this_num_digits == 1)) {
            if (C8_UNLIKELY(digits_[0] < v)) {
                throw not_a_number();
            }
        } else if (C8_UNLIKELY(!this_num_digits)) {
            if (C8_UNLIKELY(v)) {
                throw not_a_number();
            }
        }

        num_digits_ = subtract_digit_array_digit(digits_, digits_, this_num_digits, v);

        return *this;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -=(const natural &v) -> natural & {
        std::size_t this_num_digits = num_digits_;
        std::size_t v_num_digits = v.num_digits_;

        /*
         * We should not have a negative result!
         */
        if (C8_UNLIKELY(compare_digit_arrays(digits_, this_num_digits, v.digits_, v_num_digits) == comparison::lt)) {
            throw not_a_number();
        }

        num_digits_ = subtract_digit_arrays(digits_, digits_, this_num_digits, v.digits_, v_num_digits);

        return *this;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<(unsigned int count) const -> natural {
        std::size_t this_num_digits = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;

        /*
         * If our value is zero then return 0.
         */
        if (C8_UNLIKELY(!this_num_digits)) {
            return res;
        }

        res.reserve(this_num_digits + trailing_digits + 1);
        res.num_digits_ = left_shift_digit_array(res.digits_, digits_, this_num_digits, trailing_digits, digit_shift);

        return res;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<=(unsigned int count) -> natural & {
        std::size_t this_num_digits = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        /*
         * If our value is zero then return 0.
         */
        if (C8_UNLIKELY(!this_num_digits)) {
            return *this;
        }

        expand(this_num_digits + trailing_digits + 1);

        num_digits_ = left_shift_digit_array(digits_, digits_, this_num_digits, trailing_digits, digit_shift);

        return *this;
    }

    /*
     * Right shift this natural number by a bit count.
     */
    auto natural::operator >>(unsigned int count) const -> natural {
        std::size_t this_num_digits = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;

        /*
         * If our shift is more than the total number of bits that we had then return 0.
         */
        if (C8_UNLIKELY(this_num_digits <= trailing_digits)) {
            return res;
        }

        res.reserve(this_num_digits - trailing_digits);
        res.num_digits_ = right_shift_digit_array(res.digits_, digits_, this_num_digits, trailing_digits, digit_shift);

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
         * If our shift is more than the total number of bits that we had then return 0.
         */
        if (C8_UNLIKELY(this_num_digits <= trailing_digits)) {
            num_digits_ = 0;
            return *this;
        }

        num_digits_ = right_shift_digit_array(digits_, digits_, this_num_digits, trailing_digits, digit_shift);

        return *this;
    }

    /*
     * Multiply this natural number with a single digit.
     */
    auto natural::operator *(natural_digit v) const -> natural {
        natural res;

        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (C8_UNLIKELY(!this_num_digits)) {
            return res;
        }

        if (C8_UNLIKELY(!v)) {
            return res;
        }

        res.reserve(this_num_digits + 1);
        res.num_digits_ = multiply_digit_array_digit(res.digits_, digits_, this_num_digits, v);

        return res;
    }

    /*
     * Multiply this natural number with another one.
     */
    auto natural::operator *(const natural &v) const -> natural {
        natural res;

        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (C8_UNLIKELY(!this_num_digits)) {
            return res;
        }

        std::size_t v_num_digits = v.num_digits_;
        if (C8_UNLIKELY(!v_num_digits)) {
            return res;
        }

        /*
         * Are we multiplying by a single digit?  If yes, then use the fast version.
         */
        if (C8_UNLIKELY(v_num_digits == 1)) {
            res.reserve(this_num_digits + 1);
            res.num_digits_ = multiply_digit_array_digit(res.digits_, digits_, this_num_digits, v.digits_[0]);
            return res;
        }

        /*
         * We need to multiply two digit arrays.
         */
        std::size_t res_num_digits = this_num_digits + v_num_digits - 1;
        res.reserve(res_num_digits + 1);
        res.num_digits_ = multiply_digit_arrays(res.digits_, digits_, this_num_digits, v.digits_, v_num_digits);

        return res;
    }

    /*
     * Multiply this natural number with a single digit.
     */
    auto natural::operator *=(natural_digit v) -> natural & {
        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_num_digits = num_digits_;
        if (C8_UNLIKELY(!this_num_digits)) {
            return *this;
        }

        if (C8_UNLIKELY(!v)) {
            num_digits_ = 0;
            return *this;
        }

        expand(this_num_digits + 1);
        num_digits_ = multiply_digit_array_digit(digits_, digits_, this_num_digits, v);

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

        std::pair<natural, natural_digit> p;
        std::size_t this_num_digits = num_digits_;
        if (C8_UNLIKELY(!this_num_digits)) {
            p.second = 0;
            return p;
        }

        p.first.reserve(this_num_digits);
        p.first.num_digits_ = divide_modulus_digit_array_digit(p.first.digits_, p.second, digits_, this_num_digits, v);

        return p;
    }

    /*
     * Divide this natural number by another one, returning the quotient and modulus.
     */
    auto natural::divide_modulus(const natural &v) const -> std::pair<natural, natural> {
        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (C8_UNLIKELY(!v.num_digits_)) {
            throw divide_by_zero();
        }

        std::pair<natural, natural> p;

        /*
         * Is the result zero?  If yes then we're done.
         */
        if (C8_UNLIKELY(compare_digit_arrays(digits_, num_digits_, v.digits_, v.num_digits_) == comparison::lt)) {
            p.second = *this;
            return p;
        }

        /*
         * Are we dividing by a single digit?  If yes, then use the fast version
         * of divide_modulus!
         */
        if (C8_UNLIKELY(v.num_digits_ == 1)) {
            natural_digit mod;
            p.first.reserve(num_digits_);
            p.first.num_digits_ = divide_modulus_digit_array_digit(p.first.digits_, mod, digits_, num_digits_, v.digits_[0]);
            p.second = natural(mod);
            return p;
        }

        /*
         * Normalize the divisor and quotient.  We want our quotient to be aligned such
         * that its top bit is set within a natural_digit.  This may seem a little odd, but
         * we want to ensure that any quotient estimation is as accurate as possible.
         */
        auto divisor_bits = v.count_bits();
        auto divisor_digit_bits = divisor_bits & (natural_digit_bits - 1);

        unsigned int normalize_shift = static_cast<unsigned int>((natural_digit_bits - divisor_digit_bits) & (natural_digit_bits - 1));

        natural remaining;
        remaining.reserve(num_digits_ + 1);
        natural_digit *remaining_digits = remaining.digits_;
        auto remaining_num_digits = left_shift_digit_array(remaining_digits, digits_, num_digits_, 0, normalize_shift);

        natural t1;
        t1.reserve(remaining_num_digits);
        natural_digit *t1_digits = t1.digits_;

        natural t2;
        t2.reserve(remaining_num_digits);
        natural_digit *t2_digits = t2.digits_;

        natural divisor;
        divisor.reserve(v.num_digits_ + 1);
        natural_digit *divisor_digits = divisor.digits_;
        auto divisor_num_digits = left_shift_digit_array(divisor_digits, v.digits_, v.num_digits_, 0, normalize_shift);

        std::size_t res_num_digits = remaining_num_digits - divisor_num_digits + 1;
        p.first.reserve(res_num_digits);
        natural_digit *res_digits = p.first.digits_;
        zero_digit_array(res_digits, res_num_digits);

        /*
         * Now we run a long divide algorithm.
         */
        std::size_t i = remaining_num_digits;
        auto upper_div_digit = divisor_digits[divisor_num_digits - 1];
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
                auto t1_num_digits = left_shift_digit_array(t1_digits, divisor_digits, divisor_num_digits, (i - divisor_num_digits + 1), 0);
                if (C8_UNLIKELY(compare_digit_arrays(t1_digits, t1_num_digits, remaining_digits, remaining_num_digits) != comparison::gt)) {
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
                    auto t2_num_digits = left_shift_digit_array(t2_digits, divisor_digits, divisor_num_digits, (i - divisor_num_digits), 0);
                    t1_num_digits = subtract_digit_arrays(t1_digits, t1_digits, t1_num_digits, t2_digits, t2_num_digits);
                }

                res_digits[i - divisor_num_digits] = q;
                remaining_num_digits = subtract_digit_arrays(remaining_digits, remaining_digits, remaining_num_digits, t1_digits, t1_num_digits);
                if (C8_UNLIKELY(compare_digit_arrays(remaining_digits, remaining_num_digits, divisor_digits, divisor_num_digits) == comparison::lt)) {
                    break;
                }

                if (C8_UNLIKELY(i > remaining_num_digits)) {
                    i = remaining_num_digits;
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

            auto t1_num_digits = multiply_digit_array_digit(t1_digits, divisor_digits, divisor_num_digits, q);
            t1_num_digits = left_shift_digit_array(t1_digits, t1_digits, t1_num_digits, (i - divisor_num_digits), 0);

            /*
             * It's possible that our estimate might be slightly too large, so we have
             * to evaluate it on the basis of the full divisor, not just the shifted, most
             * significant digit.  This may mean we reduce our estimate slightly.
             */
            if (C8_UNLIKELY(compare_digit_arrays(t1_digits, t1_num_digits, remaining_digits, remaining_num_digits) == comparison::gt)) {
                q--;
                auto t2_num_digits = left_shift_digit_array(t2_digits, divisor_digits, divisor_num_digits, (i - divisor_num_digits), 0);
                t1_num_digits = subtract_digit_arrays(t1_digits, t1_digits, t1_num_digits, t2_digits, t2_num_digits);
            }

            res_digits[i - divisor_num_digits] = q;
            remaining_num_digits = subtract_digit_arrays(remaining_digits, remaining_digits, remaining_num_digits, t1_digits, t1_num_digits);
            if (C8_UNLIKELY(compare_digit_arrays(remaining_digits, remaining_num_digits, divisor_digits, divisor_num_digits) == comparison::lt)) {
                break;
            }

            if (C8_UNLIKELY(i > remaining_num_digits)) {
                i = remaining_num_digits;
            }
        }

        /*
         * Calculate our resulting digits.
         */
        if (!res_digits[res_num_digits - 1]) {
            res_num_digits--;
        }

        p.first.num_digits_ = res_num_digits;
        if (remaining_num_digits) {
            p.second.num_digits_ = right_shift_digit_array(p.second.digits_, remaining_digits, remaining_num_digits, 0, normalize_shift);
        }

        return p;
    }

    /*
     * Find the greatest common divisor of this and another natural number.
     */
    auto natural::gcd(const natural &v) const -> natural {
        natural smaller;

        std::size_t v_num_digits = v.num_digits_;
        if (C8_UNLIKELY(!v.num_digits_)) {
            smaller = *this;
            return smaller;
        }

        std::size_t this_num_digits = num_digits_;
        if (C8_UNLIKELY(!num_digits_)) {
            smaller = v;
            return smaller;
        }

        natural larger;
        if (C8_UNLIKELY(compare_digit_arrays(digits_, this_num_digits, v.digits_, v_num_digits) == comparison::lt)) {
            smaller = *this;
            larger = v;
        } else {
            smaller = v;
            larger = *this;
        }

        while (true) {
            natural mod = larger % smaller;
            if (C8_UNLIKELY(!mod.num_digits_)) {
                break;
            }

            larger = std::move(smaller);
            smaller = std::move(mod);
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
        std::size_t v_num_digits = v.num_digits_;
        if (v_num_digits == 0) {
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

