# c8::natural::operator += #

Addition assignment operators.

### Definitions ###

```cpp
auto operator +=(const natural &v) -> natural &
auto operator +=(natural_digit v) -> natural &
```

### Description ###

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Add `v` to this number.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

