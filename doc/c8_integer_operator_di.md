# c8::integer::operator / #

```cpp
auto operator /(const integer &v) const -> integer
```

Divides this number (left hand side of the operator) by `v` (right hand side of the operator), resulting in a new `c8::integer` number that represents the quotient.  Any remainder is ignored.

### Return Value ###

A new integer number equal to this number divided by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

