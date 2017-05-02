# c8::natural::operator &lt;&lt; #

```cpp
auto operator <<(unsigned int count) const -> natural;
```

Shifts this number (left hand side of the operator) left by `count` (right hand side of the operator) bit positions, resulting in a new `c8::natural` number.

### Return Value ###

A new natural number equal to this number shifted left by `count` bit positions.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

