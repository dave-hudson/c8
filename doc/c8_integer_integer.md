# c8::integer::integer #

```cpp
integer() noexcept
```

Constructs a `c8::integer` with a value of zero.

### Exceptions ###

None.

# c8::integer::integer #

```cpp
integer(long long v) noexcept
```

Constructs a `c8::integer` with a value of `v`.

### Exceptions ###

None.

# c8::integer::integer #

```cpp
integer(const natural &v)
```

Constructs a positive `c8::integer` with a value of `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::integer #

```cpp
integer(const std::string &v)
```

Constructs a `c8::integer` by parsing the string `v`.  `v` may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.  `v` may also be prefaced with a '-' or '+' to indicate a negative or positive value respectively.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::integer::integer #

```cpp
integer(const integer &v)
```

Constructs a `c8::integer` by copying the existing integer `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::integer #

```cpp
integer(integer &&v) noexcept
```

Constructs a `c8::integer` by moving the contents of the existing integer `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

## See Also ##

* [class integer](c8_integer)
* [c8 library](c8)

