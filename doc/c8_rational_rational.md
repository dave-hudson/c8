# c8::rational::rational #

```cpp
rational() noexcept
```

Constructs a `c8::rational` with a value of zero.

### Exceptions ###

None.

# c8::rational::rational #

```cpp
rational(long long n, long long d) noexcept
```

Constructs a `c8::rational` with a value of `n/d`.

### Exceptions ###

None.

# c8::rational::rational #

```cpp
rational(const integer &n, const integer &d)
```

Constructs a `c8::rational` with a value of `n/d`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

```cpp
rational(double v) noexcept
```

Constructs a `c8::rational` that represents the floating point value `d`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

```cpp
rational(const std::string &v)
```

Constructs a `c8::rational` by parsing the string `v`.  `v` must have the form n/d, where n and d are both integer values representing the numerator and denominator of the rational, respectively.  Either or both may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.  Either may also be prefaced with a '-' or '+' to indicate a negative or positive value respectively.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::rational::rational #

```cpp
rational(const rational &v)
```

Constructs a `c8::rational` by copying the existing rational `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

```cpp
rational(rational &&v) noexcept
```

Constructs a `c8::rational` by moving the contents of the existing rational `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

## See Also ##

* [class rational](c8_rational)
* [c8 library](c8)

