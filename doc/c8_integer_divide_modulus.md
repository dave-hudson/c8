# c8::integer::divide\_modulus #

```cpp
auto divide_modulus(const integer &v) const -> std::pair<integer, integer>
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator).  The result is a pair that represents the quotient, and the remainder.

### Return Value ###

A `std::pair` that represents the quotient and the remainder.  `first` represents the quotient, while `second` represents the remainder.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

