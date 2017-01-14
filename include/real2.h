/*
 * real2.h
 */
#ifndef __C8_REAL2_H
#define __C8_REAL2_H

#include <integer.h>

namespace c8 {
    enum class real2_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class real2 {
    public:
        /*
         * Constructors.
         */
        real2() {
            sig_ = 0;
            exp_ = 0;
        }

        real2(long long s, long long e) : sig_(s), exp_(e) {
        }

        real2(const std::string &v);
        real2(const real2 &v) = default;
        real2(real2 &&v) = default;
        ~real2() = default;
        auto operator =(const real2 &v) -> real2 & = default;
        auto operator =(real2 &&v) -> real2 & = default;

        auto add(const real2 &v) const -> real2;
        auto subtract(const real2 &v) const -> real2;
        auto multiply(const real2 &v) const -> real2;
        auto divide(const real2 &v) const -> real2;
        auto compare(const real2 &v) const -> real2_comparison;
        friend auto operator<<(std::ostream &outstr, const real2 &v) -> std::ostream &;

        auto operator+(const real2 &v) const -> real2 {
            return add(v);
        }

        auto operator+=(const real2 &v) {
            *this = add(v);
        }

        auto operator-(const real2 &v) const -> real2 {
            return subtract(v);
        }

        auto operator-() const -> real2 {
            real2 res = *this;
            res.sig_ = -sig_;
            res.exp_ = exp_;
            return res;
        }

        auto operator-=(const real2 &v) {
            *this = subtract(v);
        }

        auto operator*(const real2 &v) const -> real2 {
            return multiply(v);
        }

        auto operator*=(const real2 &v) {
            *this = multiply(v);
        }

        auto operator/(const real2 &v) const -> real2 {
            return divide(v);
        }

        auto operator/=(const real2 &v) {
            *this = divide(v);
        }

        auto operator==(const real2 &v) const -> bool {
            return compare(v) == real2_comparison::eq;
        }

        auto operator!=(const real2 &v) const -> bool {
            return compare(v) != real2_comparison::eq;
        }

        auto operator>(const real2 &v) const -> bool {
            return compare(v) == real2_comparison::gt;
        }

        auto operator>=(const real2 &v) const -> bool {
            return compare(v) != real2_comparison::lt;
        }

        auto operator<(const real2 &v) const -> bool {
            return compare(v) == real2_comparison::lt;
        }

        auto operator<=(const real2 &v) const -> bool {
            return compare(v) != real2_comparison::gt;
        }

    private:
        integer sig_;                       // Significand
        integer exp_;                       // Exponent
    };
}

#endif /* __C8_REAL2_H */

