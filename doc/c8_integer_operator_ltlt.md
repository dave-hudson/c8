# c8::integer::operator &lt;&lt; #

```cpp
auto operator <<(unsigned int count) const -> integer;
```

Shifts this number (left hand side of the operator) left by `count` (right hand side of the operator) bit positions, resulting in a new `c8::integer` number.

### Return Value ###

A new integer equal to this number shifted left by `count` bit positions.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

