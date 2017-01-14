/*
 * real10.h
 */
#ifndef __C8_REAL10_H
#define __C8_REAL10_H

#include <integer.h>

namespace c8 {
    enum class real10_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class real10 {
    public:
        /*
         * Constructors.
         */
        real10() {
            sig_ = 0;
            exp_ = 0;
        }

        real10(long long s, long long e) : sig_(s), exp_(e) {
        }

        real10(const std::string &v);
        real10(const real10 &v) = default;
        real10(real10 &&v) = default;
        ~real10() = default;
        auto operator =(const real10 &v) -> real10 & = default;
        auto operator =(real10 &&v) -> real10 & = default;

        auto add(const real10 &v) const -> real10;
        auto subtract(const real10 &v) const -> real10;
        auto multiply(const real10 &v) const -> real10;
        auto divide(const real10 &v) const -> real10;
        auto compare(const real10 &v) const -> real10_comparison;
        friend auto operator<<(std::ostream &outstr, const real10 &v) -> std::ostream &;

        auto operator+(const real10 &v) const -> real10 {
            return add(v);
        }

        auto operator+=(const real10 &v) {
            *this = add(v);
        }

        auto operator-(const real10 &v) const -> real10 {
            return subtract(v);
        }

        auto operator-() const -> real10 {
            real10 res = *this;
            res.sig_ = -sig_;
            res.exp_ = exp_;
            return res;
        }

        auto operator-=(const real10 &v) {
            *this = subtract(v);
        }

        auto operator*(const real10 &v) const -> real10 {
            return multiply(v);
        }

        auto operator*=(const real10 &v) {
            *this = multiply(v);
        }

        auto operator/(const real10 &v) const -> real10 {
            return divide(v);
        }

        auto operator/=(const real10 &v) {
            *this = divide(v);
        }

        auto operator==(const real10 &v) const -> bool {
            return compare(v) == real10_comparison::eq;
        }

        auto operator!=(const real10 &v) const -> bool {
            return compare(v) != real10_comparison::eq;
        }

        auto operator>(const real10 &v) const -> bool {
            return compare(v) == real10_comparison::gt;
        }

        auto operator>=(const real10 &v) const -> bool {
            return compare(v) != real10_comparison::lt;
        }

        auto operator<(const real10 &v) const -> bool {
            return compare(v) == real10_comparison::lt;
        }

        auto operator<=(const real10 &v) const -> bool {
            return compare(v) != real10_comparison::gt;
        }

    private:
        integer sig_;                       // Significand
        integer exp_;                       // Exponent
    };
}

#endif /* __C8_REAL10_H */

