/*
 * rational_check.cpp
 */
#include <c8.h>

#include "result.h"

/*
 * Test Constructors.
 */
auto test_rational_construct_0() -> result {
    result r("rat cons 0");

    r.start_clock();
    c8::rational v(0, 1);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("0/1");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_1() -> result {
    result r("rat cons 1");

    r.start_clock();
    c8::rational v(1024, 384);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("8/3");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_2() -> result {
    result r("rat cons 2");

    r.start_clock();
    c8::rational v(-1313, 39);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("-101/3");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_3() -> result {
    result r("rat cons 3");

    r.start_clock();
    c8::rational v("-99999187951591759827592578257927598275980/25");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("-19999837590318351965518515651585519655196/5");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_4() -> result {
    result r("rat cons 4");

    r.start_clock();
    c8::rational v("2/5154875894574578457805710875418754097512875120572105234652346059");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("2/5154875894574578457805710875418754097512875120572105234652346059");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_5() -> result {
    result r("rat cons 5");

    r.start_clock();
    c8::rational v("0x2/0x2000000000000000000000000000000000000000000000000000000000000000");
    r.stop_clock();

    r.get_stream() << std::hex << std::showbase << v;
    r.check_pass("0x1/0x1000000000000000000000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_rational_construct_6() -> result {
    result r("rat cons 6");

    r.start_clock();
    try {
        c8::rational v("2323/01185415157637671751");
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
 * Test Constructors.
 */
auto test_rational_construct_7() -> result {
    result r("rat cons 7");

    r.start_clock();
    c8::rational v(1.125);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("9/8");
    return r;
}

/*
 * Test Constructors.
 */
auto test_rational_construct_8() -> result {
    result r("rat cons 8");

    r.start_clock();
    c8::rational v(-1.0/1048576);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("-1/1048576");
    return r;
}

/*
 * Attempt to construct with an invalid rational using a double precision infinity.
 */
auto test_rational_construct_9() -> result {
    result r("rat cons 9");

    r.start_clock();
    try {
        c8::rational v(std::numeric_limits<double>::quiet_NaN());
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
 * Construct a rational using an imprecise double.
 */
auto test_rational_construct_10() -> result {
    result r("rat cons 10");

    r.start_clock();
    c8::rational v(0.1);
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("ccccccccccccd/80000000000000");
    return r;
}

/*
 * Add two positive values.
 */
auto test_rational_add_0a() -> result {
    result r("rat add 0a");
    c8::rational a0("31/3");
    c8::rational a1("42/3");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("73/3");
    return r;
}

/*
 * Add two positive values.
 */
auto test_rational_add_0b() -> result {
    result r("rat add 0b");
    c8::rational a0("31/3");
    c8::rational a1("42/3");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("73/3");
    return r;
}

/*
 * Add a positive and a negative value.
 */
auto test_rational_add_1a() -> result {
    result r("rat add 1a");
    c8::rational a0("42/13");
    c8::rational a1("-1/2");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("71/26");
    return r;
}

/*
 * Add a positive and a negative value.
 */
auto test_rational_add_1b() -> result {
    result r("rat add 1b");
    c8::rational a0("42/13");
    c8::rational a1("-1/2");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("71/26");
    return r;
}

/*
 * Add a positive and a negative value that gives a negative result
 */
auto test_rational_add_2a() -> result {
    result r("rat add 2a");
    c8::rational a0("12345678/287923");
    c8::rational a1("-34738957485741895748957485743809574812345678/287923");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("-34738957485741895748957485743809574800000000/287923");
    return r;
}

/*
 * Add a positive and a negative value that gives a negative result
 */
auto test_rational_add_2b() -> result {
    result r("rat add 2b");
    c8::rational a0("12345678/287923");
    c8::rational a1("-34738957485741895748957485743809574812345678/287923");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("-34738957485741895748957485743809574800000000/287923");
    return r;
}

/*
 * Add two very large negative values.
 */
auto test_rational_add_3a() -> result {
    result r("rat add 3a");
    c8::rational a0("-10000000000000000000000000000000000000000000000000000000000000000008789/3");
    c8::rational a1("-88888880000000000000000000000000000000000000000000000000000000999992000/31");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("-192222213333333333333333333333333333333333333333333333333333334333416153/31");
    return r;
}

/*
 * Add two very large negative values.
 */
auto test_rational_add_3b() -> result {
    result r("rat add 3b");
    c8::rational a0("-10000000000000000000000000000000000000000000000000000000000000000008789/3");
    c8::rational a1("-88888880000000000000000000000000000000000000000000000000000000999992000/31");

    r.start_clock();
    a0 += a1;
    r.stop_clock();

    r.get_stream() << a0;
    r.check_pass("-192222213333333333333333333333333333333333333333333333333333334333416153/31");
    return r;
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_rational_subtract_0a() -> result {
    result r("rat sub 0a");
    c8::rational s0(52, 3);
    c8::rational s1(2, 4);

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("101/6");
    return r;
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_rational_subtract_0b() -> result {
    result r("rat sub 0b");
    c8::rational s0(52, 3);
    c8::rational s1(2, 4);

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("101/6");
    return r;
}

/*
 * Subtract a large negative value from another large negative value.
 */
auto test_rational_subtract_1a() -> result {
    result r("rat sub 1a");
    c8::rational s0("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897/3");
    c8::rational s1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754/3");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143/3");
    return r;
}

/*
 * Subtract a large negative value from another large negative value.
 */
auto test_rational_subtract_1b() -> result {
    result r("rat sub 1b");
    c8::rational s0("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897/3");
    c8::rational s1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754/3");

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143/3");
    return r;
}

/*
 * Subtract a large negative value from a large value.
 */
auto test_rational_subtract_2a() -> result {
    result r("rat sub 2a");
    c8::rational s0("10000000000000000000000000000000000000000000000000000000000000000000000/707");
    c8::rational s1("-10000000000000000000000000000000000000000000000000000000000000000000000/7");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("1020000000000000000000000000000000000000000000000000000000000000000000000/707");
    return r;
}

/*
 * Subtract a large negative value from a large value.
 */
auto test_rational_subtract_2b() -> result {
    result r("rat sub 2b");
    c8::rational s0("10000000000000000000000000000000000000000000000000000000000000000000000/707");
    c8::rational s1("-10000000000000000000000000000000000000000000000000000000000000000000000/7");

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("1020000000000000000000000000000000000000000000000000000000000000000000000/707");
    return r;
}

/*
 * Subtract a large value from a smaller one.
 */
auto test_rational_subtract_3a() -> result {
    result r("rat sub 3a");
    c8::rational s0(2, 31459);
    c8::rational s1(52, 31459);

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("-50/31459");
    return r;
}

/*
 * Subtract a large value from a smaller one.
 */
auto test_rational_subtract_3b() -> result {
    result r("rat sub 3b");
    c8::rational s0(2, 31459);
    c8::rational s1(52, 31459);

    r.start_clock();
    s0 -= s1;
    r.stop_clock();

    r.get_stream() << s0;
    r.check_pass("-50/31459");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0a() -> result {
    result r("rat comp 0a");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0b() -> result {
    result r("rat comp 0b");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0c() -> result {
    result r("rat comp 0c");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0d() -> result {
    result r("rat comp 0d");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0e() -> result {
    result r("rat comp 0e");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_rational_compare_0f() -> result {
    result r("rat comp 0f");
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1a() -> result {
    result r("rat comp 1a");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1b() -> result {
    result r("rat comp 1b");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1c() -> result {
    result r("rat comp 1c");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1d() -> result {
    result r("rat comp 1d");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1e() -> result {
    result r("rat comp 1e");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a negative value with a positive one.
 */
auto test_rational_compare_1f() -> result {
    result r("rat comp 1f");
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2a() -> result {
    result r("rat comp 2a");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2b() -> result {
    result r("rat comp 2b");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2c() -> result {
    result r("rat comp 2c");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2d() -> result {
    result r("rat comp 2d");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2e() -> result {
    result r("rat comp 2e");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare a positive value with a negative one.
 */
auto test_rational_compare_2f() -> result {
    result r("rat comp 2f");
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3a() -> result {
    result r("rat comp 3a");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 == co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3b() -> result {
    result r("rat comp 3b");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 != co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3c() -> result {
    result r("rat comp 3c");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 > co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3d() -> result {
    result r("rat comp 3d");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 >= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3e() -> result {
    result r("rat comp 3e");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 < co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("0");
    return r;
}

/*
 * Compare two negative values.
 */
auto test_rational_compare_3f() -> result {
    result r("rat comp 3f");
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);

    r.start_clock();
    auto co2 = (co0 <= co1);
    r.stop_clock();

    r.get_stream() << co2;
    r.check_pass("1");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_0a() -> result {
    result r("rat mul 0a");
    c8::rational mu0(1, 50);
    c8::rational mu1(1, 25);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("1/1250");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_0b() -> result {
    result r("rat mul 0b");
    c8::rational mu0(1, 50);
    c8::rational mu1(1, 25);

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("1/1250");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_1a() -> result {
    result r("rat mul 1a");
    c8::rational mu0(1000000000000000000LL, 7);
    c8::rational mu1("-9999999999999999999/999");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("-1111111111111111111000000000000000000/777");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_1b() -> result {
    result r("rat mul 1b");
    c8::rational mu0(1000000000000000000LL, 7);
    c8::rational mu1("-9999999999999999999/999");

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("-1111111111111111111000000000000000000/777");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_2a() -> result {
    result r("rat mul 2a");
    c8::rational mu0(-0x3000000000000000LL, 1);
    c8::rational mu1(0x4000000000000000LL, 3);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("-4000000000000000000000000000000/1");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_2b() -> result {
    result r("rat mul 2b");
    c8::rational mu0(-0x3000000000000000LL, 1);
    c8::rational mu1(0x4000000000000000LL, 3);

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu0;
    r.check_pass("-4000000000000000000000000000000/1");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_3a() -> result {
    result r("rat mul 3a");
    c8::rational mu0("-12345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    c8::rational mu1("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890/13");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100/169");
    return r;
}

/*
 * Test multiplication.
 */
auto test_rational_multiply_3b() -> result {
    result r("rat mul 3b");
    c8::rational mu0("-12345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    c8::rational mu1("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890/13");

    r.start_clock();
    mu0 *= mu1;
    r.stop_clock();

    r.get_stream() << mu0;
    r.check_pass("15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100/169");
    return r;
}

/*
 * Test division.
 */
auto test_rational_divide_0a() -> result {
    result r("rat div 0a");
    c8::rational d0(1000000000000000000LL,7);
    c8::rational d1(99999999999999999LL,7);

    r.start_clock();
    auto d2 = d0 / d1;
    r.stop_clock();

    r.get_stream() << d2;
    r.check_pass("1000000000000000000/99999999999999999");
    return r;
}

/*
 * Test division.
 */
auto test_rational_divide_0b() -> result {
    result r("rat div 0b");
    c8::rational d0(1000000000000000000LL,7);
    c8::rational d1(99999999999999999LL,7);

    r.start_clock();
    d0 /= d1;
    r.stop_clock();

    r.get_stream() << d0;
    r.check_pass("1000000000000000000/99999999999999999");
    return r;
}

auto test_rational_divide_1a() -> result {
    result r("rat div 1a");
    c8::rational d0("7829238792751875818917817519758789749174743847389742871867617465710657162/39");
    c8::rational d1(-99999999999999999LL, 39);

    r.start_clock();
    auto d2 = d0 / d1;
    r.stop_clock();

    r.get_stream() << d2;
    r.check_pass("-7829238792751875818917817519758789749174743847389742871867617465710657162/99999999999999999");
    return r;
}

auto test_rational_divide_1b() -> result {
    result r("rat div 1b");
    c8::rational d0("7829238792751875818917817519758789749174743847389742871867617465710657162/39");
    c8::rational d1(-99999999999999999LL, 39);

    r.start_clock();
    d0 /= d1;
    r.stop_clock();

    r.get_stream() << d0;
    r.check_pass("-7829238792751875818917817519758789749174743847389742871867617465710657162/99999999999999999");
    return r;
}

auto test_rational_divide_2a() -> result {
    result r("rat div 2a");
    c8::rational d0("-0x100000000000000000000000000000000000000000000000000000000000000000000000/2323978529");
    c8::rational d1("0x10000000000000001000000000000000100000000/0x17");

    r.start_clock();
    auto d2 = d0 / d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2;
    r.check_pass("-17000000000000000000000000000000000000000000000000000000000000000/8a851921000000008a851921000000008a851921");
    return r;
}

auto test_rational_divide_2b() -> result {
    result r("rat div 2b");
    c8::rational d0("-0x100000000000000000000000000000000000000000000000000000000000000000000000/2323978529");
    c8::rational d1("0x10000000000000001000000000000000100000000/0x17");

    r.start_clock();
    d0 /= d1;
    r.stop_clock();

    r.get_stream() << std::hex << d0;
    r.check_pass("-17000000000000000000000000000000000000000000000000000000000000000/8a851921000000008a851921000000008a851921");
    return r;
}

auto test_rational_divide_3a() -> result {
    result r("rat div 3a");

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::rational d0(2000,7);
    c8::rational d1(0,1);
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

auto test_rational_divide_3b() -> result {
    result r("rat div 3b");

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::rational d0(2000,7);
    c8::rational d1(0,1);
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

auto test_rational_divide_4a() -> result {
    result r("rat div 4a");
    c8::rational d0(-1000000000000000000LL,23923758279);
    c8::rational d1(-99999999999999999LL,28279753);

    r.start_clock();
    auto d2 = d0 / d1;
    r.stop_clock();

    r.get_stream() << d2;
    r.check_pass("28279753000000000000000000/2392375827899999976076241721");
    return r;
}

auto test_rational_divide_4b() -> result {
    result r("rat div 4b");
    c8::rational d0(-1000000000000000000LL,23923758279);
    c8::rational d1(-99999999999999999LL,28279753);

    r.start_clock();
    d0 /= d1;
    r.stop_clock();

    r.get_stream() << d0;
    r.check_pass("28279753000000000000000000/2392375827899999976076241721");
    return r;
}

/*
 * Test to_double functionality.
 */
auto test_rational_to_double_0() -> result {
    result r("rat todouble 0");
    c8::rational v(0, 1);

    r.start_clock();
    double d = to_double(v);
    r.stop_clock();

    r.get_stream() << d;
    r.check_pass("0");
    return r;
}

/*
 * Test to_double functionality.
 */
auto test_rational_to_double_1() -> result {
    result r("rat todouble 1");
    c8::rational v(-3000, 59);

    r.start_clock();
    double d = to_double(v);
    r.stop_clock();

    r.get_stream() << d;
    r.check_pass("-50.8475");
    return r;
}

/*
 * Test to_double functionality.
 */
auto test_rational_to_double_2() -> result {
    result r("rat todouble 2");
    c8::rational v("47895748574857485728747548237543205782573485472759047548275024574207/389275892758257298");

    r.start_clock();
    double d = to_double(v);
    r.stop_clock();

    r.get_stream() << d;
    r.check_pass("1.23038e+50");
    return r;
}

/*
 * Test to_double functionality.
 */
auto test_rational_to_double_3() -> result {
    result r("rat todouble 3");
    c8::rational v(0.1);

    r.start_clock();
    double d = to_double(v);
    r.stop_clock();

    r.get_stream() << d;
    r.check_pass("0.1");
    return r;
}

/*
 * Test to_double functionality.
 */
auto test_rational_to_double_4() -> result {
    result r("rat todouble 4");

    c8::integer i = 1;
    c8::rational v(i << 2048, c8::natural(1));
    r.start_clock();
    try {
        double d = to_double(v);
        r.stop_clock();

        r.get_stream() << d << " (failed to throw exception)";
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
 * Test to_parts functionality.
 */
auto test_rational_to_parts_0() -> result {
    result r("rat toparts 0");
    c8::rational v(0, 1);

    r.start_clock();
    auto x = to_parts(v);
    r.stop_clock();

    r.get_stream() << x.first << ',' << x.second;
    r.check_pass("0,1");
    return r;
}

/*
 * Test to_parts functionality.
 */
auto test_rational_to_parts_1() -> result {
    result r("rat toparts 1");
    c8::rational v(-3000, 58);

    r.start_clock();
    auto x = to_parts(v);
    r.stop_clock();

    r.get_stream() << x.first << ',' << x.second;
    r.check_pass("-1500,29");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_0() -> result {
    result r("rat prn 0");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << v;
    r.stop_clock();

    r.check_pass("-4701397401952099592073/65689");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_1() -> result {
    result r("rat prn 1");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::hex << v;
    r.stop_clock();

    r.check_pass("-fedcfedc0123456789/10099");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_2() -> result {
    result r("rat prn 2");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("-FEDCFEDC0123456789/10099");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_3() -> result {
    result r("rat prn 3");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::oct << v;
    r.stop_clock();

    r.check_pass("-775563766700044321263611/200231");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_4() -> result {
    result r("rat prn 4");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::showbase << v;
    r.stop_clock();

    r.check_pass("-4701397401952099592073/65689");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_5() -> result {
    result r("rat prn 5");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::showbase << std::hex << v;
    r.stop_clock();

    r.check_pass("-0xfedcfedc0123456789/0x10099");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_6() -> result {
    result r("rat prn 6");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::showbase << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("-0XFEDCFEDC0123456789/0X10099");
    return r;
}

/*
 * Test printing.
 */
auto test_rational_print_7() -> result {
    result r("rat prn 7");
    c8::rational v("-0xfedcfedc0123456789/65689");

    r.start_clock();
    r.get_stream() << std::showbase << std::oct << v;
    r.stop_clock();

    r.check_pass("-0775563766700044321263611/0200231");
    return r;
}

