# c8::natural::divide\_modulus #

```cpp
auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>;
auto divide_modulus(const natural &v) const -> std::pair<natural, natural>;
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator).  The result is a pair that represents the quotient, and the remainder.

### Return Value ###

A `std::pair` that represents the quotient and the remainder.  `first` represents the quotient, while `second` represents the remainder.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

