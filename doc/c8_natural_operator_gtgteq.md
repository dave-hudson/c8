# c8::natural::operator >>= #

```cpp
auto operator >>=(unsigned int count) -> natural &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Shift this number right by `count` bit positions.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

