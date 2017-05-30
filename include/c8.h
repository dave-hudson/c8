/*
 * c8.h
 */
#ifndef __C8_H
#define __C8_H

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <cstdint>

/*
 * Macros to allow the compiler to make better choices about code structure.
 */
#define C8_LIKELY(x) __builtin_expect(!!(x), 1)
#define C8_UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace c8 {
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
     */
#if defined(C8_DIGIT_8_BITS)
    typedef uint8_t natural_digit;
    typedef uint16_t natural_double_digit;
#elif defined(C8_DIGIT_16_BITS)
    typedef uint16_t natural_digit;
    typedef uint32_t natural_double_digit;
#elif defined(C8_DIGIT_32_BITS)
    typedef uint32_t natural_digit;
    typedef uint64_t natural_double_digit;
#else
    typedef uint64_t natural_digit;
    typedef unsigned __int128 natural_double_digit;
#endif

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
        }

        natural(unsigned long long v) noexcept;
        natural(const std::string &v);
        natural(const natural &v);
        natural(natural &&v) noexcept;

        ~natural() = default;

        auto operator ==(const natural &v) const noexcept -> bool;
        auto operator !=(const natural &v) const noexcept -> bool;
        auto operator >(const natural &v) const noexcept -> bool;
        auto operator >=(const natural &v) const noexcept -> bool;
        auto operator <(const natural &v) const noexcept -> bool;
        auto operator <=(const natural &v) const noexcept -> bool;

        auto operator +(const natural &v) const -> natural;
        auto operator -(const natural &v) const -> natural;
        auto operator *(const natural &v) const -> natural;
        auto operator /(const natural &v) const -> natural;
        auto operator %(const natural &v) const -> natural;
        auto operator <<(unsigned int count) const -> natural;
        auto operator >>(unsigned int count) const -> natural;

        auto operator =(const natural &v) -> natural &;
        auto operator =(natural &&v) noexcept -> natural &;
        auto operator +=(const natural &v) -> natural &;
        auto operator -=(const natural &v) -> natural &;
        auto operator <<=(unsigned int count) -> natural &;
        auto operator >>=(unsigned int count) -> natural &;
        auto operator *=(const natural &v) -> natural &;
        auto operator /=(const natural &v) -> natural &;
        auto operator %=(const natural &v) -> natural &;

        auto size_bits() const noexcept -> std::size_t;
        auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
        auto gcd(const natural &v) const -> natural;

        /*
         * Is this number zero?
         */
        auto is_zero() const noexcept -> bool {
            return (num_digits_ == 0) ? true : false;
        }

        auto to_unsigned_long_long() const -> unsigned long long;

        friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &;

    private:
        std::size_t num_digits_;        // The number of digits in this number
        std::size_t digits_size_;       // Number of digits_ allocated
        natural_digit *digits_;         // Digits of the natural number
        natural_digit small_digits_[64 / sizeof(natural_digit)];
                                        // Small fixed-size digit buffer
        std::unique_ptr<natural_digit[]> large_digits_;
                                        // Pointer to large digit buffer, if needed

        auto reserve(std::size_t new_digits) -> void;
        auto expand(std::size_t new_digits) -> void;
        auto copy_digits(const natural &v) -> void;
        auto steal_digits(natural &v) -> void;
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
        integer() noexcept {
            negative_ = false;
        }

        integer(long long v) noexcept : magnitude_((v >= 0) ? static_cast<unsigned long long>(v) : static_cast<unsigned long long>(-v)) {
            negative_ = (v < 0) ? true : false;
        }

        integer(const natural &v) : magnitude_(v) {
            negative_ = false;
        }

        integer(const std::string &v);
        integer(const integer &v) = default;
        integer(integer &&v) noexcept = default;

        ~integer() = default;

        auto operator ==(const integer &v) const noexcept -> bool;
        auto operator !=(const integer &v) const noexcept -> bool;
        auto operator >(const integer &v) const noexcept -> bool;
        auto operator >=(const integer &v) const noexcept -> bool;
        auto operator <(const integer &v) const noexcept -> bool;
        auto operator <=(const integer &v) const noexcept -> bool;

        auto operator +(const integer &v) const -> integer;
        auto operator -(const integer &v) const -> integer;
        auto operator *(const integer &v) const -> integer;
        auto operator /(const integer &v) const -> integer;
        auto operator %(const integer &v) const -> integer;
        auto operator >>(unsigned int count) const -> integer;
        auto operator <<(unsigned int count) const -> integer;

        auto operator -() const -> integer {
            integer res = *this;
            res.negative_ ^= true;
            return res;
        }

        auto operator =(const integer &v) -> integer & = default;
        auto operator =(integer &&v) -> integer & = default;
        auto operator +=(const integer &v) -> integer &;
        auto operator -=(const integer &v) -> integer &;

        /*
         * Multiply another integer with this one.
         */
        auto operator *=(const integer &v) -> integer & {
            negative_ ^= v.negative_;
            magnitude_ *= v.magnitude_;
            return *this;
        }

        /*
         * Divide this integer by another one, returning the quotient.
         */
        auto operator /=(const integer &v) -> integer & {
            negative_ ^= v.negative_;
            magnitude_ /= v.magnitude_;
            return *this;
        }

        /*
         * Divide this integer by another one, returning the remainder.
         */
        auto operator %=(const integer &v) -> integer & {
            magnitude_ %= v.magnitude_;
            return *this;
        }

        /*
         * Left shift this integer by a number of bits.
         */
        auto operator <<=(unsigned int count) -> integer & {
            magnitude_ <<= count;
            return *this;
        }

        /*
         * Right shift this integer by a number of bits.
         */
        auto operator >>=(unsigned int count) -> integer & {
            magnitude_ >>= count;
            return *this;
        }

        auto divide_modulus(const integer &v) const -> std::pair<integer, integer>;

        /*
         * Is this number zero?
         */
        auto is_zero() const noexcept -> bool {
            return magnitude_.is_zero();
        }

        /*
         * Is this number negative?
         */
        auto is_negative() const -> bool {
            return negative_;
        }

        /*
         * Return the magnitude of this integer.
         */
        auto magnitude() const -> natural {
            return magnitude_;
        }

        /*
         * Negate this integer.
         */
        auto negate() -> integer & {
            negative_ ^= true;
            return *this;
        }

        auto gcd(const integer &v) const -> integer {
            integer res;
            res.magnitude_ = magnitude_.gcd(v.magnitude_);

            return res;
        }

        auto to_long_long() const -> long long;

        friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &;

    private:
        bool negative_;                 // Is this big integer negative?
        natural magnitude_;             // The magnitude of the integer
    };

    inline auto is_zero(const integer &v) -> bool {
        return v.is_zero();
    }

    inline auto is_negative(integer v) -> bool {
        return v.is_negative();
    }

    inline auto magnitude(integer v) -> natural {
        return v.magnitude();
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
        rational() noexcept {
            numerator_ = 0;
            denominator_ = 1;
        }

        rational(long long n, long long d = 1) noexcept : numerator_(n), denominator_(d) {
            normalize();
        }

        rational(const integer &n, const integer &d) : numerator_(n), denominator_(d) {
            normalize();
        }

        rational(double v);
        rational(const std::string &v);
        rational(const rational &v) = default;
        rational(rational &&v) noexcept = default;

        ~rational() = default;

        auto operator ==(const rational &v) const -> bool;
        auto operator !=(const rational &v) const -> bool;
        auto operator >(const rational &v) const -> bool;
        auto operator >=(const rational &v) const -> bool;
        auto operator <(const rational &v) const -> bool;
        auto operator <=(const rational &v) const -> bool;

        auto operator +(const rational &v) const -> rational;
        auto operator -(const rational &v) const -> rational;
        auto operator *(const rational &v) const -> rational;
        auto operator /(const rational &v) const -> rational;

        auto operator -() const -> rational {
            rational res;
            res.numerator_ = -numerator_;
            res.denominator_ = denominator_;
            return res;
        }

        auto operator =(const rational &v) -> rational & = default;
        auto operator =(rational &&v) -> rational & = default;
        auto operator +=(const rational &v) -> rational &;
        auto operator -=(const rational &v) -> rational &;
        auto operator *=(const rational &v) -> rational &;
        auto operator /=(const rational &v) -> rational &;

        /*
         * Is this number zero?
         */
        auto is_zero() const noexcept -> bool {
            return numerator_.is_zero();
        }

        auto to_double() const -> double;

        auto to_parts() const -> std::pair<integer, integer> {
            return std::make_pair(numerator_, denominator_);
        }

        friend auto operator <<(std::ostream &outstr, const rational &v) -> std::ostream &;

    private:
        integer numerator_;             // Numerator
        integer denominator_;           // Denominator

        auto normalize() -> void;
    };

    inline auto is_zero(const rational &v) -> bool {
        return v.is_zero();
    }

    inline auto to_double(const rational &v) -> double {
        return v.to_double();
    }

    inline auto to_parts(const rational &v) -> std::pair<integer, integer> {
        return v.to_parts();
    }
}

#endif // __C8_H

