# c8::integer::operator \* #

```cpp
auto operator *(const integer &v) const -> integer
```

Multiplies the value of this object (left hand side of the operator) by `v` (right hand side of the operator), resulting in a new `c8::integer` object.

### Return Value ###

A new `c8::integer` object with a value equal to the value of this object multiplied by `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

