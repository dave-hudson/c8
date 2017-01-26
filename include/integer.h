/*
 * integer.h
 */
#ifndef __C8_INTEGER_H
#define __C8_INTEGER_H

#include <natural.h>

namespace c8 {
    enum class integer_comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    class integer {
    public:
        /*
         * Constructors.
         */
        integer() {
            negative_ = false;
        }

        integer(long long v) : magnitude_((v >= 0) ? static_cast<unsigned long long>(v) : static_cast<unsigned long long>(-v)) {
            negative_ = (v < 0) ? true : false;
        }

        integer(natural v) : magnitude_(v) {
            negative_ = false;
        }

        integer(const std::string &v);
        integer(const integer &v) = default;
        integer(integer &&v) = default;
        ~integer() = default;
        auto operator =(const integer &v) -> integer & = default;
        auto operator =(integer &&v) -> integer & = default;

        auto add(const integer &v) const -> integer;
        auto subtract(const integer &v) const -> integer;
        auto shiftr(unsigned int count) const -> integer;
        auto shiftl(unsigned int count) const -> integer;
        auto multiply(const integer &v) const -> integer;
        auto divide_modulus(const integer &v) const -> std::pair<integer, integer>;
        auto divide(const integer &v) const -> integer;
        auto modulus(const integer &v) const -> integer;
        auto compare(const integer &v) const -> integer_comparison;
        auto to_long_long() const -> long long;
        friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &;

        auto isnegative() const -> bool {
            return negative_;
        }

        auto abs() const -> natural {
            return magnitude_;
        }

        auto operator +(const integer &v) const -> integer {
            return add(v);
        }

        auto operator +=(const integer &v) {
            *this = add(v);
        }

        auto operator -(const integer &v) const -> integer {
            return subtract(v);
        }

        auto operator -() const -> integer {
            integer res = *this;
            res.negative_ ^= true;
            return res;
        }

        auto operator -=(const integer &v) {
            *this = subtract(v);
        }

        auto operator >>(unsigned int count) const -> integer {
            return shiftr(count);
        }

        auto operator >>=(unsigned int count) {
            *this = shiftr(count);
        }

        auto operator <<(unsigned int count) const -> integer {
            return shiftl(count);
        }

        auto operator <<=(unsigned int count) {
            *this = shiftl(count);
        }

        auto operator *(const integer &v) const -> integer {
            return multiply(v);
        }

        auto operator *=(const integer &v) {
            *this = multiply(v);
        }

        auto operator /(const integer &v) const -> integer {
            return divide(v);
        }

        auto operator /=(const integer &v) {
            *this = divide(v);
        }

        auto operator %(const integer &v) const -> integer {
            return modulus(v);
        }

        auto operator %=(const integer &v) {
            *this = modulus(v);
        }

        auto operator ==(const integer &v) const -> bool {
            return compare(v) == integer_comparison::eq;
        }

        auto operator !=(const integer &v) const -> bool {
            return compare(v) != integer_comparison::eq;
        }

        auto operator >(const integer &v) const -> bool {
            return compare(v) == integer_comparison::gt;
        }

        auto operator >=(const integer &v) const -> bool {
            return compare(v) != integer_comparison::lt;
        }

        auto operator <(const integer &v) const -> bool {
            return compare(v) == integer_comparison::lt;
        }

        auto operator <=(const integer &v) const -> bool {
            return compare(v) != integer_comparison::gt;
        }

    private:
        bool negative_;                     // Is this big integer negative?
        natural magnitude_;                 // The magnitude of the integer
    };

    inline auto isnegative(integer v) -> bool {
        return v.isnegative();
    }

    inline auto abs(integer v) -> natural {
        return v.abs();
    }

    inline auto to_long_long(const integer &v) -> long long {
        return v.to_long_long();
    }
}

#endif /* __C8_INTEGER_H */

