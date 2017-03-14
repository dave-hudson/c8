/*
 * c8.h
 */
#ifndef __C8_H
#define __C8_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

/*
 * Macros to allow the compiler to make better choices about code structure.
 */
#define C8_LIKELY(x) __builtin_expect(!!(x), 1)
#define C8_UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace c8 {
    /*
     * Comparison enum class.
     */
    enum class comparison {
        lt,                                 // Less than
        eq,                                 // Equal
        gt                                  // Greater than
    };

    /*
     * Exception class to signal an invalid argument.
     */
    class invalid_argument : public std::invalid_argument {
    public:
        invalid_argument(const std::string &s) : std::invalid_argument(s) {
        }
    };

    /*
     * Exception class to signal an overflow.
     */
    class overflow_error : public std::overflow_error {
    public:
        overflow_error() : std::overflow_error("overflow error") {
        }
    };

    /*
     * Exception class to signal an underflow.
     */
    class underflow_error : public std::underflow_error {
    public:
        underflow_error() : std::underflow_error("underflow error") {
        }
    };

    /*
     * Exception class to signal divide-by-zero.
     */
    class divide_by_zero : public std::runtime_error {
    public:
        divide_by_zero() : std::runtime_error("divide by zero") {
        }
    };

    /*
     * Exception class to signal not a number.
     */
    class not_a_number : public std::runtime_error {
    public:
        not_a_number() : std::runtime_error("not a number") {
        }
    };

    /*
     * Define the concepts of a "digit" and a "double digit" (twice the size of a digit.
     * A digit is much larger than a numeral, but the algorithms we use for things like
     * addition, subtraction, multiplication and division all work just the same, whether
     * our digits are 1 bit, 32 bits or range from 0 to 9.
     *
     * Other versions that would work here:
     *
     * typedef uint16_t natural_digit;
     * typedef uint32_t natural_double_digit;
     *
     * Or:
     *
     * typedef uint8_t natural_digit;
     * typedef uint16_t natural_double_digit;
     */
    typedef uint32_t natural_digit;
    typedef uint64_t natural_double_digit;

    const std::size_t natural_digit_bits = 8 * sizeof(natural_digit);

    class natural {
    public:
        /*
         * Constructors.
         */
        natural() noexcept {
            num_digits_ = 0;
            digits_size_ = sizeof(small_digits_) / sizeof(natural_digit);
            digits_ = small_digits_;
            delete_digits_on_final_ = false;
        }

        natural(unsigned long long v);
        natural(const std::string &v);

        /*
         * Copy constructor.
         */
        natural(const natural &v) {
            copy_digits(v);
        }

        /*
         * Move constructor.
         */
        natural(natural &&v) noexcept {
            steal_digits(v);
        }

        /*
         * Destructor.
         */
        ~natural() {
            delete_digits();
        }

        /*
         * Copy assignment operator.
         */
        auto operator =(const natural &v) -> natural & {
            /*
             * Are we assigning to ourself?  If we are then we don't need to do anything.
             */
            if (C8_UNLIKELY(this == &v)) {
                return *this;
            }

            /*
             * Delete the old contents of this natural number and copy the original's digits.
             */
            delete_digits();
            copy_digits(v);
            return *this;
        }

        /*
         * Move assignment operator.
         */
        auto operator =(natural &&v) noexcept -> natural & {
            /*
             * Are we assigning to ourself?  If we are then we don't have to do anything.
             */
            if (C8_UNLIKELY(this == &v)) {
                return *this;
            }

            /*
             * Delete the old contents of this natural number and steal the original's digits.
             */
            delete_digits();
            steal_digits(v);
            return *this;
        }

        auto count_bits() const noexcept -> unsigned int;
        auto compare(const natural &v) const noexcept -> comparison;
        auto operator +(natural_digit v) const -> natural;
        auto operator +(const natural &v) const -> natural;
        auto operator +=(natural_digit v) -> natural &;
        auto operator +=(const natural &v) -> natural &;
        auto operator -(natural_digit v) const -> natural;
        auto operator -(const natural &v) const -> natural;
        auto operator -=(natural_digit v) -> natural &;
        auto operator -=(const natural &v) -> natural &;
        auto operator <<(unsigned int count) const -> natural;
        auto operator <<=(unsigned int count) -> natural &;
        auto operator >>(unsigned int count) const -> natural;
        auto operator >>=(unsigned int count) -> natural &;
        auto operator *(natural_digit v) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto operator *=(natural_digit v) -> natural &;
        auto operator *=(const natural &v) -> natural &;
        auto operator /(natural_digit v) const -> natural;
        auto operator /(const natural &v) const -> natural;
        auto operator /=(natural_digit v) -> natural &;
        auto operator /=(const natural &v) -> natural &;
        auto operator %(natural_digit v) const -> natural_digit;
        auto operator %(const natural &v) const -> natural;
        auto operator %=(natural_digit v) -> natural &;
        auto operator %=(const natural &v) -> natural &;
        auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto gcd(const natural &v) const -> natural;
        auto to_unsigned_long_long() const -> unsigned long long;
        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

        /*
         * Is this number zero?
         */
        auto is_zero() const noexcept -> bool {
            return (num_digits_ == 0) ? true : false;
        }

        auto operator ==(const natural &v) const -> bool {
            return compare(v) == comparison::eq;
        }

        auto operator !=(const natural &v) const -> bool {
            return compare(v) != comparison::eq;
        }

        auto operator >(const natural &v) const -> bool {
            return compare(v) == comparison::gt;
        }

        auto operator >=(const natural &v) const -> bool {
            return compare(v) != comparison::lt;
        }

        auto operator <(const natural &v) const -> bool {
            return compare(v) == comparison::lt;
        }

        auto operator <=(const natural &v) const -> bool {
            return compare(v) != comparison::gt;
        }

    private:
        std::size_t num_digits_;            // The number of digits in this number
        std::size_t digits_size_;           // Number of digits_ allocated
        natural_digit *digits_;             // Digits of the natural number
        bool delete_digits_on_final_;       // Do we need to delete digits_ on finalizing?
        natural_digit small_digits_[16];    // Small fixed-size digit buffer

        auto reserve(std::size_t new_digits) -> void;
        auto expand(std::size_t new_digits) -> void;
        auto copy_digits(const natural &v) -> void;
        auto steal_digits(natural &v) -> void;

        /*
         * Delete digits array if it is marked for deletion.
         */
        auto delete_digits() -> void {
            if (C8_UNLIKELY(delete_digits_on_final_)) {
                delete[] digits_;
            }
        }
    };

    inline auto is_zero(const natural &v) -> bool {
        return v.is_zero();
    }

    inline auto gcd(const natural &v, const natural &u) -> natural {
        return v.gcd(u);
    }

    inline auto to_unsigned_long_long(const natural &v) -> unsigned long long {
        return v.to_unsigned_long_long();
    }

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

        auto operator +(const integer &v) const -> integer;
        auto operator -(const integer &v) const -> integer;
        auto operator >>(unsigned int count) const -> integer;
        auto operator <<(unsigned int count) const -> integer;
        auto operator *(const integer &v) const -> integer;
        auto divide_modulus(const integer &v) const -> std::pair<integer, integer>;
        auto operator /(const integer &v) const -> integer;
        auto operator %(const integer &v) const -> integer;
        auto compare(const integer &v) const -> comparison;
        auto to_long_long() const -> long long;
        friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &;

        auto is_negative() const -> bool {
            return negative_;
        }

        auto abs() const -> natural {
            return magnitude_;
        }

        auto negate() -> integer & {
            negative_ ^= true;
            return *this;
        }

        auto gcd(const integer &v) const -> integer {
            integer res;
            res.magnitude_ = magnitude_.gcd(v.magnitude_);

            return res;
        }

        auto operator +=(const integer &v) -> integer & {
            *this = *this + v;
            return *this;
        }

        auto operator -() const -> integer {
            integer res = *this;
            res.negative_ ^= true;
            return res;
        }

        auto operator -=(const integer &v) -> integer & {
            *this = *this - v;
            return *this;
        }

        auto operator >>=(unsigned int count) -> integer & {
            *this = *this >> count;
            return *this;
        }

        auto operator <<=(unsigned int count) -> integer & {
            *this = *this << count;
            return *this;
        }

        auto operator *=(const integer &v) -> integer & {
            *this = *this * v;
            return *this;
        }

        auto operator /=(const integer &v) -> integer & {
            *this = *this / v;
            return *this;
        }

        auto operator %=(const integer &v) -> integer & {
            *this = *this % v;
            return *this;
        }

        auto operator ==(const integer &v) const -> bool {
            return compare(v) == comparison::eq;
        }

        auto operator !=(const integer &v) const -> bool {
            return compare(v) != comparison::eq;
        }

        auto operator >(const integer &v) const -> bool {
            return compare(v) == comparison::gt;
        }

        auto operator >=(const integer &v) const -> bool {
            return compare(v) != comparison::lt;
        }

        auto operator <(const integer &v) const -> bool {
            return compare(v) == comparison::lt;
        }

        auto operator <=(const integer &v) const -> bool {
            return compare(v) != comparison::gt;
        }

    private:
        bool negative_;                     // Is this big integer negative?
        natural magnitude_;                 // The magnitude of the integer
    };

    inline auto is_negative(integer v) -> bool {
        return v.is_negative();
    }

    inline auto abs(integer v) -> natural {
        return v.abs();
    }

    inline auto gcd(const integer &v, const integer &u) -> integer {
        return v.gcd(u);
    }

    inline auto to_long_long(const integer &v) -> long long {
        return v.to_long_long();
    }

    class rational {
    public:
        /*
         * Constructors.
         */
        rational() {
            num_ = 0;
            denom_ = 1;
        }

        rational(long long n, long long d = 1) : num_(n), denom_(d) {
            normalize();
        }

        rational(integer n, integer d) : num_(n), denom_(d) {
            normalize();
        }

        rational(double v);
        rational(const std::string &v);
        rational(const rational &v) = default;
        rational(rational &&v) = default;
        ~rational() = default;
        auto operator =(const rational &v) -> rational & = default;
        auto operator =(rational &&v) -> rational & = default;

        auto operator +(const rational &v) const -> rational;
        auto operator -(const rational &v) const -> rational;
        auto operator *(const rational &v) const -> rational;
        auto operator /(const rational &v) const -> rational;
        auto compare(const rational &v) const -> comparison;
        auto to_double() const -> double;
        friend auto operator <<(std::ostream &outstr, const rational &v) -> std::ostream &;

        auto to_parts() const -> std::pair<integer, integer> {
            return std::make_pair(num_, denom_);
        }

        auto operator +=(const rational &v) -> rational & {
            *this = *this + v;
            return *this;
        }

        auto operator -() const -> rational {
            rational res;
            res.num_ = -num_;
            res.denom_ = denom_;
            return res;
        }

        auto operator -=(const rational &v) -> rational & {
            *this = *this - v;
            return *this;
        }

        auto operator *=(const rational &v) -> rational & {
            *this = *this * v;
            return *this;
        }

        auto operator /=(const rational &v) -> rational & {
            *this = *this / v;
            return *this;
        }

        auto operator ==(const rational &v) const -> bool {
            return compare(v) == comparison::eq;
        }

        auto operator !=(const rational &v) const -> bool {
            return compare(v) != comparison::eq;
        }

        auto operator >(const rational &v) const -> bool {
            return compare(v) == comparison::gt;
        }

        auto operator >=(const rational &v) const -> bool {
            return compare(v) != comparison::lt;
        }

        auto operator <(const rational &v) const -> bool {
            return compare(v) == comparison::lt;
        }

        auto operator <=(const rational &v) const -> bool {
            return compare(v) != comparison::gt;
        }

    private:
        integer num_;                       // Numerator
        integer denom_;                     // Denominator

        auto normalize() -> void;
    };

    inline auto to_double(const rational &v) -> double {
        return v.to_double();
    }

    inline auto to_parts(const rational &v) -> std::pair<integer, integer> {
        return v.to_parts();
    }
}

#endif // __C8_H

