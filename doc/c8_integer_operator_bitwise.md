# c8::integer::operator &lt;&lt;, >> #

Bitwise operators.

## Definitions ##

1. `auto operator <<(unsigned int count) const -> integer`
2. `auto operator >>(unsigned int count) const -> integer`

## Descriptions ##

Perform bit manipulation operations on this number (left hand side of the operator) and another number `v` (right hand side of the operator), resulting in a new `c8::integer` number.

## Returns ##

1. Returns a new integer equal to this number shifted left by `count` bit positions.
1. Returns a new integer equal to this number shifted right by `count` bit positions.

## Exceptions ##

All operators may throw `std::bad_alloc` in the event of a memory allocation failure.

