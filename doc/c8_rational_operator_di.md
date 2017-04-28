# c8::rational::operator / #

Division operator.

## Definition ##

```cpp
auto operator /(const rational &v) const -> rational
```

## Description ##

Perform arithmetic operations based on this number (left hand side of the operator) and another number `v` (right hand side of the operator), resulting in a new `c8::rational` number.

## Return Value ##

A new rational number equal to this number divided by `v`.

## Exceptions ##

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

