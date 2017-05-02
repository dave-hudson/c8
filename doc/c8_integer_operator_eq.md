# c8::integer::operator = #

```cpp
auto operator =(const integer &v) -> integer &;
```

Copy the object `v` and assign the copy to this object (left hand side of the operator).

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::operator = #

```cpp
auto operator =(const integer &&v) noexcept -> integer &;
```

Move the contents of object `v` and assign them to this object (left hand side of the operator).  `v` is cleared down to zero.

### Return Value ###

A reference to this object (left hand side of the operator).

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

