# c8::integer::operator =, +=, -=, \*=, /=, %=, &lt;&lt;=, >>= #

Assignment operators.

## Definitions ##

1. `auto operator =(const integer &v) -> integer &`
2. `auto operator =(const integer &&v) noexcept -> integer &`
3. `auto operator +=(const integer &v) -> integer &`
4. `auto operator -=(const integer &v) -> integer &`
5. `auto operator *=(const integer &v) -> integer &`
6. `auto operator /=(const integer &v) -> integer &`
7. `auto operator %=(const integer &v) -> integer &`
8. `auto operator <<=(unsigned int count) -> integer &`
9. `auto operator >>=(unsigned int count) -> integer &`

## Descriptions ##

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

1. Copy the integer `v`.
2. Move the integer `v`, leaving `v` as zero.
3. Add `v` to this number.
4. Subtract `v` from this number.
5. Multiply this number by `v`.
6. Divide this number by `v`.  Any remainder is lost.
7. Keep the remainder when this number is divided by `v`.
8. Shift this number left by `count` bit positions.
9. Shift this number right by `count` bit positions.

## Return Values ##

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
7. `c8::divide_by_zero` if the divisor, `v` is zero.
8. Nothing
9. Nothing

