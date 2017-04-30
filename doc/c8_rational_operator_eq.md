# c8::rational::operator = #

```cpp
auto operator =(const rational &v) -> rational &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Copy the rational `v`.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::operator = #

```cpp
auto operator =(const rational &&v) noexcept -> rational &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Move the rational `v`, leaving `v` as zero.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

