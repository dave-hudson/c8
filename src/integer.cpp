/*
 * integer.cpp
 */
#include "c8.h"

namespace c8 {
    /*
     * Construct an integer using a string.
     *
     * The string can have an optional '-' sign to indicate that it's negative,
     * and then the usual C++-like hex, octal, * or decimal representations.
     */
    integer::integer(const std::string &v) {
        negative_ = false;

        /*
         * Does our integer have a '-' sign?
         */
        if (v.length()) {
            if (v[0] == '-') {
                negative_ = true;
            }
        }

        magnitude_ = natural(negative_ ? v.substr(1) : v);
    }

    /*
     * Return true if this integer is equal to another one, false if it's not.
     */
    auto integer::operator ==(const integer &v) const noexcept -> bool {
        /*
         * If our signs are different then we can't be equal.
         */
        if (negative_ != v.negative_) {
            return false;
        }

        /*
         * Signs are the same, so compare magnitudes.
         */
        return magnitude_ == v.magnitude_;
    }

    /*
     * Return true if this integer is not equal to another one, false if it's equal.
     */
    auto integer::operator !=(const integer &v) const noexcept -> bool {
        /*
         * If our signs are different then we must be not-equal.
         */
        if (negative_ != v.negative_) {
            return true;
        }

        /*
         * Signs are the same, so compare magnitudes.
         */
        return magnitude_ != v.magnitude_;
    }

    /*
     * Return true if this integer is greater than another one, false if it's not.
     */
    auto integer::operator >(const integer &v) const noexcept -> bool {
        /*
         * Is this number non-negative?
         */
        if (!negative_) {
            /*
             * If the number we're comparing with is non-negative too then we can
             * just do a simple comparison of the magnitudes.
             */
            if (!v.negative_) {
                return magnitude_ > v.magnitude_;
            }

            /*
             * The number we're comparing with is negative so we're definitely greater
             * than it.
             */
            return true;
        }

        /*
         * This integer is negative, so if the number we're comparing with is also
         * negative then we compare magnitudes, but we reverse the result.  The most
         * negative value is smaller, not larger.
         */
        if (v.negative_) {
            return magnitude_ < v.magnitude_;
        }

        /*
         * This integer is negative, but the one we're comparing with is not, so we're
         * smaller.
         */
        return false;
    }

    /*
     * Return true if this integer is greater than, or equal to, another one, false if it's not.
     */
    auto integer::operator >=(const integer &v) const noexcept -> bool {
        /*
         * Is this number non-negative?
         */
        if (!negative_) {
            /*
             * If the number we're comparing with is non-negative too then we can
             * just do a simple comparison of the magnitudes.
             */
            if (!v.negative_) {
                return magnitude_ >= v.magnitude_;
            }

            /*
             * The number we're comparing with is negative so we're definitely greater
             * than it.
             */
            return true;
        }

        /*
         * This integer is negative, so if the number we're comparing with is also
         * negative then we compare magnitudes, but we reverse the result.  The most
         * negative value is smaller, not larger.
         */
        if (v.negative_) {
            return magnitude_ <= v.magnitude_;
        }

        /*
         * This integer is negative, but the one we're comparing with is not, so we're
         * smaller.
         */
        return false;
    }

    /*
     * Return true if this integer is less than another one, false if it's not.
     */
    auto integer::operator <(const integer &v) const noexcept -> bool {
        /*
         * Is this number non-negative?
         */
        if (!negative_) {
            /*
             * If the number we're comparing with is non-negative too then we can
             * just do a simple comparison of the magnitudes.
             */
            if (!v.negative_) {
                return magnitude_ < v.magnitude_;
            }

            /*
             * The number we're comparing with is negative so we're definitely not
             * less than it.
             */
            return false;
        }

        /*
         * This integer is negative, so if the number we're comparing with is also
         * negative then we compare magnitudes, but we reverse the result.  The most
         * negative value is smaller, not larger.
         */
        if (v.negative_) {
            return magnitude_ > v.magnitude_;
        }

        /*
         * This integer is negative, but the one we're comparing with is not, so we're
         * smaller.
         */
        return true;
    }

    /*
     * Return true if this integer is less than, or equal to, another one, false if it's not.
     */
    auto integer::operator <=(const integer &v) const noexcept -> bool {
        /*
         * Is this number non-negative?
         */
        if (!negative_) {
            /*
             * If the number we're comparing with is non-negative too then we can
             * just do a simple comparison of the magnitudes.
             */
            if (!v.negative_) {
                return magnitude_ <= v.magnitude_;
            }

            /*
             * The number we're comparing with is negative so we're definitely not
             * less than it.
             */
            return false;
        }

        /*
         * This integer is negative, so if the number we're comparing with is also
         * negative then we compare magnitudes, but we reverse the result.  The most
         * negative value is smaller, not larger.
         */
        if (v.negative_) {
            return magnitude_ >= v.magnitude_;
        }

        /*
         * This integer is negative, but the one we're comparing with is not, so we're
         * smaller.
         */
        return true;
    }

    /*
     * Add another integer to this one.
     */
    auto integer::operator +(const integer &v) const -> integer {
        integer res;

        /*
         * If our two numbers have the same sign then we just add and retain the
         * sign for this number.
         */
        if (negative_ == v.negative_) {
            res.negative_ = negative_;
            res.magnitude_ = magnitude_ + v.magnitude_;
            return res;
        }

        /*
         * If we are adding a larger magnitude value then subtract the smaller
         * from the larger, and retain the sign of the larger.
         */
        if (magnitude_ < v.magnitude_) {
            res.negative_ = v.negative_;
            res.magnitude_ = v.magnitude_ - magnitude_;
            return res;
        }

        /*
         * We're adding a smaller magnitude value, so subtract the smaller
         * from the larger, and, again, retain the sign of the larger.
         */
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ - v.magnitude_;
        return res;
    }

