# c8::natural::operator - #

```cpp
auto operator -(const natural &v) const -> natural
auto operator -(natural_digit v) const -> natural
```

Subtracts `v` (right hand side of the operator) from the value of this object (left hand side of the operator), resulting in a new `c8::natural` number.

### Return Value ###

A new `c8::natural` object with a value equal to `v` subtracted from this number.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::not_a_number` if the result of the subtraction would be negative.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

