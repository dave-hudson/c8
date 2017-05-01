# c8::integer::operator - (Unary) #

```cpp
auto operator -() const -> integer
```

Generate a `c8::integer` object that has the negated value of this object (right hand side of the operator).

### Return Value ###

A new `c8::integer` object with a value equal to the negated value of this object.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

