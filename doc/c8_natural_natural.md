# c8::natural::natural #

```cpp
natural() noexcept
```

Constructs a natural number with a value of zero.

### Exceptions ###

None.

# c8::natural::natural #

```cpp
natural(unsigned long long v) noexcept
```

Construct a natural number with a value of `v`.

### Exceptions ###

None.

# c8::natural::natural #

```cpp
natural(const std::string &v)
```

Construct a natural number by parsing the string `v`.  `v` may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::natural::natural #

```cpp
natural(const natural &v)
```

Construct a natural number by copying the existing natural number `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::natural::natural #

```cpp
natural(natural &&v) noexcept
```

Construct a natural number by moving the contents of the existing natural number `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

