/*
 * real10.cpp
 */
#include "real10.h"

namespace c8 {
    real10::real10(const std::string &v) {
    }

    /*
     * Add another real10 to this one.
     */
    auto real10::add(const real10 &v) const -> real10 {
        real10 res;
        return res;
    }

    /*
     * Subtract another real10 from this one.
     *
     * Note that there is a subtle difference between the real10 subtract operation
     * and the natural number version: We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto real10::subtract(const real10 &v) const -> real10 {
        real10 res;
        return res;
    }

    /*
     * Multiply another real10 with this one.
     */
    auto real10::multiply(const real10 &v) const -> real10 {
        real10 res;
        return res;
    }

    /*
     * Divide this real10 by another one.
     */
    auto real10::divide(const real10 &v) const -> real10 {
        real10 res;
        return res;
    }

    /*
     * Compare a real10 with this one.
     */
    auto real10::compare(const real10 &v) const -> real10_comparison {
        return real10_comparison::lt;
    }

    /*
     * << operator to print a real10.
     */
    auto operator<<(std::ostream &outstr, const real10 &v) -> std::ostream & {
        outstr << v.sig_ << "*10^" << v.exp_;

        return outstr;
    }
}

