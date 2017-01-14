/*
 * natural.cpp
 */
#include <cctype>

#include "natural.h"

namespace c8 {
    const std::size_t natural_digit_bits = 8 * sizeof(natural_digit);

    const char digits_lower[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    const char digits_upper[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    natural::natural(unsigned long long v) {
        if (!v) {
            return;
        }

        while (v) {
            natural_digit m = static_cast<natural_digit>(-1);
            digits_.push_back(static_cast<natural_digit>(v & m));
            v >>= natural_digit_bits;
        }
    }

    natural::natural(const std::string &v) {
        std::size_t v_sz = v.size();
        if (v_sz == 0) {
            throw std::invalid_argument("zero size string");
        }

        natural res;

        /*
         * Is our first character a '0'?  If it is then we may have an octal or hex number.
         */
        std::size_t idx = 0;
        unsigned long base = 10;
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

        natural bbase{base};

        /*
         * We now know the base we're using and the starting index.
         */
        for (std::size_t i = idx; i < v_sz; i++) {
            auto c = v[i];
            if (!isdigit(c)) {
                if (base != 16) {
                    throw std::invalid_argument("invalid digit");
                }

                if (!isxdigit(c)) {
                    throw std::invalid_argument("invalid digit");
                }

                c = static_cast<char>(tolower(c));
                c -= ('a' - '0' - 10);
            }

            if (base == 8) {
                if (c >= '8') {
                    throw std::invalid_argument("invalid digit");
                }
            }

            res *= bbase;
            res += natural(static_cast<unsigned long long>(c - '0'));
        }

        *this = res;
    }

    /*
     * Return the number of bits required to represent this natural number.
     */
    auto natural::count_bits() -> unsigned int {
        /*
         * If we have no digits then this is a simple (special) case.
         */
        std::size_t this_sz = digits_.size();
        if (this_sz == 0) {
            return 0;
        }

        /*
         * We can account for trailing digits easily, but the most significant digit is
         * more tricky.  We use __builtin_clz() to count the leadign zeros of the digit,
         * but if the size of a digit is smaller than the size of an integer (which is
         * what __builtin_clz() uses) then we must compensate for the extra zeros that
         * it returns.
         */
        natural_digit d = digits_[this_sz - 1];
        auto c = (sizeof(int) / sizeof(natural_digit)) - 1;
        return static_cast<unsigned int>((this_sz + c) * 8 * sizeof(natural_digit)) - static_cast<unsigned int>(__builtin_clz(d));
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::add(const natural &v) const -> natural {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_sz = digits_.size();
        std::size_t v_sz = v.digits_.size();
        const natural *larger;
        const natural *smaller;
        std::size_t larger_sz;
        std::size_t smaller_sz;
        if (this_sz >= v_sz) {
            larger = this;
            smaller = &v;
            larger_sz = this_sz;
            smaller_sz = v_sz;
        } else {
            larger = &v;
            smaller = this;
            larger_sz = v_sz;
            smaller_sz = this_sz;
        }

        natural res;
        res.digits_.reserve(larger_sz);

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_sz; i++) {
            auto a = smaller->digits_[i];
            auto b = larger->digits_[i];
            acc = acc + (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(b));
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_sz; i < larger_sz; i++) {
            auto b = larger->digits_[i];
            acc = acc + static_cast<natural_double_digit>(b);
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
        }

        return res;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::subtract(const natural &v) const -> natural {
        std::size_t this_sz = digits_.size();
        std::size_t v_sz = v.digits_.size();

        natural res;
        res.digits_.reserve(this_sz);

        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < v_sz; i++) {
            auto a = digits_[i];
            auto b = v.digits_[i];
            acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(b)) - acc;
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = v_sz; i < this_sz; i++) {
            auto a = digits_[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (acc) {
            throw std::underflow_error("negative result");
        }

        /*
         * We need to normalize because our result can have zero upper digits.
         */
        res.normalize();

        return res;
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::shiftl(unsigned int count) const -> natural {
        std::size_t this_sz = digits_.size();
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;
        res.digits_.reserve(this_sz + trailing_digits + 1);

        /*
         * Insert as many whole digits as we need to pad the result.
         */
        for (std::size_t i = 0; i < trailing_digits; i++) {
            res.digits_.emplace_back(0);
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < this_sz; i++) {
            acc |= (static_cast<natural_double_digit>(digits_[i]) << digit_shift);
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
        }

        return res;
    }

    /*
     * Right shift this natural number by a bit count.
     */
    auto natural::shiftr(unsigned int count) const -> natural {
        std::size_t this_sz = digits_.size();
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;

        /*
         * If our shift is more than the total number of bits that we had
         * then return 0.
         */
        if (this_sz <= trailing_digits) {
            return res;
        }

        res.digits_.reserve(this_sz - trailing_digits);

        /*
         * Shift the original value and insert in the result.
         */
        natural_double_digit acc = static_cast<natural_double_digit>(digits_[trailing_digits]) >> digit_shift;
        for (std::size_t i = trailing_digits + 1; i < this_sz; i++) {
            acc |= (static_cast<natural_double_digit>(digits_[i]) << (natural_digit_bits - digit_shift));
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res.digits_.emplace_back(static_cast<natural_digit>(acc));
        }

        return res;
    }

    /*
     * Multiply this natural number with another one.
     */
    auto natural::multiply(const natural &v) const -> natural {
        /*
         * If either value is zero then our result is zero.
         */
        std::size_t this_sz = digits_.size();
        if (!this_sz) {
            natural res;
            return res;
        }

        std::size_t v_sz = v.digits_.size();
        if (!v_sz) {
            natural res;
            return res;
        }

        /*
         * Estimate our output size in digits.
         */
        std::size_t res_sz = this_sz + v_sz;

        natural res;
        res.digits_.reserve(res_sz);

        /*
         * Comba multiply.
         */
        natural_digit c0 = 0;
        natural_digit c1 = 0;
        natural_digit c2 = 0;

        for (std::size_t i = 0; i < res_sz; i++) {
            std::size_t tj = (i < (v_sz - 1)) ? i : (v_sz - 1);
            std::size_t ti = i - tj;
            std::size_t j_lim = ((this_sz - ti) < (tj + 1)) ? (this_sz - ti) : (tj + 1);
            c0 = c1;
            c1 = c2;
            c2 = 0;
            for (std::size_t j = 0; j < j_lim; j++) {
                natural_digit a = digits_[ti++];
                natural_digit b = v.digits_[tj--];
                natural_double_digit d0 = static_cast<natural_double_digit>(c0) + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(b));
                c0 = static_cast<natural_digit>(d0);
                natural_double_digit d1 = static_cast<natural_double_digit>(c1) + (d0 >> natural_digit_bits);
                c1 = static_cast<natural_digit>(d1);
                c2 += static_cast<natural_digit>((d1 >> natural_digit_bits));
            }

            res.digits_.emplace_back(c0);
        }

        /*
         * We need to normalize because our result can have zero upper digits.
         */
        res.normalize();

        return res;
    }

    /*
     * Divide this natural number by another one, returning the quotient and modulus.
     */
    auto natural::divide_modulus(const natural &v) const -> std::pair<natural, natural> {
        natural res;

        /*
         * Is the result zero?  If yes then we're done.
         */
        if (*this < v) {
            return std::make_pair(res, *this);
        }

        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (!v.digits_.size()) {
            throw std::logic_error("divide_by_zero");
        }

        /*
         * Normalize the divisor and quotient.  We want our quotient to be aligned such
         * that its top bit is set within a natural_digit.  This may seem a little odd, but
         * we want to ensure that any quotient estimation is as accurate as possible.
         */
        natural remaining = *this;
        natural divisor = v;
        auto divisor_bits = divisor.count_bits();
        auto divisor_digit_bits = divisor_bits & (natural_digit_bits - 1);
        unsigned int normalize_shift = 0;
        if (divisor_digit_bits != (natural_digit_bits - 1)) {
            normalize_shift = static_cast<unsigned int>(natural_digit_bits - divisor_digit_bits);
            remaining <<= normalize_shift;
            divisor <<= normalize_shift;
        }

        /*
         * We know that our result is at least 1.  Start by estimating the first digit
         * of the result.
         */
        std::size_t remaining_sz = remaining.digits_.size();
        std::size_t divisor_sz = divisor.digits_.size();
        natural rdigits;
        rdigits.digits_.reserve(remaining_sz);

        /*
         * Is the first digit of our dividend larger than that of the divisor?  If it is then
         * we insert a one and move to the next step.
         */
        if (remaining.digits_[remaining_sz - 1] >= divisor.digits_[divisor_sz - 1]) {
            natural m = (divisor << static_cast<unsigned int>((remaining_sz - divisor_sz) * natural_digit_bits));
            if (m <= remaining) {
                remaining -= m;
                res = natural(1);
            }
        }

        /*
         * Now we run a long divide algorithm.
         */
        for (std::size_t i = remaining_sz - 1; i > (divisor_sz - 1); i--) {
            res <<= natural_digit_bits;
            if (remaining < divisor) {
                continue;
            }

            /*
             * Estimate the next digit of the result.
             */
            natural_double_digit d_hi = static_cast<natural_double_digit>(remaining.digits_[i]);
            natural_double_digit d_lo = static_cast<natural_double_digit>(remaining.digits_[i - 1]);
            natural_double_digit d = static_cast<natural_double_digit>((d_hi << natural_digit_bits) + d_lo);
            natural_double_digit q = d / static_cast<natural_double_digit>(divisor.digits_[divisor_sz - 1]);
            natural q1{q};
            natural m = (q1 << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits)) * divisor;

            /*
             * It's possible that our estimate might be slightly too large, so we have
             * to evaluate it on the basis of the full divisor, not just the shifted, most
             * significant digit.  This may mean we reduce our estimate slightly.
             */
            while (m > remaining) {
                m -= (divisor << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits));
                q--;
            }

            remaining -= m;
            res += natural(q); 
        }

        remaining >>= normalize_shift;

        return std::make_pair(res, remaining);
    }

    /*
     * Divide this natural number by another one.
     */
    auto natural::divide(const natural &v) const -> natural {
        std::pair<natural, natural> dm = divide_modulus(v);
        return dm.first;
    }

    /*
     * Modulus (divide and return the remainder) this natural number by another one.
     */
    auto natural::modulus(const natural &v) const -> natural {
        std::pair<natural, natural> dm = divide_modulus(v);
        return dm.second;
    }

    /*
     * Compare this natural number with another one.
     */
    auto natural::compare(const natural &v) const -> natural_comparison {
        std::size_t this_sz = digits_.size();
        std::size_t v_sz = v.digits_.size();

        /*
         * If our sizes differ then this is really easy!
         */
        if (this_sz > v_sz) {
            return natural_comparison::gt;
        }

        if (this_sz < v_sz) {
            return natural_comparison::lt;
        }

        /*
         * Our sizes are the same so do digit-by-digit comparisons.
         */
        for (std::size_t i = this_sz; i != 0; --i) {
            auto a = digits_[i - 1];
            auto b = v.digits_[i - 1];
            if (a > b) {
                return natural_comparison::gt;
            }

            if (a < b) {
                return natural_comparison::lt;
            }
        }

        return natural_comparison::eq;
    }

    /*
     * Normalize a natural number (strip any leading zeros)
     */
    auto natural::normalize() -> void {
        /*
         * If our upper digit is non-zero we're done, but if not then remove the upper digit.
         */
        std::size_t sz = digits_.size();
        while (sz--) {
            if (digits_[sz]) {
                break;
            }

            digits_.pop_back();
        }
    }

    /*
     * << operator to print a natural.
     */
    auto operator<<(std::ostream &outstr, const natural &v) -> std::ostream & {
        std::size_t v_sz = v.digits_.size();
        if (v_sz == 0) {
            outstr << '0';
            return outstr;
        }

        unsigned long base = 10;
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
        natural b10{base};
        auto rem = v;
        while (rem != natural(0)) {
            std::pair<natural, natural> qm = rem.divide_modulus(b10);
            natural mod = qm.second;
            if (mod.digits_.size() == 0) {
                res.emplace_back('0');
            } else {
                res.emplace_back(digits[mod.digits_[0]]);
            }

            rem = qm.first;
        }

        std::size_t res_sz = res.size();
        for (std::size_t i = 0; i < res_sz; i++) {
            outstr << res[res_sz - 1 - i];
        }

        return outstr;
    }
}
