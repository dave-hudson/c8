# c8::integer::operator + #

```cpp
auto operator +(const integer &v) const -> integer
```

Adds this number (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::integer` number.

### Return Value ###

A new integer number equal to this number plus `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

