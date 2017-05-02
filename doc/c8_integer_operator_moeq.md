# c8::integer::operator %= #

```cpp
auto operator %=(const integer &v) -> integer &;
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator), retaining only the remainder.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

