/*
 * integer_check.c
 */
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include <integer.h>

/*
 * Result class.
 */
class result {
public:
    result(const std::string &name) {
        name_ = name;
    }

    auto get_name() -> const std::string & {
        return name_;
    }

    auto get_expected() -> const std::string & {
        return expected_;
    }

    auto get_pass() -> bool {
        return pass_;
    }

    auto set_pass(bool p) -> void {
        pass_ = p;
    }

    auto check_pass(const std::string &expected) -> bool {
        expected_ = expected;
        pass_ = (expected == s_.str());
        return pass_;
    }

    auto get_stream() -> std::stringstream & {
        return s_;
    }

    auto start_clock() -> void {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    auto stop_clock() -> void {
        end_time_ = std::chrono::high_resolution_clock::now();
    }

    auto get_elapsed() -> std::chrono::high_resolution_clock::duration {
        return end_time_ - start_time_;
    }

private:
    std::string name_;                  // Test name
    std::string expected_;              // Expected result
    bool pass_;                         // Did this test pass?
    std::chrono::high_resolution_clock::time_point start_time_;
                                        // Start time
    std::chrono::high_resolution_clock::time_point end_time_;
                                        // End time
    std::stringstream s_;               // Output string stream
};

/*
 * Construct with a long integer 0.
 */
auto test_construct_0() -> result {
    result r("cons 0");

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
auto test_construct_1() -> result {
    result r("cons 1");

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
auto test_construct_2() -> result {
    result r("cons 2");

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
auto test_construct_3() -> result {
    result r("cons 3");

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
auto test_construct_4() -> result {
    result r("cons 4");

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
auto test_construct_5() -> result {
    result r("cons 5");

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
auto test_construct_6() -> result {
    result r("cons 6");

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
auto test_construct_7() -> result {
    result r("cons 7");

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
auto test_construct_8() -> result {
    result r("cons 8");

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
auto test_construct_9() -> result {
    result r("cons 9");

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
auto test_construct_10() -> result {
    result r("cons 10");

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
auto test_construct_11() -> result {
    result r("cons 11");

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
auto test_add_0() -> result {
    result r("add 0");
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
auto test_add_1() -> result {
    result r("add 1");
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
auto test_add_2() -> result {
    result r("add 2");
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
auto test_add_3() -> result {
    result r("add 3");
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
auto test_subtract_0() -> result {
    result r("sub 0");
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
auto test_subtract_1() -> result {
    result r("sub 1");
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
auto test_subtract_2() -> result {
    result r("sub 2");
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
auto test_subtract_3() -> result {
    result r("sub 3");
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
auto test_compare_0a() -> result {
    result r("comp 0a");
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
auto test_compare_0b() -> result {
    result r("comp 0b");
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
auto test_compare_0c() -> result {
    result r("comp 0c");
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
auto test_compare_0d() -> result {
    result r("comp 0d");
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
auto test_compare_0e() -> result {
    result r("comp 0e");
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
auto test_compare_0f() -> result {
    result r("comp 0f");
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
auto test_compare_1a() -> result {
    result r("comp 1a");
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
auto test_compare_1b() -> result {
    result r("comp 1b");
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
auto test_compare_1c() -> result {
    result r("comp 1c");
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
auto test_compare_1d() -> result {
    result r("comp 1d");
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
auto test_compare_1e() -> result {
    result r("comp 1e");
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
auto test_compare_1f() -> result {
    result r("comp 1f");
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
auto test_compare_2a() -> result {
    result r("comp 2a");
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
auto test_compare_2b() -> result {
    result r("comp 2b");
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
auto test_compare_2c() -> result {
    result r("comp 2c");
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
auto test_compare_2d() -> result {
    result r("comp 2d");
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
auto test_compare_2e() -> result {
    result r("comp 2e");
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
auto test_compare_2f() -> result {
    result r("comp 2f");
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
auto test_compare_3a() -> result {
    result r("comp 3a");
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
auto test_compare_3b() -> result {
    result r("comp 3b");
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
auto test_compare_3c() -> result {
    result r("comp 3c");
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
auto test_compare_3d() -> result {
    result r("comp 3d");
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
auto test_compare_3e() -> result {
    result r("comp 3e");
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
auto test_compare_3f() -> result {
    result r("comp 3f");
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
auto test_lshift_0() -> result {
    result r("lsh 0");
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
auto test_lshift_1() -> result {
    result r("lsh 1");
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
auto test_lshift_2() -> result {
    result r("lsh 2");
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
auto test_lshift_3() -> result {
    result r("lsh 3");
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
auto test_lshift_4() -> result {
    result r("lsh 4");
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
auto test_rshift_0() -> result {
    result r("rsh 0");
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
auto test_rshift_1() -> result {
    result r("rsh 1");
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
auto test_rshift_2() -> result {
    result r("rsh 2");
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
auto test_rshift_3() -> result {
    result r("rsh 3");
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
auto test_rshift_4() -> result {
    result r("rsh 4");
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
auto test_multiply_0() -> result {
    result r("mul 0");
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
auto test_multiply_1() -> result {
    result r("mul 1");
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
auto test_multiply_2() -> result {
    result r("mul 2");
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
auto test_multiply_3() -> result {
    result r("mul 3");
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
auto test_divide_0() -> result {
    result r("div 0");
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
auto test_divide_1() -> result {
    result r("div 1");
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
auto test_divide_2() -> result {
    result r("div 2");
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
auto test_divide_3() -> result {
    result r("div 3");
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
auto test_divide_4() -> result {
    result r("div 4");
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
auto test_to_long_long_0() -> result {
    result r("toll 0");
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
auto test_to_long_long_1() -> result {
    result r("toll 1");
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
auto test_to_long_long_2() -> result {
    result r("toll 2");
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
auto test_to_long_long_3() -> result {
    result r("toll 3");
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
auto test_to_long_long_4() -> result {
    result r("toll 4");

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
auto test_print_0() -> result {
    result r("prn 0");
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
auto test_print_1() -> result {
    result r("prn 1");
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
auto test_print_2() -> result {
    result r("prn 2");
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
auto test_print_3() -> result {
    result r("prn 3");
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
auto test_print_4() -> result {
    result r("prn 4");
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
auto test_print_5() -> result {
    result r("prn 5");
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
auto test_print_6() -> result {
    result r("prn 6");
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
auto test_print_7() -> result {
    result r("prn 7");
    c8::integer v("-0xfedcfedc0123456789");

    r.start_clock();
    r.get_stream() << std::showbase << std::oct << v;
    r.stop_clock();

    r.check_pass("-0775563766700044321263611");
    return r;
}

/*
 * Report the usage for this test program.
 */
static auto usage(const char *name) -> void {
    std::cerr << "usage: " << name << " [OPTIONS]\n\n";
    std::cerr << "Options\n";
    std::cerr << "  -v  Verbose reporting (optional)\n\n";
}

typedef result (*test)();

/*
 * List of tests to run.
 */
test tests[] = {
    test_construct_0,
    test_construct_1,
    test_construct_2,
    test_construct_3,
    test_construct_4,
    test_construct_5,
    test_construct_6,
    test_construct_7,
    test_construct_8,
    test_construct_9,
    test_construct_10,
    test_construct_11,
    test_add_0,
    test_add_1,
    test_add_2,
    test_add_3,
    test_subtract_0,
    test_subtract_1,
    test_subtract_2,
    test_subtract_3,
    test_compare_0a,
    test_compare_0b,
    test_compare_0c,
    test_compare_0d,
    test_compare_0e,
    test_compare_0f,
    test_compare_1a,
    test_compare_1b,
    test_compare_1c,
    test_compare_1d,
    test_compare_1e,
    test_compare_1f,
    test_compare_2a,
    test_compare_2b,
    test_compare_2c,
    test_compare_2d,
    test_compare_2e,
    test_compare_2f,
    test_compare_3a,
    test_compare_3b,
    test_compare_3c,
    test_compare_3d,
    test_compare_3e,
    test_compare_3f,
    test_lshift_0,
    test_lshift_1,
    test_lshift_2,
    test_lshift_3,
    test_lshift_4,
    test_rshift_0,
    test_rshift_1,
    test_rshift_2,
    test_rshift_3,
    test_rshift_4,
    test_multiply_0,
    test_multiply_1,
    test_multiply_2,
    test_multiply_2,
    test_divide_0,
    test_divide_1,
    test_divide_2,
    test_divide_3,
    test_divide_4,
    test_to_long_long_0,
    test_to_long_long_1,
    test_to_long_long_2,
    test_to_long_long_3,
    test_to_long_long_4,
    test_print_0,
    test_print_1,
    test_print_2,
    test_print_3,
    test_print_4,
    test_print_5,
    test_print_6,
    test_print_7,
    nullptr
};

/*
 * Entry point.
 */
auto main(int argc, char **argv) -> int {
    bool verbose = false;
    int loops = 1;

    /*
     * Parse the command line options.
     */
    int ch;
    while ((ch = getopt(argc, argv, "bv?")) != -1) {
        switch (ch) {
        case 'b':
            loops = 10000;
            break;

        case 'v':
            verbose = true;
            break;

        case '?':
            usage(argv[0]);
            exit(-1);
        }
    }

    /*
     * Run the tests.
     */
    bool res = true;

    test *p = tests;
    while (*p) {
        std::vector<std::chrono::nanoseconds> duration;

        /*
         * Run one test every time.  This gives the pass/fail status.
         */
        bool rp = true;
        result r = (*p)();
        rp = r.get_pass();
        duration.push_back(r.get_elapsed());

        /*
         * Run more tests if we're running a benchmark.
         */
        for (auto i = 1; i < loops; i++) {
            result l = (*p)();
            duration.push_back(l.get_elapsed());
        }

        /*
         * If we're being verbose then print the results.  We print the 20th percentile result if
         * this is a benchmark test.  Lower than the 10th percentile we can see some rather
         * odd timing results and greater than the 50th percentile we can run into problems
         * related to scheduling.  The 20th percentile number is an empirical choice.
         */
        if (verbose) {
            std::nth_element(duration.begin(), duration.begin() + (duration.size() / 5), duration.end());
            std::cout << std::setw(10) << std::left << r.get_name() << " | ";
            std::cout << std::setw(10) << std::right << duration[duration.size() / 5].count() << " | " << (rp ? "pass" : "FAIL");
            std::cout << " | " << r.get_stream().str();
            if (!rp) {
                std::cout << " (" << r.get_expected() << ')';
            }

            std::cout << '\n';
        }

        res &= rp;
        p++;
    }

    if (!res) {
        std::cout << "TESTS FAILED!\n";
        exit(-1);
    }

    std::cout << "All tests passed\n";
    return 0;
}

