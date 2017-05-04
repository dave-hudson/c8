# c8::natural::operator %= #

```cpp
auto operator %=(const natural &v) -> natural &;
```

Divides the value of this object (left hand side of the operator) by the value of `v` (right hand side of the operator), retaining only the remainder.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