    /*
     * Add another integer to this one.
     */
    auto integer::operator +=(const integer &v) -> integer & {
        /*
         * If our two numbers have the same sign then we just add and retain the
         * sign for this number.
         */
        if (negative_ == v.negative_) {
            magnitude_ += v.magnitude_;
            return *this;
        }

        /*
         * If we are adding a larger magnitude value then subtract the smaller
         * from the larger, and retain the sign of the larger.
         */
        if (magnitude_ < v.magnitude_) {
            negative_ = v.negative_;
            magnitude_ = v.magnitude_ - magnitude_;
            return *this;
        }

        /*
         * We're adding a smaller magnitude value, so subtract the smaller
         * from the larger, and, again, retain the sign of the larger.
         */
        magnitude_ -= v.magnitude_;
        return *this;
    }

    /*
     * Subtract another integer from this one.
     *
     * Note that there is a subtle difference between the integer subtract operation
     * and the natural number version:  We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto integer::operator -(const integer &v) const -> integer {
        integer res;

        /*
         * If we're subtracting a negative number from a positive, or a positive from
         * a negative, then we add the magnitudes of both and retain the sign of this one.
         */
        if (negative_ != v.negative_) {
            res.negative_ = negative_;
            res.magnitude_ = magnitude_ + v.magnitude_;
            return res;
        }

        /*
         * If we're subtracting a number that has a larger magnitude than this one, then
         * subtract the magnitudes and use the inverse of the sign for the larger.
         */
        if (magnitude_ < v.magnitude_) {
            res.negative_ = !v.negative_;
            res.magnitude_ = v.magnitude_ - magnitude_;
            return res;
        }

        /*
         * We're subtracting a number with a smaller magnitude than this one, so subtract
         * the magnitudes and use the sign of this one.
         */
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ - v.magnitude_;
        return res;
    }

    /*
     * Subtract another integer from this one.
     *
     * Note that there is a subtle difference between the integer subtract operation
     * and the natural number version:  We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto integer::operator -=(const integer &v) -> integer & {
        /*
         * If we're subtracting a negative number from a positive, or a positive from
         * a negative, then we add the magnitudes of both and retain the sign of this one.
         */
        if (negative_ != v.negative_) {
            magnitude_ += v.magnitude_;
            return *this;
        }

        /*
         * If we're subtracting a number that has a larger magnitude than this one, then
         * subtract the magnitudes and use the inverse of the sign for the larger.
         */
        if (magnitude_ < v.magnitude_) {
            negative_ = !v.negative_;
            magnitude_ = v.magnitude_ - magnitude_;
            return *this;
        }

        /*
         * We're subtracting a number with a smaller magnitude than this one, so subtract
         * the magnitudes and use the sign of this one.
         */
        magnitude_ -= v.magnitude_;
        return *this;
    }

    /*
     * Left shift this integer by a number of bits.
     */
    auto integer::operator <<(unsigned int count) const -> integer {
        integer res;
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ << count;
        return res;
    }

    /*
     * Right shift this integer by a number of bits.
     */
    auto integer::operator >>(unsigned int count) const -> integer {
        integer res;
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ >> count;
        return res;
    }

    /*
     * Multiply another integer with this one.
     */
    auto integer::operator *(const integer &v) const -> integer {
        integer res;
        res.negative_ = negative_ ^ v.negative_;
        res.magnitude_ = magnitude_ * v.magnitude_;
        return res;
    }

    /*
     * Divide this integer by another one, returning the quotient and remainder.
     */
    auto integer::divide_modulus(const integer &v) const -> std::pair<integer, integer> {
        std::pair<natural, natural> dm = magnitude_.divide_modulus(v.magnitude_);

        std::pair<integer, integer> res;
        res.first.negative_ = negative_ ^ v.negative_;
        res.first.magnitude_ = std::move(dm.first);
        res.second.negative_ = negative_;
        res.second.magnitude_ = std::move(dm.second);

        return res;
    }

    /*
     * Divide this integer by another one, returning the quotient.
     */
    auto integer::operator /(const integer &v) const -> integer {
        integer res;
        res.negative_ = negative_ ^ v.negative_;
        res.magnitude_ = magnitude_ / v.magnitude_;
        return res;
    }

    /*
     * Divide this integer by another one, returning the remainder.
     */
    auto integer::operator %(const integer &v) const -> integer {
        integer res;
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ % v.magnitude_;
        return res;
    }

    /*
     * Convert this integer to a long long.
     */
    auto integer::to_long_long() const -> long long {
        /*
         * Will this number fit in a long long?  If not then throw an exception.
         */
        if (magnitude_.count_bits() > ((8 * sizeof(long long)) - 1)) {
            throw overflow_error();
        }

        /*
         * Convert the result, and, if necessary, flip the sign.
         */
        long long res = static_cast<long long>(to_unsigned_long_long(magnitude_));

        if (negative_) {
            res = -res;
        }

        return res;
    }

    /*
     * << operator to print a integer.
     */
    auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream & {
        if (v.negative_) {
            outstr << '-';
        }

        outstr << v.magnitude_;

        return outstr;
    }
}

