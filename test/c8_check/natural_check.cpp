/*
 * natural_check.cpp
 */
#include <c8.h>

#include "result.h"
#include "natural_check.h"

/*
 * Construct with a long integer 0.
 */
auto test_natural_construct_0() -> result {
    result r("nat cons 0");

    r.start_clock();
    c8::natural v(0);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("0");
    return r;
}

/*
 * Construct with a long integer.
 */
auto test_natural_construct_1() -> result {
    result r("nat cons 1");

    r.start_clock();
    c8::natural v(0x123456789abcULL);
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("123456789abc");
    return r;
}

/*
 * Construct with a string 0.
 */
auto test_natural_construct_2() -> result {
    result r("nat cons 2");

    r.start_clock();
    c8::natural v("0");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("0");
    return r;
}

/*
 * Construct with a hexadecimal string.
 */
auto test_natural_construct_3() -> result {
    result r("nat cons 3");

    r.start_clock();
    c8::natural v("0x3837439787487386792386728abcd88379dc");
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("3837439787487386792386728abcd88379dc");
    return r;
}

/*
 * Construct with a decimal string.
 */
auto test_natural_construct_4() -> result {
    result r("nat cons 4");

    r.start_clock();
    c8::natural v("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    return r;
}

/*
 * Construct with an octal string.
 */
auto test_natural_construct_5() -> result {
    result r("nat cons 5");

    r.start_clock();
    c8::natural v("0115415157637671751");
    r.stop_clock();

    r.get_stream() << std::oct << v;
    r.check_pass("115415157637671751");
    return r;
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_natural_construct_6() -> result {
    result r("nat cons 6");

    r.start_clock();
    try {
        c8::natural v6("01185415157637671751");
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::invalid_argument &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Construct with a hexadecimal string.
 */
auto test_natural_construct_7() -> result {
    result r("nat cons 7");

    r.start_clock();
    c8::natural v("0x100000000000000000000000");
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("100000000000000000000000");
    return r;
}

/*
 * Test bit counting.
 */
auto test_natural_count_bits_0() -> result {
    result r("nat count 0");
    c8::natural ct(0);

    r.start_clock();
    auto b = ct.count_bits();
    r.stop_clock();

    r.get_stream() << b;
    r.check_pass("0");
    return r;
}

/*
 * Test bit counting.
 */
auto test_natural_count_bits_1() -> result {
    result r("nat count 1");
    c8::natural ct(0xffffffffffffffffULL);

    r.start_clock();
    auto b = ct.count_bits();
    r.stop_clock();

    r.get_stream() << b;
    r.check_pass("64");
    return r;
}

/*
 * Test bit counting.
 */
auto test_natural_count_bits_2() -> result {
    result r("nat count 2");
    c8::natural ct(0x12345ULL);

    r.start_clock();
    auto b = ct.count_bits();
    r.stop_clock();

    r.get_stream() << b;
    r.check_pass("17");
    return r;
}

/*
 * Test bit counting.
 */
auto test_natural_count_bits_3() -> result {
    result r("nat count 3");
    c8::natural ct("0x123456789abcdef0123456789abcdef0123456789abcdef");

    r.start_clock();
    auto b = ct.count_bits();
    r.stop_clock();

    r.get_stream() << b;
    r.check_pass("185");
    return r;
}

/*
 * Add a 1 digit natural number and another 1 digit natural number.
 */
auto test_natural_add_0a() -> result {
    result r("nat add 0a");
    c8::natural a0("31");
    c8::natural a1("42");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("73");
    return r;
}

/*
 * Add a 1 digit natural number and another 1 digit natural number.
 */
auto test_natural_add_0b() -> result {
    result r("nat add 0b");
    c8::natural a0("31");
    c8::natural a1("42");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("73");
    return r;
}

/*
 * Add a 0 digit natural number and a 1 digit natural number.
 */
auto test_natural_add_1a() -> result {
    result r("nat add 1a");
    c8::natural a0("0");
    c8::natural a1("42");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("42");
    return r;
}

/*
 * Add a 0 digit natural number and a 1 digit natural number.
 */
auto test_natural_add_1b() -> result {
    result r("nat add 1b");
    c8::natural a0("0");
    c8::natural a1("42");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("42");
    return r;
}

/*
 * Add a 2 digit natural number and a 1 digit natural number, resulting in a 3 digit natural number.
 */
auto test_natural_add_2a() -> result {
    result r("nat add 2a");
    c8::natural a0(0xffffffffffffffffULL);
    c8::natural a1(0x2ULL);

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << std::hex << a2;
    r.check_pass("10000000000000001");
    return r;
}

/*
 * Add a 2 digit natural number and a 1 digit natural number, resulting in a 3 digit natural number.
 */
auto test_natural_add_2b() -> result {
    result r("nat add 2b");
    c8::natural a0(0xffffffffffffffffULL);
    c8::natural a1(0x2ULL);

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << std::hex << a0;
    r.check_pass("10000000000000001");
    return r;
}

/*
 * Add two very large values.
 */
auto test_natural_add_3a() -> result {
    result r("nat add 3a");
    c8::natural a0("10000000000000000000000000000000000000000000000000000000000000000008789");
    c8::natural a1("88888880000000000000000000000000000000000000000000000000000000999992000");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("98888880000000000000000000000000000000000000000000000000000001000000789");
    return r;
}

/*
 * Add two very large values.
 */
auto test_natural_add_3b() -> result {
    result r("nat add 3b");
    c8::natural a0("10000000000000000000000000000000000000000000000000000000000000000008789");
    c8::natural a1("88888880000000000000000000000000000000000000000000000000000000999992000");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("98888880000000000000000000000000000000000000000000000000000001000000789");
    return r;
}

/*
 * Add a single digit value and a 1 digit natural number.
 */
auto test_natural_add_4a() -> result {
    result r("nat add 4a");
    c8::natural a0("13");
    c8::natural_digit a1 = 42;

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("55");
    return r;
}

/*
 * Add a single digit value and a 1 digit natural number.
 */
auto test_natural_add_4b() -> result {
    result r("nat add 4b");
    c8::natural a0("13");
    c8::natural_digit a1 = 42;

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("55");
    return r;
}

/*
 * Add a 3 digit natural number and a 1 digit value, resulting in a 4 digit natural number.
 */
auto test_natural_add_5a() -> result {
    result r("nat add 5a");
    c8::natural a0("0xffffffffffffffffffffffff");
    c8::natural_digit a1 = 2;

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << std::hex << a2;
    r.check_pass("1000000000000000000000001");
    return r;
}

/*
 * Add a 3 digit natural number and a 1 digit value, resulting in a 4 digit natural number.
 */
auto test_natural_add_5b() -> result {
    result r("nat add 5b");
    c8::natural a0("0xffffffffffffffffffffffff");
    c8::natural_digit a1 = 2;

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << std::hex << a0;
    r.check_pass("1000000000000000000000001");
    return r;
}

/*
 * Add a 1 digit natural number and a 2 digit natural number, resulting in a 3 digit natural number.
 */
auto test_natural_add_6a() -> result {
    result r("nat add 6a");
    c8::natural a0(0x9ULL);
    c8::natural a1(0xffffffffffffffffULL);

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << std::hex << a2;
    r.check_pass("10000000000000008");
    return r;
}

/*
 * Add a 1 digit natural number and a 2 digit natural number, resulting in a 3 digit natural number.
 */
auto test_natural_add_6b() -> result {
    result r("nat add 6b");
    c8::natural a0(0x9ULL);
    c8::natural a1(0xffffffffffffffffULL);

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << std::hex << a0;
    r.check_pass("10000000000000008");
    return r;
}

/*
 * Subtract a 1 digit natural number from another 1 digit natural number.
 */
auto test_natural_subtract_0a() -> result {
    result r("nat sub 0a");
    c8::natural s0(52);
    c8::natural s1(2);

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("50");
    return r;
}

/*
 * Subtract a 1 digit natural number from another 1 digit natural number.
 */
auto test_natural_subtract_0b() -> result {
    result r("nat sub 0b");
    c8::natural s0(52);
    c8::natural s1(2);

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("50");
    return r;
}

/*
 * Subtract a large natural number from another large natural number.
 */
auto test_natural_subtract_1a() -> result {
    result r("nat sub 1a");
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("842758978027689671615847509157087514875097509475029454785478748571507457514754190754");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143");
    return r;
}

/*
 * Subtract a large natural number from another large natural number.
 */
auto test_natural_subtract_1b() -> result {
    result r("nat sub 1b");
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("842758978027689671615847509157087514875097509475029454785478748571507457514754190754");

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143");
    return r;
}

/*
 * Subtract a large natural number from another large natural number, resulting in a very much smaller one.
 */
auto test_natural_subtract_2a() -> result {
    result r("nat sub 2a");
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690000");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("897");
    return r;
}

/*
 * Subtract a large natural number from another large natural number, resulting in a very much smaller one.
 */
auto test_natural_subtract_2b() -> result {
    result r("nat sub 2b");
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690000");

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("897");
    return r;
}

/*
 * Subtract a large natural number from a smaller one.  This will throw an exception because there
 * aren't any negative natural numbers.
 */
auto test_natural_subtract_3a() -> result {
    result r("nat sub 3a");
    c8::natural s0(2);
    c8::natural s1(52);

    r.start_clock();
    try {
        auto s2 = s0 - s1;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::not_a_number &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Subtract a large natural number from a smaller one.  This will throw an exception because there
 * aren't any negative natural numbers.
 */
auto test_natural_subtract_3b() -> result {
    result r("nat sub 3b");
    c8::natural s0(2);
    c8::natural s1(52);

    r.start_clock();
    try {
        s0 -= s1;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::not_a_number &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Subtract a single digit from a 1 digit natural number.
 */
auto test_natural_subtract_4a() -> result {
    result r("nat sub 4a");
    c8::natural s0(53);

    r.start_clock();
    auto s2 = s0 - 15;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("38");
    return r;
}

/*
 * Subtract a single digit from a 1 digit natural number.
 */
auto test_natural_subtract_4b() -> result {
    result r("nat sub 4b");
    c8::natural s0(53);

    r.start_clock();
    s0 -= 15;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("38");
    return r;
}

/*
 * Subtract a larger single digit from a smaller natural number.  This will throw an exception
 * because there aren't any negative natural numbers.
 */
auto test_natural_subtract_5a() -> result {
    result r("nat sub 5a");
    c8::natural s0(100);

    r.start_clock();
    try {
        auto s2 = s0 - 127;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::not_a_number &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Subtract a larger single digit from a smaller natural number.  This will throw an exception
 * because there aren't any negative natural numbers.
 */
auto test_natural_subtract_5b() -> result {
    result r("nat sub 5b");
    c8::natural s0(100);

    r.start_clock();
    try {
        s0 -= 127;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::not_a_number &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Subtract a single digit zero from a zero natural number.
 */
auto test_natural_subtract_6a() -> result {
    result r("nat sub 6a");
    c8::natural s0(0);

    r.start_clock();
    auto s2 = s0 - 0;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("0");
    return r;
}

/*
 * Subtract a single digit zero from a zero natural number.
 */
auto test_natural_subtract_6b() -> result {
    result r("nat sub 6b");
    c8::natural s0(0);

    r.start_clock();
    s0 -= 0;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("0");
    return r;
}

/*
 * Subtract a single digit from a larger natural number.
 */
auto test_natural_subtract_7a() -> result {
    result r("nat sub 7a");
    c8::natural s0("0x1000000000000000000000000");

    r.start_clock();
    auto s2 = s0 - 1;
    r.stop_clock();

    r.get_stream() << std::hex << s2;
    r.check_pass("ffffffffffffffffffffffff");
    return r;
}

/*
 * Subtract a single digit from a larger natural number.
 */
auto test_natural_subtract_7b() -> result {
    result r("nat sub 7b");
    c8::natural s0("0x1000000000000000000000000");

    r.start_clock();
    s0 -= 1;
    r.stop_clock();

    r.get_stream() << std::hex << s0;
    r.check_pass("ffffffffffffffffffffffff");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0a() -> result {
    result r("nat comp 0a");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0b() -> result {
    result r("nat comp 0b");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0c() -> result {
    result r("nat comp 0c");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0d() -> result {
    result r("nat comp 0d");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0e() -> result {
    result r("nat comp 0e");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_natural_compare_0f() -> result {
    result r("nat comp 0f");
    c8::natural co0(2);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1a() -> result {
    result r("nat comp 1a");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1b() -> result {
    result r("nat comp 1b");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1c() -> result {
    result r("nat comp 1c");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1d() -> result {
    result r("nat comp 1d");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1e() -> result {
    result r("nat comp 1e");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_natural_compare_1f() -> result {
    result r("nat comp 1f");
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2a() -> result {
    result r("nat comp 2a");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2b() -> result {
    result r("nat comp 2b");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2c() -> result {
    result r("nat comp 2c");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2d() -> result {
    result r("nat comp 2d");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2e() -> result {
    result r("nat comp 2e");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_natural_compare_2f() -> result {
    result r("nat comp 2f");
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3a() -> result {
    result r("nat comp 3a");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3b() -> result {
    result r("nat comp 3b");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3c() -> result {
    result r("nat comp 3c");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3d() -> result {
    result r("nat comp 3d");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3e() -> result {
    result r("nat comp 3e");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two 2 digit values.
 */
auto test_natural_compare_3f() -> result {
    result r("nat comp 3f");
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_0a() -> result {
    result r("nat lsh 0a");
    c8::natural l0(0x349f);

    r.start_clock();
    auto l1 = l0 << 0;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("349f");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_0b() -> result {
    result r("nat lsh 0b");
    c8::natural l0(0x349f);

    r.start_clock();
    l0 <<= 0;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("349f");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_1a() -> result {
    result r("nat lsh 1a");
    c8::natural l0(0x349f);

    r.start_clock();
    auto l1 = l0 << 1;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("693e");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_1b() -> result {
    result r("nat lsh 1b");
    c8::natural l0(0x349f);

    r.start_clock();
    l0 <<= 1;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("693e");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_2a() -> result {
    result r("nat lsh 2a");
    c8::natural l0(0x349f);

    r.start_clock();
    auto l1 = l0 << 18;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("d27c0000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_2b() -> result {
    result r("nat lsh 2b");
    c8::natural l0(0x349f);

    r.start_clock();
    l0 <<= 18;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("d27c0000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_3a() -> result {
    result r("nat lsh 3a");
    c8::natural l0(0x349f);

    r.start_clock();
    auto l1 = l0 << 187;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("1a4f80000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_3b() -> result {
    result r("nat lsh 3b");
    c8::natural l0(0x349f);

    r.start_clock();
    l0 <<= 187;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("1a4f80000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_4a() -> result {
    result r("nat lsh 4a");
    c8::natural l0("0x349f298375323985afbce9837928798789dffeffee987678687678676756562");

    r.start_clock();
    auto l1 = l0 << 69;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_4b() -> result {
    result r("nat lsh 4b");
    c8::natural l0("0x349f298375323985afbce9837928798789dffeffee987678687678676756562");

    r.start_clock();
    l0 <<= 69;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_5a() -> result {
    result r("nat lsh 5a");
    c8::natural l0("0x349f2983753239856562");

    r.start_clock();
    auto l1 = l0 << 64;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("349f29837532398565620000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_5b() -> result {
    result r("nat lsh 5b");
    c8::natural l0("0x349f2983753239856562");

    r.start_clock();
    l0 <<= 64;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("349f29837532398565620000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_6a() -> result {
    result r("nat lsh 6a");
    c8::natural l0(0x349f2);

    r.start_clock();
    auto l1 = l0 << 64;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("349f20000000000000000");
    return r;
}

/*
 * Test left shifting.
 */
auto test_natural_lshift_6b() -> result {
    result r("nat lsh 6b");
    c8::natural l0(0x349f2);

    r.start_clock();
    l0 <<= 64;
    r.stop_clock();

    r.get_stream() << std::hex << l0;
    r.check_pass("349f20000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_0a() -> result {
    result r("nat rsh 0a");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    auto r1 = r0 >> 0;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("23490000000000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_0b() -> result {
    result r("nat rsh 0b");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    r0 >>= 0;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("23490000000000000000000000000000000000000000000000000000");
    return r;
}

/* * Test right shifting.
 */
auto test_natural_rshift_1a() -> result {
    result r("nat rsh 1a");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    auto r1 = r0 >> 1;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("11a48000000000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_1b() -> result {
    result r("nat rsh 1b");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    r0 >>= 1;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("11a48000000000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_2a() -> result {
    result r("nat rsh 2a");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    auto r1 = r0 >> 19;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("469200000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_2b() -> result {
    result r("nat rsh 2b");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    r0 >>= 19;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("469200000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_3a() -> result {
    result r("nat rsh 3a");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    auto r1 = r0 >> 197;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("11a4800");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_3b() -> result {
    result r("nat rsh 3b");
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");

    r.start_clock();
    r0 >>= 197;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("11a4800");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_4a() -> result {
    result r("nat rsh 4a");
    c8::natural r0("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");

    r.start_clock();
    auto r1 = r0 >> 123;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_4b() -> result {
    result r("nat rsh 4b");
    c8::natural r0("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");

    r.start_clock();
    r0 >>= 123;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_5a() -> result {
    result r("nat rsh 5a");
    c8::natural r0("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");

    r.start_clock();
    auto r1 = r0 >> 1234;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("0");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_5b() -> result {
    result r("nat rsh 5b");
    c8::natural r0("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");

    r.start_clock();
    r0 >>= 1234;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("0");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_6a() -> result {
    result r("nat rsh 6a");
    c8::natural r0(0x693e);

    r.start_clock();
    auto r1 = r0 >> 16;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("0");
    return r;
}

/*
 * Test right shifting.
 */
auto test_natural_rshift_6b() -> result {
    result r("nat rsh 6b");
    c8::natural r0(0x693e);

    r.start_clock();
    r0 >>= 16;
    r.stop_clock();

    r.get_stream() << std::hex << r0;
    r.check_pass("0");
    return r;
}

/*
 * Multiply a 1 digit natural number by another 1 digit natural number.
 */
auto test_natural_multiply_0a() -> result {
    result r("nat mul 0a");
    c8::natural mu0(3);
    c8::natural mu1(22);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("66");
    return r;
}

/*
 * Multiply a 1 digit natural number by another 1 digit natural number.
 */
auto test_natural_multiply_0b() -> result {
    result r("nat mul 0b");
    c8::natural mu0(3);
    c8::natural mu1(22);

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("66");
    return r;
}

/*
 * Multiply a multi-digit natural number by another multi-digit natural number.
 */
auto test_natural_multiply_1a() -> result {
    result r("nat mul 1a");
    c8::natural mu0(1000000000000000000ULL);
    c8::natural mu1(9999999999999999999ULL);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("9999999999999999999000000000000000000");
    return r;
}

/*
 * Multiply a multi-digit natural number by another multi-digit natural number.
 */
auto test_natural_multiply_1b() -> result {
    result r("nat mul 1b");
    c8::natural mu0(1000000000000000000ULL);
    c8::natural mu1(9999999999999999999ULL);

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("9999999999999999999000000000000000000");
    return r;
}

/*
 * Multiply a multi-digit natural number by another multi-digit natural number.
 */
auto test_natural_multiply_2a() -> result {
    result r("nat mul 2a");
    c8::natural mu0(0x2000000000000000ULL);
    c8::natural mu1(0x4000000000000000ULL);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("8000000000000000000000000000000");
    return r;
}

/*
 * Multiply a multi-digit natural number by another multi-digit natural number.
 */
auto test_natural_multiply_2b() -> result {
    result r("nat mul 2b");
    c8::natural mu0(0x2000000000000000ULL);
    c8::natural mu1(0x4000000000000000ULL);

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu0;
    r.check_pass("8000000000000000000000000000000");
    return r;
}

/*
 * Multiply a large natural number by another large natural number.
 */
auto test_natural_multiply_3a() -> result {
    result r("nat mul 3a");
    c8::natural mu0("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::natural mu1("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100");
    return r;
}

/*
 * Multiply a large natural number by another large natural number.
 */
auto test_natural_multiply_3b() -> result {
    result r("nat mul 3b");
    c8::natural mu0("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::natural mu1("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100");
    return r;
}

/*
 * Multiply a large natural number by 1 digit natural number.
 */
auto test_natural_multiply_4a() -> result {
    result r("nat mul 4a");
    c8::natural mu0("0x1000000000000000100000000000000100000000");
    c8::natural mu1("0xabcdef12");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Multiply a large natural number by 1 digit natural number.
 */
auto test_natural_multiply_4b() -> result {
    result r("nat mul 4b");
    c8::natural mu0("0x1000000000000000100000000000000100000000");
    c8::natural mu1("0xabcdef12");

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu0;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Multiply a 1 digit natural number by a large natural number.
 */
auto test_natural_multiply_5a() -> result {
    result r("nat mul 5a");
    c8::natural mu0("0xabcdef12");
    c8::natural mu1("0x1000000000000000100000000000000100000000");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Multiply a 1 digit natural number by a large natural number.
 */
auto test_natural_multiply_5b() -> result {
    result r("nat mul 5b");
    c8::natural mu0("0xabcdef12");
    c8::natural mu1("0x1000000000000000100000000000000100000000");

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu0;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Multiply a large natural number by a single digit.
 */
auto test_natural_multiply_6a() -> result {
    result r("nat mul 6a");
    c8::natural mu0("0x1000000000000000100000000000000100000000");

    r.start_clock();
    auto mu2 = mu0 * 0x0abcdef12;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Multiply a large natural number by a single digit.
 */
auto test_natural_multiply_6b() -> result {
    result r("nat mul 6b");
    c8::natural mu0("0x1000000000000000100000000000000100000000");

    r.start_clock();
    mu0 *= 0x0abcdef12;
    r.stop_clock();

    r.get_stream() << std::hex << mu0;
    r.check_pass("abcdef1200000000abcdef120000000abcdef1200000000");
    return r;
}

/*
 * Divide a 2 digit natural number by another 2 digit natural number.
 */
auto test_natural_divide_0a() -> result {
    result r("nat div 0a");
    c8::natural d0(1000000000000000000ULL);
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("10,10");
    return r;
}

/*
 * Divide a 2 digit natural number by another 2 digit natural number.
 */
auto test_natural_divide_0b() -> result {
    result r("nat div 0b");
    c8::natural d0(1000000000000000000ULL);
    auto mo0 = d0;
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << d0 << ',' << mo0;
    r.check_pass("10,10");
    return r;
}

/*
 * Divide a 2 digit natural number by another 2 digit natural number.
 */
auto test_natural_divide_0c() -> result {
    result r("nat div 0c");
    c8::natural d0(1000000000000000000ULL);
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << dm.first << ',' << dm.second;
    r.check_pass("10,10");
    return r;
}

/*
 * Divide a large natural number by another 2 digit natural number.
 */
auto test_natural_divide_1a() -> result {
    result r("nat div 1a");
    c8::natural d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("78292387927518758972102054472775487212767983201652300846,35600667362958008");
    return r;
}

/*
 * Divide a large natural number by another 2 digit natural number.
 */
auto test_natural_divide_1b() -> result {
    result r("nat div 1b");
    c8::natural d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    auto mo0 = d0;
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << d0 << ',' << mo0;
    r.check_pass("78292387927518758972102054472775487212767983201652300846,35600667362958008");
    return r;
}

/*
 * Divide a large natural number by another 2 digit natural number.
 */
auto test_natural_divide_1c() -> result {
    result r("nat div 1c");
    c8::natural d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::natural d1(99999999999999999ULL);

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << dm.first << ',' << dm.second;
    r.check_pass("78292387927518758972102054472775487212767983201652300846,35600667362958008");
    return r;
}

/*
 * Divide a large natural number by another large natural number.
 */
auto test_natural_divide_2a() -> result {
    result r("nat div 2a");
    c8::natural d0("0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000001000000000000000100000000");

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("ffffffffffffffff000000000000000,100000000000000000000000");
    return r;
}

/*
 * Divide a large natural number by another large natural number.
 */
auto test_natural_divide_2b() -> result {
    result r("nat div 2b");
    c8::natural d0("0x100000000000000000000000000000000000000000000000000000000000000000000000");
    auto mo0 = d0;
    c8::natural d1("0x10000000000000001000000000000000100000000");

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << std::hex << d0 << ',' << mo0;
    r.check_pass("ffffffffffffffff000000000000000,100000000000000000000000");
    return r;
}

/*
 * Divide a large natural number by another large natural number.
 */
auto test_natural_divide_2c() -> result {
    result r("nat div 2c");
    c8::natural d0("0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000001000000000000000100000000");

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << std::hex << dm.first << ',' << dm.second;
    r.check_pass("ffffffffffffffff000000000000000,100000000000000000000000");
    return r;
}

/*
 * Divide by zero.  This will throw an exception!
 */
auto test_natural_divide_3a() -> result {
    result r("nat div 3a");
    c8::natural d0(2000);
    c8::natural d1(0);

    r.start_clock();
    try {
        auto d2 = d0 / d1;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::divide_by_zero &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Divide by zero.  This will throw an exception!
 */
auto test_natural_divide_3b() -> result {
    result r("nat div 3b");
    c8::natural d0(2000);
    c8::natural d1(0);

    r.start_clock();
    try {
        d0 /= d1;
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::divide_by_zero &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Divide by zero.  This will throw an exception!
 */
auto test_natural_divide_3c() -> result {
    result r("nat div 3c");
    c8::natural d0(2000);
    c8::natural d1(0);

    r.start_clock();
    try {
        auto dm = d0.divide_modulus(d1);
        r.stop_clock();

        r.get_stream() << "failed to throw exception";
        r.set_pass(false);
    } catch (const c8::divide_by_zero &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Divide a 2 digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_4a() -> result {
    result r("nat div 4a");
    c8::natural d0("0x10000000000000");
    c8::natural d1("0x10000000");

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("1000000,0");
    return r;
}

/*
 * Divide a 2 digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_4b() -> result {
    result r("nat div 4b");
    c8::natural d0("0x10000000000000");
    auto mo0 = d0;
    c8::natural d1("0x10000000");

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << std::hex << d0 << ',' << mo0;
    r.check_pass("1000000,0");
    return r;
}

/*
 * Divide a 2 digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_4c() -> result {
    result r("nat div 4c");
    c8::natural d0("0x10000000000000");
    c8::natural d1("0x10000000");

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << std::hex << dm.first << ',' << dm.second;
    r.check_pass("1000000,0");
    return r;
}

/*
 * Divide a large digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_5a() -> result {
    result r("nat div 5a");
    c8::natural d0("0x10000000000000001000000000000000100000000");
    c8::natural d1("1");

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("10000000000000001000000000000000100000000,0");
    return r;
}

/*
 * Divide a large digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_5b() -> result {
    result r("nat div 5b");
    c8::natural d0("0x10000000000000001000000000000000100000000");
    auto mo0 = d0;
    c8::natural d1("1");

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << std::hex << d0 << ',' << mo0;
    r.check_pass("10000000000000001000000000000000100000000,0");
    return r;
}

/*
 * Divide a large digit natural number by a 1 digit natural number.
 */
auto test_natural_divide_5c() -> result {
    result r("nat div 5c");
    c8::natural d0("0x10000000000000001000000000000000100000000");
    c8::natural d1("1");

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << std::hex << dm.first << ',' << dm.second;
    r.check_pass("10000000000000001000000000000000100000000,0");
    return r;
}

/*
 * Trigger a divide that exercises a corner case in the divide estimation logic.
 */
auto test_natural_divide_6a() -> result {
    result r("nat div 6a");
    c8::natural d0("0x1000000000000000000000002000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000000000000010000000000000000");

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("1000000000000000000000000ffffffff,ffffffffffffffff000000010000000000000000");
    return r;
}

/*
 * Trigger a divide that exercises a corner case in the divide estimation logic.
 */
auto test_natural_divide_6b() -> result {
    result r("nat div 6b");
    c8::natural d0("0x1000000000000000000000002000000000000000000000000000000000000000000000000");
    auto mo0 = d0;
    c8::natural d1("0x10000000000000000000000010000000000000000");

    r.start_clock();
    d0 /= d1;
    mo0 %= d1;
    r.stop_clock();

    r.get_stream() << std::hex << d0 << ',' << mo0;
    r.check_pass("1000000000000000000000000ffffffff,ffffffffffffffff000000010000000000000000");
    return r;
}

/*
 * Trigger a divide that exercises a corner case in the divide estimation logic.
 */
auto test_natural_divide_6c() -> result {
    result r("nat div 6c");
    c8::natural d0("0x1000000000000000000000002000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000000000000010000000000000000");

    r.start_clock();
    auto dm = d0.divide_modulus(d1);
    r.stop_clock();

    r.get_stream() << std::hex << dm.first << ',' << dm.second;
    r.check_pass("1000000000000000000000000ffffffff,ffffffffffffffff000000010000000000000000");
    return r;
}

/*
 * Divide a 3 digit natural number by a single digit value.
 */
auto test_natural_divide_7a() -> result {
    result r("nat div 7a");
    c8::natural d0("0x100000000000000000");

    r.start_clock();
    auto d2 = d0 / 0x10000000;
    auto mo2 = d0 % 0x10000000;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("10000000000,0");
    return r;
}

/*
 * Divide a 3 digit natural number by a single digit value.
 */
auto test_natural_divide_7b() -> result {
    result r("nat div 7b");
    c8::natural d0("0x100000000000000000");
    auto mo0 = d0;

    r.start_clock();
    d0 /= 0x10000000;
    mo0 %= 0x10000000;
    r.stop_clock();

    r.get_stream() << std::hex << d0 << ',' << mo0;
    r.check_pass("10000000000,0");
    return r;
}

/*
 * Divide a 3 digit natural number by a single digit value.
 */
auto test_natural_divide_7c() -> result {
    result r("nat div 7c");
    c8::natural d0("0x100000000000000000");

    r.start_clock();
    auto dm = d0.divide_modulus(0x10000000);
    r.stop_clock();

    r.get_stream() << std::hex << dm.first << ',' << dm.second;
    r.check_pass("10000000000,0");
    return r;
}

/*
 * Divide a 1 digit natural number by a single digit value.
 */
auto test_natural_divide_8a() -> result {
    result r("nat div 8a");
    c8::natural d0("894597578");

    r.start_clock();
    auto d2 = d0 / 33;
    auto mo2 = d0 % 33;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("27109017,17");
    return r;
}

/*
 * Divide a 1 digit natural number by a single digit value.
 */
auto test_natural_divide_8b() -> result {
    result r("nat div 8b");
    c8::natural d0("894597578");
    auto mo0 = d0;

    r.start_clock();
    d0 /= 33;
    mo0 %= 33;
    r.stop_clock();

    r.get_stream() << d0 << ',' << mo0;
    r.check_pass("27109017,17");
    return r;
}

/*
 * Divide a 1 digit natural number by a single digit value.
 */
auto test_natural_divide_8c() -> result {
    result r("nat div 8c");
    c8::natural d0("894597578");

    r.start_clock();
    auto dm = d0.divide_modulus(33);
    r.stop_clock();

    r.get_stream() << dm.first << ',' << dm.second;
    r.check_pass("27109017,17");
    return r;
}

/*
 * Test greatest common divisor.
 */
auto test_natural_gcd_0() -> result {
    result r("nat gcd 0");
    c8::natural g0(2000);
    c8::natural g1(56);

    r.start_clock();
    c8::natural g2 = gcd(g0, g1);
    r.stop_clock();

    r.get_stream() << g2;
    r.check_pass("8");
    return r;
}

/*
 * Test greatest common divisor.
 */
auto test_natural_gcd_1() -> result {
    result r("nat gcd 1");
    c8::natural g0("47598475892456783750932574388878478947978888888");
    c8::natural g1("87987922283");

    r.start_clock();
    c8::natural g2 = gcd(g0, g1);
    r.stop_clock();

    r.get_stream() << g2;
    r.check_pass("1");
    return r;
}

/*
 * Test greatest common divisor.
 */
auto test_natural_gcd_2() -> result {
    result r("nat gcd 2");
    c8::natural g0("8888888");
    c8::natural g1("8888888");

    r.start_clock();
    c8::natural g2 = gcd(g0, g1);
    r.stop_clock();

    r.get_stream() << g2;
    r.check_pass("8888888");
    return r;
}

/*
 * Test greatest common divisor.
 */
auto test_natural_gcd_3() -> result {
    result r("nat gcd 3");
    c8::natural g0("2038355020176327696765561949673186971898109715960816150233379221718753632190267");
    c8::natural g1("1957628088684195906794648605131674616575412301467318480917205787195238636855999");

    r.start_clock();
    c8::natural g2 = gcd(g0, g1);
    r.stop_clock();

    r.get_stream() << g2;
    r.check_pass("20181732873032947492728336135378088830674353623374417329043358630878748833567");
    return r;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_natural_to_unsigned_long_long_0() -> result {
    result r("nat toull 0");
    c8::natural n(0);

    r.start_clock();
    unsigned long long u = to_unsigned_long_long(n);
    r.stop_clock();

    r.get_stream() << u;
    r.check_pass("0");
    return r;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_natural_to_unsigned_long_long_1() -> result {
    result r("nat toull 1");
    c8::natural n(2000);

    r.start_clock();
    unsigned long long u = to_unsigned_long_long(n);
    r.stop_clock();

    r.get_stream() << u;
    r.check_pass("2000");
    return r;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_natural_to_unsigned_long_long_2() -> result {
    result r("nat toull 2");
    c8::natural n("47895748574857485728747548237543205782573485472759047548275024574207");

    r.start_clock();
    try {
        unsigned long long u = to_unsigned_long_long(n);
        r.stop_clock();

        r.get_stream() << u << " (failed to throw exception)";
        r.set_pass(false);
    } catch (const c8::overflow_error &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_natural_to_unsigned_long_long_3() -> result {
    result r("nat toull 3");
    c8::natural n(0x123456789a);

    r.start_clock();
    unsigned long long u = to_unsigned_long_long(n);
    r.stop_clock();

    r.get_stream() << std::hex << u;
    r.check_pass("123456789a");
    return r;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_natural_to_unsigned_long_long_4() -> result {
    result r("nat toull 4");
    c8::natural n0(1);
    c8::natural n1 = n0 << (sizeof(unsigned long long) * 8);

    /*
     * Construct a natural number that is one bit too large to be able to convert.
     */
    r.start_clock();
    try {
        unsigned long long u = to_unsigned_long_long(n1);
        r.stop_clock();

        r.get_stream() << u << " (failed to throw exception)";
        r.set_pass(false);
    } catch (const c8::overflow_error &e) {
        r.stop_clock();

        r.get_stream() << "exception thrown: " + std::string(e.what());
        r.set_pass(true);
    } catch (...) {
        r.stop_clock();

        r.get_stream() << "unexpected exception thrown";
        r.set_pass(false);
    }

    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_0() -> result {
    result r("nat prn 0");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << v;
    r.stop_clock();

    r.check_pass("4701397401952099592073");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_1() -> result {
    result r("nat prn 1");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::hex << v;
    r.stop_clock();

    r.check_pass("fedcfedc0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_2() -> result {
    result r("nat prn 2");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("FEDCFEDC0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_3() -> result {
    result r("nat prn 3");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::oct << v;
    r.stop_clock();

    r.check_pass("775563766700044321263611");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_4() -> result {
    result r("nat prn 4");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << v;
    r.stop_clock();

    r.check_pass("4701397401952099592073");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_5() -> result {
    result r("nat prn 5");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::hex << v;
    r.stop_clock();

    r.check_pass("0xfedcfedc0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_6() -> result {
    result r("nat prn 6");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("0XFEDCFEDC0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_natural_print_7() -> result {
    result r("nat prn 7");
    c8::natural v("0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::oct << v;
    r.stop_clock();

    r.check_pass("0775563766700044321263611");
    return r;
}

