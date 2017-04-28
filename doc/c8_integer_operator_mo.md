# c8::integer::operator % #

Modulus operator.

### Definition ###

```cpp
auto operator %(const integer &v) const -> integer
```

### Description ###

Divides this number (left hand side of the operator) by `v` (right hand side of the operator), resulting in a new `c8::integer` number that represents the remainder.  The quotient is ignored.

### Return Value ###

A new integer number equal to the remainder when this number is divided by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

