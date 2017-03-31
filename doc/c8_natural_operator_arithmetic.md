# c8::natural::operator +, -, \*, /, % #

Arithmetic operators.

## Definitions ##

1. `auto operator +(const natural &v) const -> natural`
   `auto operator +(natural_digit v) const -> natural`
2. `auto operator -(const natural &v) const -> natural`
   `auto operator -(natural_digit v) const -> natural`
3. `auto operator *(const natural &v) const -> natural`
   `auto operator *(natural_digit v) const -> natural`
4. `auto operator /(const natural &v) const -> natural`
   `auto operator /(natural_digit v) const -> natural`
5. `auto operator %(const natural &v) const -> natural`
   `auto operator %(natural_digit v) const -> natural_digit`

## Descriptions ##

Perform arithmetic operations based on this number (left hand side of the operator) and another number `v` (right hand side of the operator), resulting in a new `c8::natural` number.

## Returns ##

1. Returns a new natural number equal to this number plus `v`.
2. Returns a new natural number equal `v` subtracted from this number.
3. Returns a new natural number equal to this number multiplied by `v`.
4. Returns a new natural number equal to this number divided by `v`.  Any remainder is ignored.
5. Returns a new natural number equal to the remainder when this number is divided by `v`.

## Exceptions ##

All operators may throw `std::bad_alloc` in the event of a memory allocation failure.

In addition:

1. Nothing
2. `c8::not_a_number` if the result of the subtraction would be negative.
3. Nothing
4. `c8::divide_by_zero` if the divisor, `v` is zero.
5. `c8::divide_by_zero` if the divisor, `v` is zero.

