## c8::rational ##

`c8::rational` is a class designed to represent the rational numbers (ratios of two integers).  Rationals are able to express all of the numbers computable by a digital system, and, for example, are able to represent all floating point numbers.

## Constructors ##

* [rational](c8_rational_rational)

Construct a rational number.

## Destructor ##

* ~rational

Destroy (and free all resources allocated to) a rational number.

## Comparison Operators ##

* [operator ==](c8_rational_operator_compare)
* [operator !=](c8_rational_operator_compare)
* [operator >](c8_rational_operator_compare)
* [operator >=](c8_rational_operator_compare)
* [operator &lt;](c8_rational_operator_compare)
* [operator &lt;=](c8_rational_operator_compare)

Compare this rational number with another.

## Arithmetic Operators ##

* [operator +](c8_rational_operator_arithmetic)
* [operator -](c8_rational_operator_arithmetic)
* [operator *](c8_rational_operator_arithmetic)
* [operator /](c8_rational_operator_arithmetic)

Perform arithmetic with this number.

## Assignment Operators ##

* [operator =](c8_rational_operator_assign)
* [operator +=](c8_rational_operator_assign)
* [operator -=](c8_rational_operator_assign)
* [operator *=](c8_rational_operator_assign)
* [operator /=](c8_rational_operator_assign)

Perform assignment operations on this number.

## Public Member Functions ##

* `auto operator -() const -> rational`
* `auto to_double() const -> double`
* `auto is_zero() const noexcept -> bool`
* `auto to_parts() const -> std::pair<integer, integer>`
* `friend auto operator <<(std::ostream &outstr, const rational &v) -> std::ostream &`

TBD
