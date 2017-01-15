/*
 * rational.cpp
 */
#include "rational.h"

namespace c8 {
    rational::rational(const std::string &v) {
        /*
         * Do we have a '/' character separating a numerator and denominator?
         */
        std::size_t pos = v.find(std::string("/"));
        if (pos == std::string::npos) {
            num_ = integer(v);
        } else {
            num_ = integer(v.substr(0, pos));
            denom_ = natural(v.substr(pos + 1));
        }

        normalize();
    }

    /*
     * Add another rational to this one.
     */
    auto rational::add(const rational &v) const -> rational {
        rational res;

        res.num_ = (num_ * integer(v.denom_)) + (integer(denom_) * v.num_);
        res.denom_ = denom_ * v.denom_;

        res.normalize();

        return res;
    }

    /*
     * Subtract another rational from this one.
     *
     * Note that there is a subtle difference between the rational subtract operation
     * and the natural number version: We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto rational::subtract(const rational &v) const -> rational {
        rational res;

        res.num_ = (num_ * integer(v.denom_)) - (integer(denom_) * v.num_);
        res.denom_ = denom_ * v.denom_;

        res.normalize();

        return res;
    }

    /*
     * Multiply another rational with this one.
     */
    auto rational::multiply(const rational &v) const -> rational {
        rational res;

        res.num_ = num_ * v.num_;
        res.denom_ = denom_ * v.denom_;

        res.normalize();

        return res;
    }

    /*
     * Divide this rational by another one.
     */
    auto rational::divide(const rational &v) const -> rational {
        rational res;

        /*
         * Are we attempting to divide by zero?  If we are then throw an exception.
         */
        if (iszero(abs(v.num_))) {
            throw std::logic_error("divide_by_zero");
        }

        res.num_ = num_ * integer(v.denom_);
        integer d = integer(denom_) * v.num_;
        if (isnegative(d)) {
            res.num_ = -res.num_;
        }

        res.denom_ = abs(d);

        res.normalize();

        return res;
    }

    /*
     * Compare a rational with this one.
     */
    auto rational::compare(const rational &v) const -> rational_comparison {
        integer lhs = num_ * integer(v.denom_);
        integer rhs = v.num_ * integer(denom_);

        auto ures = lhs.compare(rhs);
        switch (ures) {
        case integer_comparison::lt:
            return rational_comparison::lt;

        case integer_comparison::eq:
            return rational_comparison::eq;

        case integer_comparison::gt:
            return rational_comparison::gt;
        }
    }

    /*
     * Normalize the data.
     */
    auto rational::normalize() -> void {
        /*
         * Find the GCD of the numerator and denominator.
         */
        natural g = gcd(abs(num_), denom_);
        num_ /= g;
        denom_ /= g;
    }

    /*
     * << operator to print a rational.
     */
    auto operator<<(std::ostream &outstr, const rational &v) -> std::ostream & {
        outstr << v.num_ << '/' << v.denom_;

        return outstr;
    }
}

