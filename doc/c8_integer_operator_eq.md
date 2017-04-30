# c8::integer::operator = #

```cpp
auto operator =(const integer &v) -> integer &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Copy the integer `v`.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::operator = #

```cpp
auto operator =(const integer &&v) noexcept -> integer &
```

Perform operations that assign values to this number (left hand side of the operator).  The previous value of this number is lost and any memory resources that were used by it are released.

Move the integer `v`, leaving `v` as zero.

### Return Value ###

A reference to this number (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

