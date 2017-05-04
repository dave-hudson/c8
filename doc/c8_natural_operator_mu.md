# c8::natural::operator \* #

```cpp
auto operator *(const natural &v) const -> natural;
```

Multiplies the value of this object (left hand side of the operator) by the value of `v` (right hand side of the operator), resulting in a new `c8::natural` object.

### Return Value ###

A new `c8::natural` object with a value equal to the value of this object multiplied by the value of `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

