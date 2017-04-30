# c8::integer::operator \* #

```cpp
auto operator *(const integer &v) const -> integer
```

Multiplies this number (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::integer` number.

### Return Value ###

A new integer number equal to this number multiplied by `v`

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

