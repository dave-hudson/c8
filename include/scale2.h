/*
 * scale2.h
 */
#ifndef __C8_SCALE2_H
#define __C8_SCALE2_H

#include <integer.h>

namespace c8 {
    enum class scale2_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class scale2 {
    public:
        /*
         * Constructors.
         */
        scale2() {
            sig_ = 0;
            exp_ = 0;
        }

        scale2(long long s, long long e) : sig_(s), exp_(e) {
        }

        scale2(const std::string &v);
        scale2(const scale2 &v) = default;
        scale2(scale2 &&v) = default;
        ~scale2() = default;
        auto operator =(const scale2 &v) -> scale2 & = default;
        auto operator =(scale2 &&v) -> scale2 & = default;

        auto add(const scale2 &v) const -> scale2;
        auto subtract(const scale2 &v) const -> scale2;
        auto multiply(const scale2 &v) const -> scale2;
        auto divide(const scale2 &v) const -> scale2;
        auto compare(const scale2 &v) const -> scale2_comparison;
        friend auto operator<<(std::ostream &outstr, const scale2 &v) -> std::ostream &;

        auto operator+(const scale2 &v) const -> scale2 {
            return add(v);
        }

        auto operator+=(const scale2 &v) {
            *this = add(v);
        }

        auto operator-(const scale2 &v) const -> scale2 {
            return subtract(v);
        }

        auto operator-() const -> scale2 {
            scale2 res = *this;
            res.sig_ = -sig_;
            res.exp_ = exp_;
            return res;
        }

        auto operator-=(const scale2 &v) {
            *this = subtract(v);
        }

        auto operator*(const scale2 &v) const -> scale2 {
            return multiply(v);
        }

        auto operator*=(const scale2 &v) {
            *this = multiply(v);
        }

        auto operator/(const scale2 &v) const -> scale2 {
            return divide(v);
        }

        auto operator/=(const scale2 &v) {
            *this = divide(v);
        }

        auto operator==(const scale2 &v) const -> bool {
            return compare(v) == scale2_comparison::eq;
        }

        auto operator!=(const scale2 &v) const -> bool {
            return compare(v) != scale2_comparison::eq;
        }

        auto operator>(const scale2 &v) const -> bool {
            return compare(v) == scale2_comparison::gt;
        }

        auto operator>=(const scale2 &v) const -> bool {
            return compare(v) != scale2_comparison::lt;
        }

        auto operator<(const scale2 &v) const -> bool {
            return compare(v) == scale2_comparison::lt;
        }

        auto operator<=(const scale2 &v) const -> bool {
            return compare(v) != scale2_comparison::gt;
        }

    private:
        integer sig_;                       // Significand
        integer exp_;                       // Exponent
    };
}

#endif /* __C8_SCALE2_H */

