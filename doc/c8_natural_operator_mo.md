# c8::natural::operator % #

Modulus operators.

### Definitions ###

```cpp
auto operator %(const natural &v) const -> natural
auto operator %(natural_digit v) const -> natural_digit
```

### Description ###

Perform arithmetic operations based on this number (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::natural` number.  The quotient is ignored.

### Return Value ###

A new natural number equal to the remainder when this number is divided by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

