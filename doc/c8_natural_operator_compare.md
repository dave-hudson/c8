# c8::natural::operator ==, !=, >, >=, &lt; &lt;= #

Comparison operators.

## Definitions ##

1. `auto operator ==(const natural &v) const noexcept -> bool`
2. `auto operator !=(const natural &v) const noexcept -> bool`
3. `auto operator >(const natural &v) const noexcept -> bool`
4. `auto operator >=(const natural &v) const noexcept -> bool`
5. `auto operator <(const natural &v) const noexcept -> bool`
6. `auto operator <=(const natural &v) const noexcept -> bool`

## Description ##

Numerically compare the value of this number (left hand side of the operator) and another number `v` (right hand side of the operator).

## Return Values ##

1. Returns `true` if this number is equal to `v`; `false` if it is not equal.
2. Returns `true` if this number is not equal to `v`; `false` if it is equal.
3. Returns `true` if this number is greater than `v`; `false` if it is not.
4. Returns `true` if this number is greater than, or equal to, `v`; `false` if it is not.
5. Returns `true` if this number is less than `v`; `false` if it is not.
6. Returns `true` if this number is less than, or equal to, `v`; `false` if it is not.

## Throws ##

Nothing.
