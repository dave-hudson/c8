# c8::natural::operator / #

```cpp
auto operator /(const natural &v) const -> natural;
```

Divides the value of this object (left hand side of the operator) by the value of `v` (right hand side of the operator), resulting in a new `c8::natural` object that represents the quotient.  Any remainder is lost.

### Return Value ###

A new `c8::natural` object with a value equal to the quotient of the value of this object divided by the value of `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

