## c8::natural ##

`c8::natural` is a class designed to represent the natural numbers (zero and all positive whole numbers).  Conceptually they behave in a somewhat similar way to `unsigned int`, but offer unlimited precision.

## Constructors ##

* [natural](c8_natural_natural)

## Destructor ##

* ~natural

## Assignment Operators ##

* [operator =](c8_natural_operator_eq)

## Arithmetic Operators ##

* [operator +](c8_natural_operator_pl)
* [operator -](c8_natural_operator_mi)
* [operator *](c8_natural_operator_mu)
* [operator /](c8_natural_operator_di)
* [operator %](c8_natural_operator_mo)

## Bitwise Operators ##

* [operator &lt;&lt;](c8_natural_operator_ltlt)
* [operator >>](c8_natural_operator_gtgt)

## Compound Assignment Operators ##

* [operator +=](c8_natural_operator_pleq)
* [operator -=](c8_natural_operator_mieq)
* [operator *=](c8_natural_operator_mueq)
* [operator /=](c8_natural_operator_dieq)
* [operator %=](c8_natural_operator_moeq)
* [operator &lt;&lt;=](c8_natural_operator_ltlteq)
* [operator >>=](c8_natural_operator_gtgteq)

## Comparison Operators ##

* [operator ==](c8_natural_operator_eqeq)
* [operator !=](c8_natural_operator_exeq)
* [operator >](c8_natural_operator_gt)
* [operator >=](c8_natural_operator_gteq)
* [operator &lt;](c8_natural_operator_lt)
* [operator &lt;=](c8_natural_operator_lteq)

## Public Member Functions ##

* `auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>`
* `auto divide_modulus(const natural &v) const -> std::pair<natural, natural>`
* `auto gcd(const natural &v) const -> natural`
* [is\_zero](c8_natural_is_zero)
* [size\_bits](c8_natural_size_bits)
* `auto to_unsigned_long_long() const -> unsigned long long`
* `friend auto operator <<(std::ostream &outstr, const natural &v) -> std::ostream &`

TBD

## Examples ##

c8::natural allows us to create two large numbers, subtract one from the other, then output the result to `std::cout`:

```cpp
c8::natural s0("5872489572457574027439274027101850990940275827586671651690897");
c8::natural s1("842758978027689671615847509102945571507457514754190754");

auto s2 = s0 - s1;

std::cout << s2;
```

## See Also ##

* [c8 library](c8)

