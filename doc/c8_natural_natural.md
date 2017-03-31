# c8::natural::natural #

Constructors.

## Definitions ##

1. `natural() noexcept`
2. `natural(unsigned long long v) noexcept`
3. `natural(const std::string &v)`
4. `natural(const natural &v)`
5. `natural(natural &&v) noexcept`

## Descriptions ##

1. Construct a natural number with a value of zero.
2. Construct a natural number with a value of `v`.
3. Construct a natural number by parsing the string `v`.  `v` may have a prefix of "0" for an octal value, or "0x" for a hexadecimal value.
4. Construct a natural number by copying the existing natural number `v`.
5. Construct a natural number by moving the contents of the existing natural number `v`.  Afterwards `v` will be left as zero.

## Exceptions ##

1. Nothing
2. Nothing
3. `std::bad_alloc` in the event of a memory allocation failure.  `c8::invalid_argument` in the event of an empty, or incorrectly formed, string `v`.
4. `std::bad_alloc` in the event of a memory allocation failure.
5. Nothing

