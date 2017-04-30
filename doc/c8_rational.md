## c8::rational ##

`c8::rational` is a class designed to represent the rational numbers (ratios of two integers).  Rationals are able to express all of the numbers computable by a digital system, and, for example, are able to represent all floating point numbers.

## Constructors ##

* [rational](c8_rational_rational)

## Destructor ##

* ~rational

## Comparison Operators ##

* [operator ==](c8_rational_operator_eqeq)
* [operator !=](c8_rational_operator_exeq)
* [operator >](c8_rational_operator_gt)
* [operator >=](c8_rational_operator_gteq)
* [operator &lt;](c8_rational_operator_lt)
* [operator &lt;=](c8_rational_operator_lteq)

## Arithmetic Operators ##

* [operator +](c8_rational_operator_pl)
* [operator -](c8_rational_operator_mi)
* [operator *](c8_rational_operator_mu)
* [operator /](c8_rational_operator_di)

## Assignment Operators ##

* [operator =](c8_rational_operator_eq)
* [operator +=](c8_rational_operator_pleq)
* [operator -=](c8_rational_operator_mieq)
* [operator *=](c8_rational_operator_mueq)
* [operator /=](c8_rational_operator_dieq)

## Public Member Functions ##

* `auto operator -() const -> rational`
* `auto to_double() const -> double`
* `auto is_zero() const noexcept -> bool`
* `auto to_parts() const -> std::pair<integer, integer>`
* `friend auto operator <<(std::ostream &outstr, const rational &v) -> std::ostream &`

TBD

