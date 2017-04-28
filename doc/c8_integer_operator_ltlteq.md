# c8::integer::operator &lt;&lt;= #

Left shift assignment operator.

### Definition ###

```cpp
auto operator <<=(unsigned int count) -> integer &
```

### Description ###

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Shift this number left by `count` bit positions.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

