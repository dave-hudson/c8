# c8::rational::operator /= #

Division assignment operator.

### Definition ###

```cpp
auto operator /=(const rational &v) -> rational &
```

### Description ###

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Divide this number by `v`.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::divide_by_zero` if the divisor, `v` is zero.

