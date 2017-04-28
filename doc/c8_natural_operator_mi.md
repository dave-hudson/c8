# c8::natural::operator - #

Subtraction operators.

### Definitions ###

```cpp
auto operator -(const natural &v) const -> natural
auto operator -(natural_digit v) const -> natural
```

### Description ###

Subtracts `v` (right hand side of the operator) from this number (left hand side of the operator), resulting in a new `c8::natural` number.

### Return Value ###

A new natural number equal `v` subtracted from this number.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::not_a_number` if the result of the subtraction would be negative.

