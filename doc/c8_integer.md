## c8::integer ##

`c8::integer` is a class designed to represent the integers (all positive and negative whole numbers).  Conceptually they behave in a somewhat similar way to `int`, but offer unlimited precision.

## Constructors ##

* [integer](c8_integer_integer)

## Destructor ##

* ~integer

## Assignment Operators ##

* [operator =](c8_integer_operator_eq)

## Unary Operators ##

* [operator -](c8_integer_operator_unary_mi)

## Arithmetic Operators ##

* [operator +](c8_integer_operator_pl)
* [operator -](c8_integer_operator_mi)
* [operator *](c8_integer_operator_mu)
* [operator /](c8_integer_operator_di)
* [operator %](c8_integer_operator_mo)

## Bitwise Operators ##

* [operator &lt;&lt;](c8_integer_operator_ltlt)
* [operator >>](c8_integer_operator_gtgt)

## Compound Assignment Operators ##

* [operator +=](c8_integer_operator_pleq)
* [operator -=](c8_integer_operator_mieq)
* [operator *=](c8_integer_operator_mueq)
* [operator /=](c8_integer_operator_dieq)
* [operator %=](c8_integer_operator_moeq)
* [operator &lt;&lt;=](c8_integer_operator_ltlteq)
* [operator >>=](c8_integer_operator_gtgteq)

## Comparison Operators ##

* [operator ==](c8_integer_operator_eqeq)
* [operator !=](c8_integer_operator_exeq)
* [operator >](c8_integer_operator_gt)
* [operator >=](c8_integer_operator_gteq)
* [operator &lt;](c8_integer_operator_lt)
* [operator &lt;=](c8_integer_operator_lteq)

## Public Member Functions ##

* `auto abs() const -> natural`
* [divide\_modulus](c8_integer_divide_modulus)
* `auto gcd(const integer &v) const -> integer`
* [is\_negative](c8_integer_is_negative)
* [is\_zero](c8_integer_is_zero)
* `auto negate() -> integer &`
* `auto to_long_long() const -> long long`

# Friend Functions ##

* `friend auto operator <<(std::ostream &outstr, const integer &v) -> std::ostream &`

## See Also ##

* [c8 library](c8)

