# c8::natural::natural #

```cpp
natural() noexcept;
```

Constructs a `c8::natural` with a value of zero.

### Exceptions ###

None.

# c8::natural::natural #

```cpp
natural(unsigned long long v) noexcept;
```

Constructs a `c8::natural` with a value of `v`.

### Exceptions ###

None.

# c8::natural::natural #

```cpp
natural(const std::string &v);
```

Constructs a `c8::natural` by parsing the string `v`.  `v` may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::natural::natural #

```cpp
natural(const natural &v);
```

Constructs a `c8::natural` by copying the existing natural number `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::natural::natural #

```cpp
natural(natural &&v) noexcept;
```

Constructs a `c8::natural` by moving the contents of the existing natural number `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

## See Also ##

* [class natural](c8_natural)
* [c8 library](c8)

