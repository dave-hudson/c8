# c8::integer::integer #

Zero value constructor.

### Definition ###

```cpp
integer() noexcept
```

### Description ###

Construct an integer with a value of zero.

### Exceptions ###

None.

# c8::integer::integer #

Integer value constructor.

### Definition ###

```cpp
integer(long long v) noexcept
```

### Description ###

Construct an integer with a value of `v`.

### Exceptions ###

None.

# c8::integer::integer #

c8::natural value constructor.

### Definition ###

```cpp
integer(const natural &v)
```

### Description ###

Construct a positive integer with a value of `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::integer #

String value constructor.

### Definition ###

```cpp
integer(const std::string &v)
```

### Description ###

Construct an integer by parsing the string `v`.  `v` may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.  `v` may also be prefaced with a '-' or '+' to indicate a negative or positive value respectively.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::integer::integer #

Copy constructor.

### Definition ###

```cpp
integer(const integer &v)
```

### Description ###

Construct an integer by copying the existing integer `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::integer::integer #

Move constructor.

### Definition ###

```cpp
integer(integer &&v) noexcept
```

### Description ###

Construct an integer by moving the contents of the existing integer `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

