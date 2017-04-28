# c8::natural::operator &lt;&lt; #

Left shift operator.

### Definition ###

```cpp
auto operator <<(unsigned int count) const -> natural
```

### Description ###

Shifts this number (left hand side of the operator) left by `count` (right hand side of the operator) bit positions, resulting in a new `c8::natural` number.

### Return Value ###

A new natural number equal to this number shifted left by `count` bit positions.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

