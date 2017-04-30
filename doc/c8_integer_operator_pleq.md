# c8::integer::operator += #

```cpp
auto operator +=(const integer &v) -> integer &
```

Adds `v` (right hand side of the operator) to the value of this object (left hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

