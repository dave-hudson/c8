# c8::natural::operator + #

```cpp
auto operator +(const natural &v) const -> natural;
auto operator +(natural_digit v) const -> natural;
```

Adds the value of this object (left hand side of the operator) and `v` (right hand side of the operator), resulting in a new `c8::natural` object.

### Return Value ###

A new `c8::natural` object with a value equal to the sum of the value of this object and `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

