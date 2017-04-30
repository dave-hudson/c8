# c8::natural::operator %= #

```cpp
auto operator %=(const natural &v) -> natural &
auto operator %=(natural_digit v) -> natural &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Keep the remainder when this number is divided by `v`.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

