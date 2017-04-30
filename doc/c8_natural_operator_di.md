# c8::natural::operator / #

```cpp
auto operator /(const natural &v) const -> natural
auto operator /(natural_digit v) const -> natural
```

Divides this number (left hand side of the operator) by `v` (right hand side of the operator), resulting in a new `c8::natural` number that represents the quotient.  Any remainder is ignored.

### Return Value ###

A new natural number equal to this number divided by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

