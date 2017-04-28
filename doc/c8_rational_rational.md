# c8::rational::rational #

Zero value constructor.

### Definition ###

```cpp
rational() noexcept
```

### Description ###

Construct a rational with a value of zero.

### Exceptions ###

None.

# c8::rational::rational #

Integer value constructor.

### Definition ###

```cpp
rational(long long n, long long d) noexcept
```

### Description ###

Construct a rational with a value of `n/d`.

### Exceptions ###

None.

# c8::rational::rational #

c8::natural value constructor.

### Definition ###

```cpp
rational(const integer &n, const integer &d)
```

### Description ###

Construct a rational with a value of `n/d`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

Floating point value constructor.

### Definition ###

```cpp
rational(double v) noexcept
```

### Description ###

Construct a rational that represents the floating point value `d`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

String value constructor.

### Definition ###

```cpp
rational(const std::string &v)
```

### Description ###

Construct a rational by parsing the string `v`.  `v` must have the form n/d, where n and d are both integer values representing the numerator and denominator of the rational, respectively.  Either or both may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.  Either may also be prefaced with a '-' or '+' to indicate a negative or positive value respectively.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.
* `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.

# c8::rational::rational #

Copy constructor.

### Definition ###

```cpp
rational(const rational &v)
```

### Description ###

Construct a rational by copying the existing rational `v`.

### Exceptions ###

* `std::bad_alloc` in the event of a memory allocation failure.

# c8::rational::rational #

Move constructor.

### Definition ###

```cpp
rational(rational &&v) noexcept
```

### Description ###

Construct a rational by moving the contents of the existing rational `v`.  Afterwards `v` will be left as zero.

### Exceptions ###

None.

