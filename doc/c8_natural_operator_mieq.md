# c8::natural::operator -= #

```cpp
auto operator -=(const natural &v) -> natural &
auto operator -=(natural_digit v) -> natural &
```

Subtracts `v` (right hand side of the operator) from the value of this object (left hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::not_a_number` if the result of the subtraction would be negative.

