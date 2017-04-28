# c8::rational::operator \* #

Multiplication operator.

## Definition ##

```cpp
auto operator *(const rational &v) const -> rational
```

## Description ##

Perform arithmetic operations based on this number (left hand side of the operator) and another number `v` (right hand side of the operator), resulting in a new `c8::rational` number.

## Return Value ##

A new rational number equal to this number multiplied by `v`.

## Exceptions ##

* `std::bad_alloc` in the event of a memory allocation failure.

