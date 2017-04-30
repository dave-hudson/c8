# c8::natural::operator /= #

```cpp
auto operator /=(const natural &v) -> natural &
auto operator /=(natural_digit v) -> natural &
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator).  Any remainder is lost.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

