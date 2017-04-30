# c8::rational::operator /= #

```cpp
auto operator /=(const rational &v) -> rational &
```

Divides the value of this object (left hand side of the operator) by `v` (right hand side of the operator).  Any remainder is lost.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

