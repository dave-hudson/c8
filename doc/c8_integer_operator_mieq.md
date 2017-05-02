# c8::integer::operator -= #

```cpp
auto operator -=(const integer &v) -> integer &;
```

Subtracts `v` (right hand side of the operator) from the value of this object (left hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

