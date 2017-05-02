# c8::rational::operator - (Unary) #

```cpp
auto operator -() const -> rational;
```

Generate a `c8::rational` object that has the negated value of this object (right hand side of the operator).

### Return Value ###

A new `c8::rational` object with a value equal to the negated value of this object.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class rational](c8_rational)
* [c8 library](c8)

