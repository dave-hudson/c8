/*
 * integer_check.c
 */
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include <integer.h>

bool verbose = false;

/*
 * Get the elapsed number of system ticks before we start a test running.
 */
auto get_start_time_ticks() -> uint64_t {
    uint32_t lo;
    uint32_t hi;

    asm volatile(
        "cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %1\n\t"
        "mov %%eax, %0\n\t"
        : "=r" (lo), "=r" (hi)
        :
        : "rax", "rbx", "rcx", "rdx", "memory"
    );
    return (static_cast<uint64_t>(hi) << 32) | static_cast<uint64_t>(lo);
}

/*
 * Get the elapsed number of system ticks after we complete a test running.
 */
auto get_end_time_ticks() -> uint64_t {
    uint32_t lo;
    uint32_t hi;

    asm volatile(
        "rdtscp\n\t"
        "mov %%edx, %1\n\t"
        "mov %%eax, %0\n\t"
        "cpuid\n\t"
        : "=r" (lo), "=r" (hi)
        :
        : "rax", "rbx", "rcx", "rdx", "memory"
    );
    return (static_cast<uint64_t>(hi) << 32) | static_cast<uint64_t>(lo);
}

/*
 * Test a string against a string stream that should contain the same text.
 *
 * Returns true if the string and string stream match, false if they don't.
 * If the global "verbose" flag is set then also prints the results.
 */
auto test_check(const std::string &tag, uint64_t t, const std::string &s, const std::stringstream &ss) -> bool {
    bool res = (s == ss.str());

    if (verbose) {
        std::cout << std::setw(10) << std::left << tag << " | ";
        std::cout << std::setw(10) << std::right << t << " | " << (res ? "pass" : "FAIL") << " | " << ss.str();
        if (!res) {
            std::cout << " (" << s << ')';
        }

        std::cout << '\n';
    }

    return res;
}

/*
 * Similar to test_check() but doesn't actually check anything; the result is already
 * known.
 *
 * This function allows uniform handling of test outputs.
 */
auto test_nocheck(const std::string &tag, uint64_t t, const std::string &s, bool res) {
    if (verbose) {
        std::cout << std::setw(10) << std::left << tag << " | ";
        std::cout << std::setw(10) << std::right << t << " | " << (res ? "pass" : "FAIL") << " | " << s << '\n';
    }

    return res;
}

/*
 * Construct with a long integer 0.
 */
auto test_construct_0() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v(0);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 0", p, "0", s);
}

/*
 * Construct with a long integer.
 */
auto test_construct_1() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v(0x123456789abcLL);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 1", p, "123456789abc", s);
}

/*
 * Construct with a string 0.
 */
auto test_construct_2() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("0");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 2", p, "0", s);
}

/*
 * Construct with a hexadecimal string.
 */
auto test_construct_3() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("0x3837439787487386792386728abcd88379dc");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 3", p, "3837439787487386792386728abcd88379dc", s);
}

/*
 * Construct with a decimal string.
 */
auto test_construct_4() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 4", p, "3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097", s);
}

/*
 * Construct with an octal string.
 */
auto test_construct_5() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("0115415157637671751");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::oct << v;
    return test_check("cons 5", p, "115415157637671751", s);
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_construct_6() -> bool {
    bool res = true;

    auto t = get_start_time_ticks();
    try {
        c8::integer v("01185415157637671751");
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 6", p, "failed to throw exception", false);
    } catch (const c8::invalid_argument &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 6", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 6", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Construct with a long integer.
 */
auto test_construct_7() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v(-0x123456789abcLL);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 7", p, "-123456789abc", s);
}

/*
 * Construct with a hexadecimal string.
 */
auto test_construct_8() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("-0x3837439787487386792386728abcd88379dc");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 8", p, "-3837439787487386792386728abcd88379dc", s);
}

/*
 * Construct with a decimal string.
 */
auto test_construct_9() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("-3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 9", p, "-3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097", s);
}

/*
 * Construct with an octal string.
 */
