/*
 * scale10.h
 */
#ifndef __C8_SCALE10_H
#define __C8_SCALE10_H

#include <integer.h>

namespace c8 {
    enum class scale10_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class scale10 {
    public:
        /*
         * Constructors.
         */
        scale10() {
            sig_ = 0;
            exp_ = 0;
        }

        scale10(long long s, long long e) : sig_(s), exp_(e) {
        }

        scale10(const std::string &v);
        scale10(const scale10 &v) = default;
        scale10(scale10 &&v) = default;
        ~scale10() = default;
        auto operator =(const scale10 &v) -> scale10 & = default;
        auto operator =(scale10 &&v) -> scale10 & = default;

        auto add(const scale10 &v) const -> scale10;
        auto subtract(const scale10 &v) const -> scale10;
        auto multiply(const scale10 &v) const -> scale10;
        auto divide(const scale10 &v) const -> scale10;
        auto compare(const scale10 &v) const -> scale10_comparison;
        friend auto operator<<(std::ostream &outstr, const scale10 &v) -> std::ostream &;

        auto operator+(const scale10 &v) const -> scale10 {
            return add(v);
        }

        auto operator+=(const scale10 &v) {
            *this = add(v);
        }

        auto operator-(const scale10 &v) const -> scale10 {
            return subtract(v);
        }

        auto operator-() const -> scale10 {
            scale10 res = *this;
            res.sig_ = -sig_;
            res.exp_ = exp_;
            return res;
        }

        auto operator-=(const scale10 &v) {
            *this = subtract(v);
        }

        auto operator*(const scale10 &v) const -> scale10 {
            return multiply(v);
        }

        auto operator*=(const scale10 &v) {
            *this = multiply(v);
        }

        auto operator/(const scale10 &v) const -> scale10 {
            return divide(v);
        }

        auto operator/=(const scale10 &v) {
            *this = divide(v);
        }

        auto operator==(const scale10 &v) const -> bool {
            return compare(v) == scale10_comparison::eq;
        }

        auto operator!=(const scale10 &v) const -> bool {
            return compare(v) != scale10_comparison::eq;
        }

        auto operator>(const scale10 &v) const -> bool {
            return compare(v) == scale10_comparison::gt;
        }

        auto operator>=(const scale10 &v) const -> bool {
            return compare(v) != scale10_comparison::lt;
        }

        auto operator<(const scale10 &v) const -> bool {
            return compare(v) == scale10_comparison::lt;
        }

        auto operator<=(const scale10 &v) const -> bool {
            return compare(v) != scale10_comparison::gt;
        }

    private:
        integer sig_;                       // Significand
        integer exp_;                       // Exponent
    };
}

#endif /* __C8_SCALE10_H */

