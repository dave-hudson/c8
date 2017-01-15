/*
 * scale10.cpp
 */
#include "scale10.h"

namespace c8 {
    scale10::scale10(const std::string &v) {
    }

    /*
     * Add another scale10 to this one.
     */
    auto scale10::add(const scale10 &v) const -> scale10 {
        scale10 res;
        return res;
    }

    /*
     * Subtract another scale10 from this one.
     *
     * Note that there is a subtle difference between the scale10 subtract operation
     * and the natural number version: We don't have to worry about throwing
     * exceptions for negative results.
     */
    auto scale10::subtract(const scale10 &v) const -> scale10 {
        scale10 res;
        return res;
    }

    /*
     * Multiply another scale10 with this one.
     */
    auto scale10::multiply(const scale10 &v) const -> scale10 {
        scale10 res;
        return res;
    }

    /*
     * Divide this scale10 by another one.
     */
    auto scale10::divide(const scale10 &v) const -> scale10 {
        scale10 res;
        return res;
    }

    /*
     * Compare a scale10 with this one.
     */
    auto scale10::compare(const scale10 &v) const -> scale10_comparison {
        return scale10_comparison::lt;
    }

    /*
     * << operator to print a scale10.
     */
    auto operator<<(std::ostream &outstr, const scale10 &v) -> std::ostream & {
        outstr << v.sig_ << "*10^" << v.exp_;

        return outstr;
    }
}

