/*
 * real2.cpp
 */
#include "real2.h"

namespace c8 {
    real2::real2(const std::string &v) {
    }

    /*
     * Add another real2 to this one.
     */
    auto real2::add(const real2 &v) const -> real2 {
        real2 res;
        return res;
    }

    /*
     * Subtract another real2 from this one.
     *
     * Note that there is a subtle difference between the real2 subtract operation
     * and the natural number version: We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto real2::subtract(const real2 &v) const -> real2 {
        real2 res;
        return res;
    }

    /*
     * Multiply another real2 with this one.
     */
    auto real2::multiply(const real2 &v) const -> real2 {
        real2 res;
        return res;
    }

    /*
     * Divide this real2 by another one.
     */
    auto real2::divide(const real2 &v) const -> real2 {
        real2 res;
        return res;
    }

    /*
     * Compare a real2 with this one.
     */
    auto real2::compare(const real2 &v) const -> real2_comparison {
        return real2_comparison::lt;
    }

    /*
     * << operator to print a real2.
     */
    auto operator<<(std::ostream &outstr, const real2 &v) -> std::ostream & {
        outstr << v.sig_ << "*2^" << v.exp_;

        return outstr;
    }
}

