# c8::rational::operator +, -, \*, /, % #

Arithmetic operators.

## Definitions ##

1. `auto operator +(const rational &v) const -> rational`
2. `auto operator -(const rational &v) const -> rational`
3. `auto operator *(const rational &v) const -> rational`
4. `auto operator /(const rational &v) const -> rational`

## Descriptions ##

Perform arithmetic operations based on this number (left hand side of the operator) and another number `v` (right hand side of the operator), resulting in a new `c8::rational` number.

## Returns ##

1. Returns a new rational number equal to this number plus `v`.
2. Returns a new rational number equal `v` subtracted from this number.
3. Returns a new rational number equal to this number multiplied by `v`.
4. Returns a new rational number equal to this number divided by `v`.

## Exceptions ##

All operators may throw `std::bad_alloc` in the event of a memory allocation failure.

In addition:

1. Nothing
2. Nothing.
3. Nothing
4. `c8::divide_by_zero` if the divisor, `v` is zero.

