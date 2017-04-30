## c8::integer ##

`c8::integer` is a class designed to represent the integers (all positive and negative whole numbers).  Conceptually they behave in a somewhat similar way to `int`, but offer unlimited precision.

## Constructors ##

* [integer](c8_integer_integer)

## Destructor ##

* ~integer

## Comparison Operators ##

* [operator ==](c8_integer_operator_eqeq)
* [operator !=](c8_integer_operator_exeq)
* [operator >](c8_integer_operator_gt)
* [operator >=](c8_integer_operator_gteq)
* [operator &lt;](c8_integer_operator_lt)
* [operator &lt;=](c8_integer_operator_lteq)

## Arithmetic Operators ##

* [operator +](c8_integer_operator_pl)
* [operator -](c8_integer_operator_mi)
* [operator *](c8_integer_operator_mu)
* [operator /](c8_integer_operator_di)
* [operator %](c8_integer_operator_mo)

## Bitwise Operators ##

* [operator &lt;&lt;](c8_integer_operator_ltlt)
* [operator >>](c8_integer_operator_gtgt)

## Assignment Operators ##

* [operator =](c8_integer_operator_eq)
* [operator +=](c8_integer_operator_pleq)
* [operator -=](c8_integer_operator_mieq)
* [operator *=](c8_integer_operator_mueq)
* [operator /=](c8_integer_operator_dieq)
* [operator %=](c8_integer_operator_moeq)
* [operator &lt;&lt;=](c8_integer_operator_ltlteq)
* [operator >>=](c8_integer_operator_gtgteq)

## Public Member Functions ##

* `auto operator -() const -> integer`
* `auto divide_modulus(const integer &v) const -> std::pair<integer, integer>`
* `auto gcd(const integer &v) const -> integer`
* `auto to_long_long() const -> long long`
* `auto is_zero() const noexcept -> bool`
* `auto is_negative() const -> bool`
* `auto abs() const -> natural`
* `auto negate() -> integer &`
* `friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &`

TBD

