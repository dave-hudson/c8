/*
 * integer.cpp
 */
#include "integer.h"

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
     * Add another integer to this one.
     */
    auto integer::operator +(const integer &v) const -> integer {
        /*
         * If our two numbers have the same sign then we just add and retain the
         * sign for this number.
         */
        if (negative_ == v.negative_) {
            integer res;
            res.negative_ = negative_;
            res.magnitude_ = magnitude_ + v.magnitude_;
            return res;
        }

        /*
         * If we are adding a larger magnitude value then subtract the smaller
         * from the larger, and retain the sign of the larger.
         */
        if (magnitude_ < v.magnitude_) {
            integer res;
            res.negative_ = v.negative_;
            res.magnitude_ = v.magnitude_ - magnitude_;
            return res;
        }

        /*
         * We're adding a smaller magnitude value, so subtract the smaller
         * from the larger, and, again, retain the sign of the larger.
         */
        integer res;
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
    auto integer::operator -(const integer &v) const -> integer {
        /*
         * If we're subtracting a negative number from a positive, or a positive from
         * a negative, then we add the magnitudes of both and retain the sign of this one.
         */
        if (negative_ != v.negative_) {
            integer res;
            res.negative_ = negative_;
            res.magnitude_ = magnitude_ + v.magnitude_;
            return res;
        }

        /*
         * If we're subtracting a number that has a larger magnitude than this one, then
         * subtract the magnitudes and use the inverse of the sign for the larger.
         */
        if (magnitude_ < v.magnitude_) {
            integer res;
            res.negative_ = !v.negative_;
            res.magnitude_ = v.magnitude_ - magnitude_;
            return res;
        }

        /*
         * We're subtracting a number with a smaller magnitude than this one, so subtract
         * the magnitudes and use the sign of this one.
         */
        integer res;
        res.negative_ = negative_;
        res.magnitude_ = magnitude_ - v.magnitude_;
        return res;
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
     * Divide this integer by another one, returning the quotient and modulus.
     */
    auto integer::divide_modulus(const integer &v) const -> std::pair<integer, integer> {
        std::pair<natural, natural> dm = magnitude_.divide_modulus(v.magnitude_);

        integer div_res;
        div_res.negative_ = negative_ ^ v.negative_;
        div_res.magnitude_ = dm.first;

        integer mod_res;
        mod_res.magnitude_ = dm.second;
        return std::make_pair(div_res, mod_res);
    }

    /*
     * Compare a integer with this one.
     */
    auto integer::compare(const integer &v) const -> comparison {
        /*
         * Is this number non-negative?
         */
        if (!negative_) {
            /*
             * If the number we're comparing with is non-negative too then we can
             * just do a simple comparison of the magnitudes.
             */
            if (!v.negative_) {
                return magnitude_.compare(v.magnitude_);
            }

            /*
             * The number we're comparing with is negative so we're definitely greater
             * than it.
             */
            return comparison::gt;
        }

        /*
         * This integer is negative, so if the number we're comparing with is also
         * negative then we compare magnitudes, but we reverse the result.  The most
         * negative value is smaller, not larger.
         */
        if (v.negative_) {
            auto ures = magnitude_.compare(v.magnitude_);
            switch (ures) {
            case comparison::lt:
                return comparison::gt;

            case comparison::eq:
                return comparison::eq;

            case comparison::gt:
                return comparison::lt;
            }
        }

        /*
         * This integer is negative, but the one we're comparing with is not, so we're
         * smaller.
         */
        return comparison::lt;
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