auto test_construct_10() -> bool {
    auto t = get_start_time_ticks();
    c8::integer v("-0115415157637671751");
    auto p = get_end_time_ticks() - t;
    std::stringstream s0;
    s0 << std::oct << v;
    return test_check("cons 10", p, "-115415157637671751", s0);
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_construct_11() -> bool {
    bool res = true;

    auto t = get_start_time_ticks();
    try {
        c8::integer v("-01185415157637671751");
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 11", p, "failed to throw exception", false);
    } catch (const c8::invalid_argument &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 11", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 11", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Add two positive values.
 */
auto test_add_0() -> bool {
    c8::integer a0("31");
    c8::integer a1("42");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 0", p, "73", s);
}

/*
 * Add a positive and a negative value.
 */
auto test_add_1() -> bool {
    c8::integer a0("42");
    c8::integer a1("-21");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 1", p, "21", s);
}

/*
 * Add a positive and a negative value that gives a negative result
 */
auto test_add_2() -> bool {
    c8::integer a0("12345678");
    c8::integer a1("-34738957485741895748957485743809574812345678");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 2", p, "-34738957485741895748957485743809574800000000", s);
}

/*
 * Add two very large negative values.
 */
auto test_add_3() -> bool {
    c8::integer a0("-10000000000000000000000000000000000000000000000000000000000000000008789");
    c8::integer a1("-88888880000000000000000000000000000000000000000000000000000000999992000");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 3", p, "-98888880000000000000000000000000000000000000000000000000000001000000789", s);
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_subtract_0() -> bool {
    c8::integer s0(52);
    c8::integer s1(2);
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 0", p, "50", s);
}

/*
 * Subtract a large negative value from another large negative value.
 */
auto test_subtract_1() -> bool {
    c8::integer s0("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::integer s1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 1", p, "-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143", s);
}

/*
 * Subtract a large negative value from a large value.
 */
auto test_subtract_2() -> bool {
    c8::integer s0("10000000000000000000000000000000000000000000000000000000000000000000000");
    c8::integer s1("-10000000000000000000000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 2", p, "20000000000000000000000000000000000000000000000000000000000000000000000", s);
}

/*
 * Subtract a large value from a smaller one.
 */
auto test_subtract_3() -> bool {
    c8::integer s0(2);
    c8::integer s1(52);
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 3", p, "-50", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0a() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0a", p, "0", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0b() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0b", p, "1", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0c() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0c", p, "1", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0d() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0d", p, "1", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0e() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0e", p, "0", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0f() -> bool {
    c8::integer co0(2);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0f", p, "0", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1a() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1a", p, "0", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1b() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1b", p, "1", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1c() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1c", p, "0", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1d() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1d", p, "0", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1e() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1e", p, "1", s);
}

/*
 * Compare a negative value with a positive one.
 */
auto test_compare_1f() -> bool {
    c8::integer co0(-0x987654321LL);
    c8::integer co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1f", p, "1", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2a() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2a", p, "0", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2b() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2b", p, "1", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2c() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2c", p, "1", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2d() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2d", p, "1", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2e() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2e", p, "0", s);
}

/*
 * Compare a positive value with a negative one.
 */
auto test_compare_2f() -> bool {
    c8::integer co0(1);
    c8::integer co1(-0x987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2f", p, "0", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3a() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3a", p, "1", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3b() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3b", p, "0", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3c() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3c", p, "0", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3d() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3d", p, "1", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3e() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3e", p, "0", s);
}

/*
 * Compare two negative values.
 */
auto test_compare_3f() -> bool {
    c8::integer co0(-0x2f987654321LL);
    c8::integer co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3f", p, "1", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_0() -> bool {
    c8::integer l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 0;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 0a", p, "349f", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_1() -> bool {
    c8::integer l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 0b", p, "693e", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_2() -> bool {
    c8::integer l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 18;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 0c", p, "d27c0000", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_3() -> bool {
    c8::integer l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 187;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 0d", p, "1a4f80000000000000000000000000000000000000000000000", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_4() -> bool {
    c8::integer l0("-0x349f298375323985afbce9837928798789dffeffee987678687678676756562");
    auto t = get_start_time_ticks();
    auto l1 = l0 << 69;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 1a", p, "-693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_0() -> bool {
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 0;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 0a", p, "23490000000000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_1() -> bool {
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 0b", p, "11a48000000000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_2() -> bool {
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 19;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 0c", p, "469200000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_3() -> bool {
    c8::integer r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 197;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 0d", p, "11a4800", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_4() -> bool {
    c8::integer r0("-0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 123;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 1a", p, "-d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_0() -> bool {
    c8::integer mu0(3);
    c8::integer mu1(22);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 0", p, "66", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_1() -> bool {
    c8::integer mu0(1000000000000000000LL);
    c8::integer mu1("-9999999999999999999");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 1", p, "-9999999999999999999000000000000000000", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_2() -> bool {
    c8::integer mu0(-0x2000000000000000LL);
    c8::integer mu1(0x4000000000000000LL);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << mu2;
    return test_check("mul 2", p, "-8000000000000000000000000000000", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_3() -> bool {
    c8::integer mu0("-12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::integer mu1("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 3", p, "15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100", s);
}

/*
 * Test division.
 */
auto test_divide_0() -> bool {
    c8::integer d0(1000000000000000000LL);
    c8::integer d1(99999999999999999LL);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2 << ',' << mo2;
    return test_check("div 0", p, "10,10", s);
}

/*
 * Test division.
 */
auto test_divide_1() -> bool {
    c8::integer d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::integer d1(-99999999999999999LL);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2 << ',' << mo2;
    return test_check("div 1", p, "-78292387927518758972102054472775487212767983201652300846,35600667362958008", s);
}

/*
 * Test division.
 */
auto test_divide_2() -> bool {
    c8::integer d0("-0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::integer d1("0x10000000000000001000000000000000100000000");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2 << ',' << mo2;
    return test_check("div 2", p, "-ffffffffffffffff000000000000000,100000000000000000000000", s);
}

/*
 * Test division.
 */
auto test_divide_3() -> bool {
    bool res = true;

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::integer d0(2000);
    c8::integer d1(0);
    auto t = get_start_time_ticks();
    try {
        auto d2 = d0 / d1;
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << d2;
        res &= test_nocheck("div 3", p, "failed to throw exception", false);
    } catch (const c8::divide_by_zero &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("div 3", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("div 3", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test division.
 */
auto test_divide_4() -> bool {
    c8::integer d0(-1000000000000000000LL);
    c8::integer d1(-99999999999999999LL);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2 << ',' << mo2;
    return test_check("div 4a", p, "10,10", s);
}

/*
 * Test to_long_long functionality.
 */
auto test_to_long_long_0() -> bool {
    c8::integer i(0);
    auto t = get_start_time_ticks();
    long long l = to_long_long(i);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << l;
    return test_check("toll 0", p, "0", s);
}

/*
 * Test to_long_long functionality.
 */
auto test_to_long_long_1() -> bool {
    c8::integer i(-3000);
    auto t = get_start_time_ticks();
    long long l = to_long_long(i);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << l;
    return test_check("toll 1", p, "-3000", s);
}

/*
 * Test to_long_long functionality.
 */
auto test_to_long_long_2() -> bool {
    bool res = true;

    c8::integer i("47895748574857485728747548237543205782573485472759047548275024574207");
    auto t = get_start_time_ticks();
    try {
        long long l = to_long_long(i);
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << l;
        res &= test_nocheck("toll 2", p, "failed to throw exception", false);
    } catch (const c8::overflow_error &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toll 2", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toll 2", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test to_long_long functionality.
 */
auto test_to_long_long_3() -> bool {
    c8::integer i(-12345678987654321LL);
    auto t = get_start_time_ticks();
    long long l = to_long_long(i);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << l;
    return test_check("toll 3", p, "-12345678987654321", s);
}

/*
 * Test to_long_long functionality.
 */
auto test_to_long_long_4() -> bool {
    bool res = true;

    /*
     * Construct an integer that is one bit too large to be able to convert.
     */
    c8::integer i0(1);
    c8::integer i1 = i0 << ((sizeof(unsigned long long) * 8) - 1);
    auto t = get_start_time_ticks();
    try {
        long long l = to_long_long(i1);
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << l;
        res &= test_nocheck("toll 4", p, "failed to throw exception", false);
    } catch (const c8::overflow_error &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toll 4", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toll 4", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test printing.
 */
auto test_print_0() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 0", p, "-4701397401952099592073", s);
}

/*
 * Test printing.
 */
auto test_print_1() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 1", p, "-fedcfedc0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_2() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 2", p, "-FEDCFEDC0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_3() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 3", p, "-775563766700044321263611", s);
}

/*
 * Test printing.
 */
auto test_print_4() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 4", p, "-4701397401952099592073", s);
}

/*
 * Test printing.
 */
auto test_print_5() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 5", p, "-0xfedcfedc0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_6() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 6", p, "-0XFEDCFEDC0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_7() -> bool {
    c8::integer v("-0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 7", p, "-0775563766700044321263611", s);
}

/*
 * Report the usage for this test program.
 */
static auto usage(const char *name) -> void {
    std::cerr << "usage: " << name << " [OPTIONS]\n\n";
    std::cerr << "Options\n";
    std::cerr << "  -v  Verbose reporting (optional)\n\n";
}

/*
 * Entry point.
 */
auto main(int argc, char **argv) -> int {
    /*
     * Parse the command line options.
     */
    int ch;
    while ((ch = getopt(argc, argv, "v?")) != -1) {
        switch (ch) {
        case 'v':
            verbose = true;
            break;

        case '?':
            usage(argv[0]);
            exit(-1);
        }
    }

    bool res = true;

    res &= test_construct_0();
    res &= test_construct_1();
    res &= test_construct_2();
    res &= test_construct_3();
    res &= test_construct_4();
    res &= test_construct_5();
    res &= test_construct_6();
    res &= test_construct_7();
    res &= test_construct_8();
    res &= test_construct_9();
    res &= test_construct_10();
    res &= test_construct_11();
    res &= test_add_0();
    res &= test_add_1();
    res &= test_add_2();
    res &= test_add_3();
    res &= test_subtract_0();
    res &= test_subtract_1();
    res &= test_subtract_2();
    res &= test_subtract_3();
    res &= test_compare_0a();
    res &= test_compare_0b();
    res &= test_compare_0c();
    res &= test_compare_0d();
    res &= test_compare_0e();
    res &= test_compare_0f();
    res &= test_compare_1a();
    res &= test_compare_1b();
    res &= test_compare_1c();
    res &= test_compare_1d();
    res &= test_compare_1e();
    res &= test_compare_1f();
    res &= test_compare_2a();
    res &= test_compare_2b();
    res &= test_compare_2c();
    res &= test_compare_2d();
    res &= test_compare_2e();
    res &= test_compare_2f();
    res &= test_compare_3a();
    res &= test_compare_3b();
    res &= test_compare_3c();
    res &= test_compare_3d();
    res &= test_compare_3e();
    res &= test_compare_3f();
    res &= test_lshift_0();
    res &= test_lshift_1();
    res &= test_lshift_2();
    res &= test_lshift_3();
    res &= test_lshift_4();
    res &= test_rshift_0();
    res &= test_rshift_1();
    res &= test_rshift_2();
    res &= test_rshift_3();
    res &= test_rshift_4();
    res &= test_multiply_0();
    res &= test_multiply_1();
    res &= test_multiply_2();
    res &= test_multiply_2();
    res &= test_divide_0();
    res &= test_divide_1();
    res &= test_divide_2();
    res &= test_divide_3();
    res &= test_divide_4();
    res &= test_to_long_long_0();
    res &= test_to_long_long_1();
    res &= test_to_long_long_2();
    res &= test_to_long_long_3();
    res &= test_to_long_long_4();
    res &= test_print_0();
    res &= test_print_1();
    res &= test_print_2();
    res &= test_print_3();
    res &= test_print_4();
    res &= test_print_5();
    res &= test_print_6();
    res &= test_print_7();

    if (!res) {
        std::cout << "TESTS FAILED!\n";
        exit(-1);
    }

    std::cout << "All tests passed\n";
    return 0;
}

