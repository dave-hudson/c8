# c8::natural::operator =, +=, -=, \*=, /=, %=, &lt;&lt;=, >>= #

Assignment operators.

## Definitions ##

1. `auto operator =(const natural &v) -> natural &`
2. `auto operator =(const natural &&v) noexcept -> natural &`
3. `auto operator +=(const natural &v) -> natural &`  
   `auto operator +=(natural_digit v) -> natural &`
4. `auto operator -=(const natural &v) -> natural &`  
   `auto operator -=(natural_digit v) -> natural &`
5. `auto operator *=(const natural &v) -> natural &`  
   `auto operator *=(natural_digit v) -> natural &`
6. `auto operator /=(const natural &v) -> natural &`  
   `auto operator /=(natural_digit v) -> natural &`
7. `auto operator %=(const natural &v) -> natural &`  
   `auto operator %=(natural_digit v) -> natural &`
8. `auto operator <<=(unsigned int count) -> natural &`
9. `auto operator >>=(unsigned int count) -> natural &`

## Descriptions ##

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

1. Copy the natural number `v`.
2. Move the natural number `v`, leaving `v` as zero.
3. Add `v` to this number.
4. Subtract `v` from this number.
5. Multiply this number by `v`.
6. Divide this number by `v`.  Any remainder is lost.
7. Keep the remainder when this number is divided by `v`.
8. Shift this number left by `count` bit positions.
9. Shift this number right by `count` bit positions.

## Returns ##

All operators return a reference to this number (left hand side of the operator).

## Exceptions ##

All operators other than number 2 may throw `std::bad_alloc` in the event of a memory allocation failure.

In addition:

1. Nothing
2. Nothing
3. Nothing
4. `c8::not_a_number` if the result of the subtraction would be negative.
5. Nothing
6. `c8::divide_by_zero` if the divisor, `v` is zero.
7. `c8::divide_by_zero` if the divisor, `v` is zero.
8. Nothing
9. Nothing

