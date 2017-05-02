# c8::integer::operator &lt;&lt;= #

```cpp
auto operator <<=(unsigned int count) -> integer &;
```

Shifts the value of this object (left hand side of the operator) left by `count` (right hand side of the operator) bit positions.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

