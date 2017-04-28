# c8::natural::operator + #

Addition operators.

### Definitions ###

```cpp
auto operator +(const natural &v) const -> natural
auto operator +(natural_digit v) const -> natural
```

### Description ###

Adds this number (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::natural` number.

### Return Value ###

A new natural number equal to this number plus `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

