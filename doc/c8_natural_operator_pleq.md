# c8::natural::operator += #

```cpp
auto operator +=(const natural &v) -> natural &
auto operator +=(natural_digit v) -> natural &
```

Adds `v` (right hand side of the operator) to the value of this object (left hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

