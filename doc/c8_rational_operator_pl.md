# c8::rational::operator + #

```cpp
auto operator +(const rational &v) const -> rational
```

Adds the value of this object (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::rational` object.

### Return Value ###

A new `c8::rational` object with a value equal to the sum of the value of this object and `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class rational](c8_rational)
* [c8 library](c8)

