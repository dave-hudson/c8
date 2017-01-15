/*
 * scale2.cpp
 */
#include "scale2.h"

namespace c8 {
    scale2::scale2(const std::string &v) {
    }

    /*
     * Add another scale2 to this one.
     */
    auto scale2::add(const scale2 &v) const -> scale2 {
        scale2 res;
        return res;
    }

    /*
     * Subtract another scale2 from this one.
     *
     * Note that there is a subtle difference between the scale2 subtract operation
     * and the natural number version: We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto scale2::subtract(const scale2 &v) const -> scale2 {
        scale2 res;
        return res;
    }

    /*
     * Multiply another scale2 with this one.
     */
    auto scale2::multiply(const scale2 &v) const -> scale2 {
        scale2 res;
        return res;
    }

    /*
     * Divide this scale2 by another one.
     */
    auto scale2::divide(const scale2 &v) const -> scale2 {
        scale2 res;
        return res;
    }

    /*
     * Compare a scale2 with this one.
     */
    auto scale2::compare(const scale2 &v) const -> scale2_comparison {
        return scale2_comparison::lt;
    }

    /*
     * << operator to print a scale2.
     */
    auto operator<<(std::ostream &outstr, const scale2 &v) -> std::ostream & {
        outstr << v.sig_ << "*2^" << v.exp_;

        return outstr;
    }
}

