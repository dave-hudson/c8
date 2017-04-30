# c8::integer::operator /= #

```cpp
auto operator /=(const integer &v) -> integer &
```

Divides this number (left hand side of the operator) by `v` (right hand side of the operator), and assigns the result back to this number.  Any remainder is lost.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

