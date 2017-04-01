## c8::natural ##

`c8::natural` is a class designed to represent the natural numbers (zero and all positive whole numbers).  Conceptually they behave in a somewhat similar way to `unsigned int`, but offer unlimited precision.

## Constructors ##

* [natural](c8_natural_natural)

Construct a natural number.

## Destructor ##

* ~natural

Destroy (and free all resources allocated to) a natural number.

## Comparison Operators ##

* [operator ==](c8_natural_operator_compare)
* [operator !=](c8_natural_operator_compare)
* [operator >](c8_natural_operator_compare)
* [operator >=](c8_natural_operator_compare)
* [operator &lt;](c8_natural_operator_compare)
* [operator &lt;=](c8_natural_operator_compare)

Compare this natural number with another.

## Arithmetic Operators ##

* [operator +](c8_natural_operator_arithmetic)
* [operator -](c8_natural_operator_arithmetic)
* [operator *](c8_natural_operator_arithmetic)
* [operator /](c8_natural_operator_arithmetic)
* [operator %](c8_natural_operator_arithmetic)

Perform arithmetic with this number.

## Bitwise Operators ##

* [operator &lt;&lt;](c8_natural_operator_bitwise)
* [operator >>](c8_natural_operator_bitwise)

Perform bitwise operations on this number.

## Assignment Operators ##

* [operator =](c8_natural_operator_assign)
* [operator +=](c8_natural_operator_assign)
* [operator -=](c8_natural_operator_assign)
* [operator *=](c8_natural_operator_assign)
* [operator /=](c8_natural_operator_assign)
* [operator %=](c8_natural_operator_assign)
* [operator &lt;&lt;=](c8_natural_operator_assign)
* [operator >>=](c8_natural_operator_assign)

Perform assignment operations on this number.

## Public Member Functions ##

* `auto count_bits() const noexcept -> unsigned int`
* `auto divide_modulus(natural_digit v) const -> std::pair<natural, natural_digit>`
* `auto divide_modulus(const natural &v) const -> std::pair<natural, natural>`
* `auto gcd(const natural &v) const -> natural`
* `auto to_unsigned_long_long() const -> unsigned long long`
* `auto is_zero() const noexcept -> bool`
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

