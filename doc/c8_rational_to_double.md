# c8::rational::is\_zero #

```cpp
auto to_double() const -> double;
```

Returns the value of this object as a double-precision floating point value.

### Return Value ###

The value of this object, represented as a double-precision floating point value.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::overflow_error` if this object's value is too large to represent as a double-precision value.
* `c8::underflow_error` if this object's value is too small (too close to zero) to represent as a double-precision value.


## See Also ##

* [class rational](c8_rational)
* [c8 library](c8)

