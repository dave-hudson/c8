/*
 * natural.cpp
 */
#include <cctype>
#include <cstring>

#include "natural.h"

namespace c8 {
    const std::size_t natural_digit_bits = 8 * sizeof(natural_digit);

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
        digits_ = nullptr;
        digits_size_ = 0;

        if (!v) {
            return;
        }

        reserve(sizeof(unsigned long long) / sizeof(natural_digit));

        num_digits_ = 0;
        while (v) {
            natural_digit m = static_cast<natural_digit>(-1);
            digits_[num_digits_] = static_cast<natural_digit>(v & m);
            num_digits_++;
            v >>= natural_digit_bits;
        }
    }

    /*
     * Construct a natural number from a string.
     *
     * The string representation can have the usual C++-like hex, octal, or decimal
     * representations.
     */
    natural::natural(const std::string &v) {
        digits_ = nullptr;
        digits_size_ = 0;
        num_digits_ = 0;

        std::size_t v_sz = v.size();
        if (v_sz == 0) {
            throw invalid_argument("zero size string");
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
                    throw invalid_argument("invalid digit");
                }

                if (!isxdigit(c)) {
                    throw invalid_argument("invalid digit");
                }

                c = static_cast<char>(tolower(c));
                c -= ('a' - '0' - 10);
            }

            if (base == 8) {
                if (c >= '8') {
                    throw invalid_argument("invalid digit");
                }
            }

            res *= bbase;
            res += natural(static_cast<unsigned long long>(c - '0'));
        }

        *this = std::move(res);
    }

    /*
     * Copy constructor.
     */
    natural::natural(const natural &v) {
        num_digits_ = v.num_digits_;
        digits_size_ = v.digits_size_;
        if (!digits_size_) {
            digits_ = nullptr;
            return;
        }

        digits_ = new natural_digit[digits_size_];
        std::memcpy(digits_, v.digits_, sizeof(natural_digit) * digits_size_);
    }

    /*
     * Move constructor.
     */
    natural::natural(natural &&v) {
        num_digits_ = v.num_digits_;
        digits_size_ = v.digits_size_;
        digits_ = v.digits_;
        v.digits_ = nullptr;
        v.num_digits_ = 0;
        v.digits_size_ = 0;
    }

    /*
     * Destructor.
     */
    natural::~natural() {
        if (digits_size_) {
            delete[] digits_;
        }
    }

    /*
     * Copy assignment operator.
     */
    auto natural::operator =(const natural &v) -> natural & {
        /*
         * Are we assigning to ourself?  If we are then we don't need to do anything.
         */
        if (this == &v) {
            return *this;
        }

        /*
         * Delete the old contents of this natural number.
         */
        if (digits_size_) {
            delete[] digits_;
        }

        /*
         * Copy the contents of v.
         */
        num_digits_ = v.num_digits_;
        digits_size_ = v.digits_size_;
        if (!digits_size_) {
            digits_ = nullptr;
            return *this;
        }

        digits_ = new natural_digit[digits_size_];
        std::memcpy(digits_, v.digits_, sizeof(natural_digit) * digits_size_);

        return *this;
    }

    /*
     * Move assignment operator.
     */
    auto natural::operator =(natural &&v) -> natural & {
        /*
         * Are we assigning to ourself?  If we are then we don't have to do anything.
         */
        if (this == &v) {
            return *this;
        }

        /*
         * Delete the old contents of this natural number.
         */
        if (digits_size_) {
            delete[] digits_;
        }

        /*
         * Move the contents of v.
         */
        num_digits_ = v.num_digits_;
        digits_size_ = v.digits_size_;
        digits_ = v.digits_;
        v.digits_ = nullptr;
        v.num_digits_ = 0;
        v.digits_size_ = 0;

        return *this;
    }

    /*
     * Return the number of bits required to represent this natural number.
     */
    auto natural::count_bits() const -> unsigned int {
        /*
         * If we have no digits then this is a simple (special) case.
         */
        std::size_t this_sz = num_digits_;
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
        return static_cast<unsigned int>((this_sz + c) * natural_digit_bits) - static_cast<unsigned int>(__builtin_clz(d));
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
        res.reserve(larger_sz + 1);

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_sz; i++) {
            auto a = smaller->digits_[i];
            auto b = larger->digits_[i];
            acc = acc + (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(b));
            res.digits_[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_sz; i < larger_sz; i++) {
            auto b = larger->digits_[i];
            acc = acc + static_cast<natural_double_digit>(b);
            res.digits_[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        res.num_digits_ = larger_sz;

        if (acc) {
            res.digits_[larger_sz] = static_cast<natural_digit>(acc);
            res.num_digits_++;
        }

        return res;
    }

    /*
     * Add another natural number to this one.
     */
    auto natural::operator +=(const natural &v) -> void {
        /*
         * Work out which of the two numbers is larger and which is smaller.
         */
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;
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

        expand(larger_sz + 1);

        /*
         * Add the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < smaller_sz; i++) {
            auto a = smaller->digits_[i];
            auto b = larger->digits_[i];
            acc = acc + (static_cast<natural_double_digit>(a) + static_cast<natural_double_digit>(b));
            digits_[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        /*
         * Add the remaining digits and any carries.
         */
        for (std::size_t i = smaller_sz; i < larger_sz; i++) {
            auto b = larger->digits_[i];
            acc = acc + static_cast<natural_double_digit>(b);
            digits_[i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        num_digits_ = larger_sz;

        if (acc) {
            digits_[larger_sz] = static_cast<natural_digit>(acc);
            num_digits_++;
        }
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -(const natural &v) const -> natural {
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

        natural res;
        res.reserve(this_sz);
        res.num_digits_ = this_sz;

        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < v_sz; i++) {
            auto a = digits_[i];
            auto b = v.digits_[i];
            acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(b)) - acc;
            res.digits_[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = v_sz; i < this_sz; i++) {
            auto a = digits_[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            res.digits_[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (acc) {
            throw not_a_number();
        }

        /*
         * We need to normalize because our result can have zero upper digits.
         */
        res.normalize();

        return res;
    }

    /*
     * Subtract another natural number from this one.
     */
    auto natural::operator -=(const natural &v) -> void {
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

        /*
         * Subtract the parts together until we run out of digits in the smaller part.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < v_sz; i++) {
            auto a = digits_[i];
            auto b = v.digits_[i];
            acc = (static_cast<natural_double_digit>(a) - static_cast<natural_double_digit>(b)) - acc;
            digits_[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * Subtract the remaining digits and any carries.
         */
        for (std::size_t i = v_sz; i < this_sz; i++) {
            auto a = digits_[i];
            acc = static_cast<natural_double_digit>(a) - acc;
            digits_[i] = static_cast<natural_digit>(acc);
            acc = (acc >> natural_digit_bits) & 1;
        }

        /*
         * We should not have a carry!
         */
        if (acc) {
            throw not_a_number();
        }

        /*
         * We need to normalize because our result can have zero upper digits.
         */
        normalize();
    }

    /*
     * Left shift this natural number by a bit count.
     */
    auto natural::operator <<(unsigned int count) const -> natural {
        std::size_t this_sz = num_digits_;
        std::size_t trailing_digits = count / natural_digit_bits;
        std::size_t digit_shift = count % natural_digit_bits;

        natural res;
        res.reserve(this_sz + trailing_digits + 1);
        res.num_digits_ = trailing_digits + this_sz;

        /*
         * Are we shifting by whole digits?
         */
        if (digit_shift == 0) {
            std::memcpy(&res.digits_[trailing_digits], digits_, this_sz * sizeof(natural_digit));
            return res;
        }

        /*
         * Shift the original value by the remaining number of bits that we
         * need, and insert those in the result.
         */
        natural_double_digit acc = 0;
        for (std::size_t i = 0; i < this_sz; i++) {
            acc |= (static_cast<natural_double_digit>(digits_[i]) << digit_shift);
            res.digits_[trailing_digits + i] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        if (acc) {
            res.digits_[trailing_digits + this_sz] = static_cast<natural_digit>(acc);
            res.num_digits_++;
        }

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
         * If our shift is more than the total number of bits that we had
         * then return 0.
         */
        if (this_sz <= trailing_digits) {
            return res;
        }

        res.reserve(this_sz - trailing_digits);

        /*
         * Shift the original value and insert in the result.
         */
        natural_double_digit acc = static_cast<natural_double_digit>(digits_[trailing_digits]) >> digit_shift;
        for (std::size_t i = trailing_digits + 1; i < this_sz; i++) {
            acc |= (static_cast<natural_double_digit>(digits_[i]) << (natural_digit_bits - digit_shift));
            res.digits_[i - (trailing_digits + 1)] = static_cast<natural_digit>(acc);
            acc >>= natural_digit_bits;
        }

        res.num_digits_ = this_sz - (trailing_digits + 1);

        if (acc) {
            res.digits_[this_sz - (trailing_digits + 1)] = static_cast<natural_digit>(acc);
            res.num_digits_++;
        }

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
        if (!this_sz) {
            natural res;
            return res;
        }

        std::size_t v_sz = v.num_digits_;
        if (!v_sz) {
            natural res;
            return res;
        }

        /*
         * Estimate our output size in digits.
         */
        std::size_t res_sz = this_sz + v_sz;

        natural res;
        res.reserve(res_sz);
        res.num_digits_ = res_sz;

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
                natural_digit a = digits_[ti++];
                natural_digit b = v.digits_[tj--];
                natural_double_digit d0 = static_cast<natural_double_digit>(c0) + (static_cast<natural_double_digit>(a) * static_cast<natural_double_digit>(b));
                c0 = static_cast<natural_digit>(d0);
                natural_double_digit d1 = static_cast<natural_double_digit>(c1) + (d0 >> natural_digit_bits);
                c1 = static_cast<natural_digit>(d1);
                c2 += static_cast<natural_digit>((d1 >> natural_digit_bits));
            }

            res.digits_[i] = c0;
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
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (!v.num_digits_) {
            throw divide_by_zero();
        }

        /*
         * Is the result zero?  If yes then we're done.
         */
        if (*this < v) {
            return std::make_pair(std::move(res), *this);
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
        std::size_t remaining_sz = remaining.num_digits_;
        std::size_t divisor_sz = divisor.num_digits_;

        /*
         * Is the first digit of our dividend larger than that of the divisor?  If it is then
         * we insert a one and move to the next step.
         */
        res.reserve(remaining_sz - divisor_sz + 1);
        res.num_digits_ = remaining_sz - divisor_sz + 1;

        /*
         * Now we run a long divide algorithm.
         */
        std::size_t i = remaining_sz;
        natural_double_digit upper_div_digit = static_cast<natural_double_digit>(divisor.digits_[divisor_sz - 1]);
        while (remaining >= divisor) {
            /*
             * Iterate through our dividend from the most significant digit to the least.
             */
            i--;

            /*
             * During the course of an earlier iteration we may have reduced the size of
             * our remaining dividend by more than one digit.  If so then move on to the next
             * digit.
             */
            if (i >= remaining.num_digits_) {
                continue;
            }

            /*
             * We know that our divisor has been shifted so that the most significant digit has
             * its top bit set.  This means that the quotient for our next digit can only be 0 or 1.
             */
            natural_double_digit d_hi = static_cast<natural_double_digit>(remaining.digits_[i]);
            if (d_hi >= upper_div_digit) {
                natural m = divisor << static_cast<unsigned int>((i - divisor_sz + 1) * natural_digit_bits);

                /*
                 * Was the result 1?
                 */
                if (remaining >= m) {
                    remaining -= m;
                    res.digits_[i - divisor_sz + 1] = static_cast<natural_digit>(1);

                    /*
                     * In reducing the remaining divisor we may have affected our ability to
                     * continue this particular iteration.  If we have, then move on to the
                     * next digit.
                     */
                    if (i >= remaining.num_digits_) {
                        continue;
                    }

                    /*
                     * If we have fewer digits left then the size of the divisor then there's nothing
                     * left to do.
                     */
                    if (i < divisor_sz) {
                        break;
                    }

                    /*
                     * Having reduced our remaining divident we changed its upper most digit
                     * so we need to fetch the new version.
                     */
                    d_hi = static_cast<natural_double_digit>(remaining.digits_[i]);
                }
            }

            /*
             * Estimate the next digit of the result.
             */
            natural_double_digit d_lo = static_cast<natural_double_digit>(remaining.digits_[i - 1]);
            natural_double_digit d = static_cast<natural_double_digit>((d_hi << natural_digit_bits) + d_lo);
            natural_double_digit q = d / upper_div_digit;
            natural m = (natural(q) * divisor) << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits);

            /*
             * It's possible that our estimate might be slightly too large, so we have
             * to evaluate it on the basis of the full divisor, not just the shifted, most
             * significant digit.  This may mean we reduce our estimate slightly.
             */
            if (m > remaining) {
                m -= (divisor << static_cast<unsigned int>((i - divisor_sz) * natural_digit_bits));
                q--;
            }

            remaining -= m;

            res.digits_[i - divisor_sz] = static_cast<natural_digit>(q);
        }

        remaining >>= normalize_shift;

        /*
         * We need to normalize because our result can have zero upper digits.
         */
        res.normalize();

        return std::make_pair(std::move(res), std::move(remaining));
    }

    /*
     * Compare this natural number with another one.
     */
    auto natural::compare(const natural &v) const -> natural_comparison {
        std::size_t this_sz = num_digits_;
        std::size_t v_sz = v.num_digits_;

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
     * Find the greatest common divisor of this and another natural number.
     */
    auto natural::gcd(const natural &v) const -> natural {
        if (v.is_zero()) {
            return *this;
        }

        if (is_zero()) {
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

        natural mod;
        while (!smaller.is_zero()) {
            mod = larger % smaller;
            larger = smaller;
            smaller = mod;
        }

        return larger;
    }

    /*
     * Normalize a natural number (strip any leading zeros)
     */
    auto natural::normalize() -> void {
        /*
         * If our upper digit is non-zero we're done, but if not then remove the upper digit.
         */
        std::size_t sz = num_digits_;
        while (sz--) {
            if (digits_[sz]) {
                break;
            }

            num_digits_--;
        }
    }

    /*
     * Reserve a number of digits in this natural number.
     */
    auto natural::reserve(std::size_t new_digits) -> void {
        /*
         * Always make space for one extra digit.  This means that sometimes
         * we won't have to expand this number if we do an in-place edit.
         */
        digits_ = new natural_digit[new_digits + 1]();
        digits_size_ = new_digits + 1;
        num_digits_ = 0;
    }

    /*
     * Expand the number of digits in this natural number.
     */
    auto natural::expand(std::size_t new_digits) -> void {
        if (digits_size_ >= new_digits) {
            return;
        }

        auto n = new natural_digit[new_digits];
        std::memset(&n[num_digits_], 0, sizeof(natural_digit) * (new_digits - num_digits_));

        if (digits_) {
            std::memcpy(n, digits_, sizeof(natural_digit) * num_digits_);
            delete[] digits_;
        }

        digits_size_ = new_digits;
        num_digits_ = 0;
        digits_ = n;
    }

    /*
     * Convert this natural number to an unsigned long long.
     */
    auto natural::to_unsigned_long_long() const -> unsigned long long {
        /*
         * Will this number fit in an unsigned long long?  If not then throw an
         * exception.
         */
        if (this->count_bits() > (8 * sizeof(long long))) {
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
        for (unsigned int i = 0; i < sz; i++) {
            res |= (static_cast<unsigned long long>(digits_[i]) << (i * natural_digit_bits));
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
            if (mod.num_digits_ == 0) {
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

