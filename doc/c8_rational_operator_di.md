# c8::rational::operator / #

```cpp
auto operator /(const rational &v) const -> rational;
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator), resulting in a new `c8::natural` object.

### Return Value ###

A new `c8::rational` object with a value equal to the value of this object divided by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class rational](c8_rational)
* [c8 library](c8)

