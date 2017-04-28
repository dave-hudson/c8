# c8::natural::size\_bits #

Returns the number of bits required to represent this natural number.

## Definition ##

`auto size_bits() const noexcept -> std::size_t`

## Description ##

It is often useful to determine the minimum number of bits required to represent a `natural`, without any rounding to the nearest multiple of a digit.  This function computes and returns that number.

For example, the natural number 1 requires 1 bit, the number 7 requires 3 bits, the number 0x178927fbea79ac2 requires 57 bits, and the number 0xedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210 requires 316.

## Return Value ##

The number of bits required to represent this natural number.

## Exceptions ##

None.
