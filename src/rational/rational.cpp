/*
 * rational.cpp
 */
#include "rational.h"

namespace c8 {
    rational::rational(const std::string &v) {
    }

    /*
     * Add another rational to this one.
     */
    auto rational::add(const rational &v) const -> rational {
        rational res;
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
        return res;
    }

    /*
     * Multiply another rational with this one.
     */
    auto rational::multiply(const rational &v) const -> rational {
        rational res;
        return res;
    }

    /*
     * Divide this rational by another one, returning the quotient and modulus.
     */
    auto rational::divide_modulus(const rational &v) const -> std::pair<rational, rational> {
        rational div_res;
        rational mod_res;
        return std::make_pair(div_res, mod_res);
    }

    /*
     * Divide this rational by another one.
     */
    auto rational::divide(const rational &v) const -> rational {
        std::pair<rational, rational> dm = divide_modulus(v);
        return dm.first;
    }

    /*
     * Modulus (divide and return the remainder) this rational by another one.
     */
    auto rational::modulus(const rational &v) const -> rational {
        std::pair<rational, rational> dm = divide_modulus(v);
        return dm.second;
    }

    /*
     * Compare a rational with this one.
     */
    auto rational::compare(const rational &v) const -> rational_comparison {
        return rational_comparison::lt;
    }

    /*
     * << operator to print a rational.
     */
    auto operator<<(std::ostream &outstr, const rational &v) -> std::ostream & {
        outstr << v.num_ << '/' << v.denom_;

        return outstr;
    }
}

