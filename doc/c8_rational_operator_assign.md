# c8::rational::operator =, +=, -=, \*=, /= #

Assignment operators.

## Definitions ##

1. `auto operator =(const rational &v) -> rational &`
2. `auto operator =(const rational &&v) noexcept -> rational &`
3. `auto operator +=(const rational &v) -> rational &`
4. `auto operator -=(const rational &v) -> rational &`
5. `auto operator *=(const rational &v) -> rational &`
6. `auto operator /=(const rational &v) -> rational &`

## Descriptions ##

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

1. Copy the rational `v`.
2. Move the rational `v`, leaving `v` as zero.
3. Add `v` to this number.
4. Subtract `v` from this number.
5. Multiply this number by `v`.
6. Divide this number by `v`.

## Returns ##

All operators return a reference to this number (left hand side of the operator).

## Exceptions ##

All operators other than number 2 may throw `std::bad_alloc` in the event of a memory allocation failure.

In addition:

1. Nothing
2. Nothing
3. Nothing
4. Nothing
5. Nothing
6. `c8::divide_by_zero` if the divisor, `v` is zero.

