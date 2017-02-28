/*
 * integer_check.cpp
 */
#include <integer.h>

#include "result.h"

/*
 * Construct with a long integer 0.
 */
auto test_integer_construct_0() -> result {
    result r("int cons 0");

    r.start_clock();
    c8::integer v(0);
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("0");
    return r;
}

/*
 * Construct with a long integer.
 */
auto test_integer_construct_1() -> result {
    result r("int cons 1");

    r.start_clock();
    c8::integer v(0x123456789abcLL);
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("123456789abc");
    return r;
}

/*
 * Construct with a string 0.
 */
auto test_integer_construct_2() -> result {
    result r("int cons 2");

    r.start_clock();
    c8::integer v("0");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("0");
    return r;
}

/*
 * Construct with a hexadecimal string.
 */
auto test_integer_construct_3() -> result {
    result r("int cons 3");

    r.start_clock();
    c8::integer v("0x3837439787487386792386728abcd88379dc");
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("3837439787487386792386728abcd88379dc");
    return r;
}

/*
 * Construct with a decimal string.
 */
auto test_integer_construct_4() -> result {
    result r("int cons 4");

    r.start_clock();
    c8::integer v("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    return r;
}

/*
 * Construct with an octal string.
 */
auto test_integer_construct_5() -> result {
    result r("int cons 5");

    r.start_clock();
    c8::integer v("0115415157637671751");
    r.stop_clock();

    r.get_stream() << std::oct << v;
    r.check_pass("115415157637671751");
    return r;
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_integer_construct_6() -> result {
    result r("int cons 6");

    r.start_clock();
    try {
        c8::integer v("01185415157637671751");
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
 * Construct with a long integer.
 */
auto test_integer_construct_7() -> result {
    result r("int cons 7");

    r.start_clock();
    c8::integer v(-0x123456789abcLL);
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("-123456789abc");
    return r;
}

/*
 * Construct with a hexadecimal string.
 */
auto test_integer_construct_8() -> result {
    result r("int cons 8");

    r.start_clock();
    c8::integer v("-0x3837439787487386792386728abcd88379dc");
    r.stop_clock();

    r.get_stream() << std::hex << v;
    r.check_pass("-3837439787487386792386728abcd88379dc");
    return r;
}

/*
 * Construct with a decimal string.
 */
auto test_integer_construct_9() -> result {
    result r("int cons 9");

    r.start_clock();
    c8::integer v("-3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    r.stop_clock();

    r.get_stream() << v;
    r.check_pass("-3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    return r;
}

/*
 * Construct with an octal string.
 */
auto test_integer_construct_10() -> result {
    result r("int cons 10");

    r.start_clock();
    c8::integer v("-0115415157637671751");
    r.stop_clock();

    r.get_stream() << std::oct << v;
    r.check_pass("-115415157637671751");
    return r;
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_integer_construct_11() -> result {
    result r("int cons 11");

    r.start_clock();
    try {
        c8::integer v("-01185415157637671751");
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
 * Add two positive values.
 */
auto test_integer_add_0() -> result {
    result r("int add 0");
    c8::integer a0("31");
    c8::integer a1("42");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("73");
    return r;
}

/*
 * Add a positive and a negative value.
 */
auto test_integer_add_1() -> result {
    result r("int add 1");
    c8::integer a0("42");
    c8::integer a1("-21");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("21");
    return r;
}

/*
 * Add a positive and a negative value that gives a negative result
 */
auto test_integer_add_2() -> result {
    result r("int add 2");
    c8::integer a0("12345678");
    c8::integer a1("-34738957485741895748957485743809574812345678");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("-34738957485741895748957485743809574800000000");
    return r;
}

/*
 * Add two very large negative values.
 */
auto test_integer_add_3() -> result {
    result r("int add 3");
    c8::integer a0("-10000000000000000000000000000000000000000000000000000000000000000008789");
    c8::integer a1("-88888880000000000000000000000000000000000000000000000000000000999992000");

    r.start_clock();
    auto a2 = a0 + a1;
    r.stop_clock();

    r.get_stream() << a2;
    r.check_pass("-98888880000000000000000000000000000000000000000000000000000001000000789");
    return r;
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_integer_subtract_0() -> result {
    result r("int sub 0");
    c8::integer s0(52);
    c8::integer s1(2);

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("50");
    return r;
}

/*
 * Subtract a large negative value from another large negative value.
 */
auto test_integer_subtract_1() -> result {
    result r("int sub 1");
    c8::integer s0("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::integer s1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143");
    return r;
}

/*
 * Subtract a large negative value from a large value.
 */
auto test_integer_subtract_2() -> result {
    result r("int sub 2");
    c8::integer s0("10000000000000000000000000000000000000000000000000000000000000000000000");
    c8::integer s1("-10000000000000000000000000000000000000000000000000000000000000000000000");

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("20000000000000000000000000000000000000000000000000000000000000000000000");
    return r;
}

/*
 * Subtract a large value from a smaller one.
 */
auto test_integer_subtract_3() -> result {
    result r("int sub 3");
    c8::integer s0(2);
    c8::integer s1(52);

    r.start_clock();
    auto s2 = s0 - s1;
    r.stop_clock();

    r.get_stream() << s2;
    r.check_pass("-50");
    return r;
}

/*
 * Compare two positive values.
 */
auto test_integer_compare_0a() -> result {
    result r("int comp 0a");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_0b() -> result {
    result r("int comp 0b");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_0c() -> result {
    result r("int comp 0c");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_0d() -> result {
    result r("int comp 0d");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_0e() -> result {
    result r("int comp 0e");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_0f() -> result {
    result r("int comp 0f");
    c8::integer co0(2);
    c8::integer co1(1);

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
auto test_integer_compare_1a() -> result {
    result r("int comp 1a");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_1b() -> result {
    result r("int comp 1b");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_1c() -> result {
    result r("int comp 1c");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_1d() -> result {
    result r("int comp 1d");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_1e() -> result {
    result r("int comp 1e");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_1f() -> result {
    result r("int comp 1f");
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);

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
auto test_integer_compare_2a() -> result {
    result r("int comp 2a");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_2b() -> result {
    result r("int comp 2b");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_2c() -> result {
    result r("int comp 2c");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_2d() -> result {
    result r("int comp 2d");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_2e() -> result {
    result r("int comp 2e");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_2f() -> result {
    result r("int comp 2f");
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);

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
auto test_integer_compare_3a() -> result {
    result r("int comp 3a");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_compare_3b() -> result {
    result r("int comp 3b");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_compare_3c() -> result {
    result r("int comp 3c");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_compare_3d() -> result {
    result r("int comp 3d");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_compare_3e() -> result {
    result r("int comp 3e");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_compare_3f() -> result {
    result r("int comp 3f");
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);

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
auto test_integer_lshift_0() -> result {
    result r("int lsh 0");
    c8::integer l0(0x349f);

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
auto test_integer_lshift_1() -> result {
    result r("int lsh 1");
    c8::integer l0(0x349f);

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
auto test_integer_lshift_2() -> result {
    result r("int lsh 2");
    c8::integer l0(0x349f);

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
auto test_integer_lshift_3() -> result {
    result r("int lsh 3");
    c8::integer l0(0x349f);

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
auto test_integer_lshift_4() -> result {
    result r("int lsh 4");
    c8::integer l0("-0x349f298375323985afbce9837928798789dffeffee987678687678676756562");

    r.start_clock();
    auto l1 = l0 << 69;
    r.stop_clock();

    r.get_stream() << std::hex << l1;
    r.check_pass("-693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    return r;
}

/*
 * Test right shifting.
 */
auto test_integer_rshift_0() -> result {
    result r("int rsh 0");
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");

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
auto test_integer_rshift_1() -> result {
    result r("int rsh 1");
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");

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
auto test_integer_rshift_2() -> result {
    result r("int rsh 2");
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");

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
auto test_integer_rshift_3() -> result {
    result r("int rsh 3");
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");

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
auto test_integer_rshift_4() -> result {
    result r("int rsh 4");
    c8::integer r0("-0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");

    r.start_clock();
    auto r1 = r0 >> 123;
    r.stop_clock();

    r.get_stream() << std::hex << r1;
    r.check_pass("-d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a");
    return r;
}

/*
 * Test multiplication.
 */
auto test_integer_multiply_0() -> result {
    result r("int mul 0");
    c8::integer mu0(3);
    c8::integer mu1(22);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("66");
    return r;
}

/*
 * Test multiplication.
 */
auto test_integer_multiply_1() -> result {
    result r("int mul 1");
    c8::integer mu0(1000000000000000000LL);
    c8::integer mu1("-9999999999999999999");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("-9999999999999999999000000000000000000");
    return r;
}

/*
 * Test multiplication.
 */
auto test_integer_multiply_2() -> result {
    result r("int mul 2");
    c8::integer mu0(-0x2000000000000000LL);
    c8::integer mu1(0x4000000000000000LL);

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << std::hex << mu2;
    r.check_pass("-8000000000000000000000000000000");
    return r;
}

/*
 * Test multiplication.
 */
auto test_integer_multiply_3() -> result {
    result r("int mul 3");
    c8::integer mu0("-12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::integer mu1("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

    r.start_clock();
    auto mu2 = mu0 * mu1;
    r.stop_clock();

    r.get_stream() << mu2;
    r.check_pass("15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100");
    return r;
}

/*
 * Test division.
 */
auto test_integer_divide_0() -> result {
    result r("int div 0");
    c8::integer d0(1000000000000000000LL);
    c8::integer d1(99999999999999999LL);

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("10,10");
    return r;
}

/*
 * Test division.
 */
auto test_integer_divide_1() -> result {
    result r("int div 1");
    c8::integer d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::integer d1(-99999999999999999LL);

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("-78292387927518758972102054472775487212767983201652300846,35600667362958008");
    return r;
}

/*
 * Test division.
 */
auto test_integer_divide_2() -> result {
    result r("int div 2");
    c8::integer d0("-0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::integer d1("0x10000000000000001000000000000000100000000");

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << std::hex << d2 << ',' << mo2;
    r.check_pass("-ffffffffffffffff000000000000000,100000000000000000000000");
    return r;
}

/*
 * Test division.
 */
auto test_integer_divide_3() -> result {
    result r("int div 3");
    c8::integer d0(2000);
    c8::integer d1(0);

    /*
     * Divide by zero.  This will throw an exception!
     */
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
 * Test division.
 */
auto test_integer_divide_4() -> result {
    result r("int div 4");
    c8::integer d0(-1000000000000000000LL);
    c8::integer d1(-99999999999999999LL);

    r.start_clock();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    r.stop_clock();

    r.get_stream() << d2 << ',' << mo2;
    r.check_pass("10,10");
    return r;
}

/*
 * Test to_long_long functionality.
 */
auto test_integer_to_long_long_0() -> result {
    result r("int toll 0");
    c8::integer i(0);

    r.start_clock();
    long long l = to_long_long(i);
    r.stop_clock();

    r.get_stream() << l;
    r.check_pass("0");
    return r;
}

/*
 * Test to_long_long functionality.
 */
auto test_integer_to_long_long_1() -> result {
    result r("int toll 1");
    c8::integer i(-3000);

    r.start_clock();
    long long l = to_long_long(i);
    r.stop_clock();

    r.get_stream() << l;
    r.check_pass("-3000");
    return r;
}

/*
 * Test to_long_long functionality.
 */
auto test_integer_to_long_long_2() -> result {
    result r("int toll 2");
    c8::integer i("47895748574857485728747548237543205782573485472759047548275024574207");

    r.start_clock();
    try {
        long long l = to_long_long(i);
        r.stop_clock();

        r.get_stream() << l << " (failed to throw exception)";
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
 * Test to_long_long functionality.
 */
auto test_integer_to_long_long_3() -> result {
    result r("int toll 3");
    c8::integer i(-12345678987654321LL);

    r.start_clock();
    long long l = to_long_long(i);
    r.stop_clock();

    r.get_stream() << l;
    r.check_pass("-12345678987654321");
    return r;
}

/*
 * Test to_long_long functionality.
 */
auto test_integer_to_long_long_4() -> result {
    result r("int toll 4");

    /*
     * Construct an integer that is one bit too large to be able to convert.
     */
    c8::integer i0(1);
    c8::integer i1 = i0 << ((sizeof(unsigned long long) * 8) - 1);

    r.start_clock();
    try {
        long long l = to_long_long(i1);
        r.stop_clock();

        r.get_stream() << l << " (failed to throw exception)";
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
auto test_integer_print_0() -> result {
    result r("int prn 0");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << v;
    r.stop_clock();

    r.check_pass("-4701397401952099592073");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_1() -> result {
    result r("int prn 1");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::hex << v;
    r.stop_clock();

    r.check_pass("-fedcfedc0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_2() -> result {
    result r("int prn 2");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("-FEDCFEDC0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_3() -> result {
    result r("int prn 3");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::oct << v;
    r.stop_clock();

    r.check_pass("-775563766700044321263611");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_4() -> result {
    result r("int prn 4");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << v;
    r.stop_clock();

    r.check_pass("-4701397401952099592073");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_5() -> result {
    result r("int prn 5");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::hex << v;
    r.stop_clock();

    r.check_pass("-0xfedcfedc0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_6() -> result {
    result r("int prn 6");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::uppercase << std::hex << v;
    r.stop_clock();

    r.check_pass("-0XFEDCFEDC0123456789");
    return r;
}

/*
 * Test printing.
 */
auto test_integer_print_7() -> result {
    result r("int prn 7");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::oct << v;
    r.stop_clock();

    r.check_pass("-0775563766700044321263611");
    return r;
}

