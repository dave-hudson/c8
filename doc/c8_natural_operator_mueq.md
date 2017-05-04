# c8::natural::operator \*= #

```cpp
auto operator *=(const natural &v) -> natural &;
```

Multiplies the value of this object (left hand side of the operator) by `v` (right hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

