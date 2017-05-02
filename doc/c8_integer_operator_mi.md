# c8::integer::operator - #

```cpp
auto operator -(const integer &v) const -> integer;
```

Subtracts `v` (right hand side of the operator) from the value of this object (left hand side of the operator), resulting in a new `c8::integer` number.

### Return Value ###

A new `c8::integer` object with a value equal to `v` subtracted from this number.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

