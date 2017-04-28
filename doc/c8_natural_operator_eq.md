# c8::natural::operator = #

Copy assignment operator.

### Definition ###

```cpp
auto operator =(const natural &v) -> natural &
```

### Description ###

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Copy the natural number `v`.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::natural::operator = #

Move assignment operator.

### Definition ###

```cpp
auto operator =(const natural &&v) noexcept -> natural &
```

### Description ###

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Move the natural number `v`, leaving `v` as zero.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

