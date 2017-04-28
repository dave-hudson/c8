# c8::integer::operator - #

Subtraction operator.

### Definition ###

```cpp
auto operator -(const integer &v) const -> integer
```

### Description ###

Subtracts `v` (right hand side of the operator) from this number (left hand side of the operator), resulting in a new `c8::integer` number.

### Return Value ###

A new integer number equal to `v` subtracted from this number.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

