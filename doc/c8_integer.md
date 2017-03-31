## c8::integer ##

`c8::integer` is a class designed to represent the integers (all positive and negative whole numbers).  Conceptually they behave in a somewhat similar way to `int`, but offer unlimited precision.

## Constructors ##

[integer](c8_integer_integer)

Construct a integer number.

## Destructor ##

~integer

Destroy (and free all resources allocated to) a integer number.

## Comparison Operators ##

[operator ==](c8_integer_operator_compare)
[operator !=](c8_integer_operator_compare)
[operator >](c8_integer_operator_compare)
[operator >=](c8_integer_operator_compare)
[operator &lt;](c8_integer_operator_compare)
[operator &lt;=](c8_integer_operator_compare)

Compare this integer number with another.

## Arithmetic Operators ##

[operator +](c8_integer_operator_arithmetic)
[operator -](c8_integer_operator_arithmetic)
[operator *](c8_integer_operator_arithmetic)
[operator /](c8_integer_operator_arithmetic)
[operator %](c8_integer_operator_arithmetic)

Perform arithmetic with this number.

## Bitwise Operators ##

[operator &lt;&lt;](c8_integer_operator_bitwise)
[operator >>](c8_integer_operator_bitwise)

Perform bitwise operations on this number.

## Assignment Operators ##

[operator =](c8_integer_operator_assign)
[operator +=](c8_integer_operator_assign)
[operator -=](c8_integer_operator_assign)
[operator *=](c8_integer_operator_assign)
[operator /=](c8_integer_operator_assign)
[operator %=](c8_integer_operator_assign)
[operator &lt;&lt;=](c8_integer_operator_assign)
[operator >>=](c8_integer_operator_assign)

Perform assignment operations on this number.

## Public Member Functions ##

`auto operator -() const -> integer`
`auto divide_modulus(const integer &v) const -> std::pair<integer, integer>`
`auto gcd(const integer &v) const -> integer`
`auto to_long_long() const -> long long`
`auto is_zero() const noexcept -> bool`
`auto is_negative() const -> bool`
`auto abs() const -> natural`
`auto negate() -> integer &`
`friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &`

TBD

