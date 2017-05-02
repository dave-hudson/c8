# c8::natural::operator >> #

```cpp
auto operator >>(unsigned int count) const -> natural;
```

Shifts this number (left hand side of the operator) right by `count` (right hand side of the operator) bit positions, resulting in a new `c8::natural` number.  If the value of `count` is larger than the number of bits used to hold the original number then the result will be zero.

### Return Value ###

A new natural number equal to this number shifted right by `count` bit positions.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

