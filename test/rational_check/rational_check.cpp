/*
 * rational_check.c
 */
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <unistd.h>

#include <rational.h>

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
 * Test Constructors.
 */
auto test_construct_0() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(0, 1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 0", p, "0/1", s);
}

/*
 * Test Constructors.
 */
auto test_construct_1() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(1024, 384);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 1", p, "8/3", s);
}

/*
 * Test Constructors.
 */
auto test_construct_2() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(-1313, 39);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 2", p, "-101/3", s);
}

/*
 * Test Constructors.
 */
auto test_construct_3() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v("-99999187951591759827592578257927598275980/25");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 3", p, "-19999837590318351965518515651585519655196/5", s);
}

/*
 * Test Constructors.
 */
auto test_construct_4() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v("2/5154875894574578457805710875418754097512875120572105234652346059");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 4", p, "2/5154875894574578457805710875418754097512875120572105234652346059", s);
}

/*
 * Test Constructors.
 */
auto test_construct_5() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v("0x2/0x2000000000000000000000000000000000000000000000000000000000000000");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << std::showbase << v;
    return test_check("cons 5", p, "0x1/0x1000000000000000000000000000000000000000000000000000000000000000", s);
}

/*
 * Attempt to construct with an invalid octal string.
 */
auto test_construct_6() -> bool {
    bool res = true;

    auto t = get_start_time_ticks();
    try {
        c8::rational v("2323/01185415157637671751");
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
 * Test Constructors.
 */
auto test_construct_7() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(1.125);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 7", p, "9/8", s);
}

/*
 * Test Constructors.
 */
auto test_construct_8() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(-1.0/1048576);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << v;
    return test_check("cons 8", p, "-1/1048576", s);
}

/*
 * Attempt to construct with an invalid rational using a double precision infinity.
 */
auto test_construct_9() -> bool {
    bool res = true;

    auto t = get_start_time_ticks();
    try {
        c8::rational v(std::numeric_limits<double>::quiet_NaN());
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 9", p, "failed to throw exception", false);
    } catch (const c8::not_a_number &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 9", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("cons 9", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Construct a rational using an imprecise double.
 */
auto test_construct_10() -> bool {
    auto t = get_start_time_ticks();
    c8::rational v(0.1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 10", p, "ccccccccccccd/80000000000000", s);
}

/*
 * Add two positive values.
 */
auto test_add_0() -> bool {
    c8::rational a0("31/3");
    c8::rational a1("42/3");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 0", p, "73/3", s);
}

/*
 * Add a positive and a negative value.
 */
auto test_add_1() -> bool {
    c8::rational a0("42/13");
    c8::rational a1("-1/2");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 1", p, "71/26", s);
}

/*
 * Add a positive and a negative value that gives a negative result
 */
auto test_add_2() -> bool {
    c8::rational a0("12345678/287923");
    c8::rational a1("-34738957485741895748957485743809574812345678/287923");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 2", p, "-34738957485741895748957485743809574800000000/287923", s);
}

/*
 * Add two very large negative values.
 */
auto test_add_3() -> bool {
    c8::rational a0("-10000000000000000000000000000000000000000000000000000000000000000008789/3");
    c8::rational a1("-88888880000000000000000000000000000000000000000000000000000000999992000/31");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 3", p, "-192222213333333333333333333333333333333333333333333333333333334333416153/31", s);
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_subtract_0() -> bool {
    c8::rational s0(52, 3);
    c8::rational s1(2, 4);
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 0", p, "101/6", s);
}

/*
 * Subtract a large negative value from another large negative value.
 */
auto test_subtract_1() -> bool {
    c8::rational s0("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897/3");
    c8::rational s1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754/3");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 1", p, "-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143/3", s);
}

/*
 * Subtract a large negative value from a large value.
 */
auto test_subtract_2() -> bool {
    c8::rational s0("10000000000000000000000000000000000000000000000000000000000000000000000/707");
    c8::rational s1("-10000000000000000000000000000000000000000000000000000000000000000000000/7");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 2", p, "1020000000000000000000000000000000000000000000000000000000000000000000000/707", s);
}

/*
 * Subtract a large value from a smaller one.
 */
auto test_subtract_3() -> bool {
    c8::rational s0(2, 31459);
    c8::rational s1(52, 31459);
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 3", p, "-50/31459", s);
}

/*
 * Compare two positive values.
 */
auto test_compare_0a() -> bool {
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(2, 303);
    c8::rational co1(1, 303);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(-0x987654321LL, 2787539);
    c8::rational co1(1, 1);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(1, 32787158);
    c8::rational co1(-0x987654321LL, 3);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
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
    c8::rational co0(-0x2f987654321LL);
    c8::rational co1(-0x2f987654321LL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3f", p, "1", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_0() -> bool {
    c8::rational mu0(1, 50);
    c8::rational mu1(1, 25);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 0", p, "1/1250", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_1() -> bool {
    c8::rational mu0(1000000000000000000LL, 7);
    c8::rational mu1("-9999999999999999999/999");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 1", p, "-1111111111111111111000000000000000000/777", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_2() -> bool {
    c8::rational mu0(-0x3000000000000000LL, 1);
    c8::rational mu1(0x4000000000000000LL, 3);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << mu2;
    return test_check("mul 2", p, "-4000000000000000000000000000000/1", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_3() -> bool {
    c8::rational mu0("-12345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    c8::rational mu1("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 3", p, "15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100/169", s);
}

/*
 * Test division.
 */
auto test_divide_0() -> bool {
    c8::rational d0(1000000000000000000LL,7);
    c8::rational d1(99999999999999999LL,7);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2;
    return test_check("div 0", p, "1000000000000000000/99999999999999999", s);
}

auto test_divide_1() -> bool {
    c8::rational d0("7829238792751875818917817519758789749174743847389742871867617465710657162/39");
    c8::rational d1(-99999999999999999LL, 39);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2;
    return test_check("div 1", p, "-7829238792751875818917817519758789749174743847389742871867617465710657162/99999999999999999", s);
}

auto test_divide_2() -> bool {
    c8::rational d0("-0x100000000000000000000000000000000000000000000000000000000000000000000000/2323978529");
    c8::rational d1("0x10000000000000001000000000000000100000000/0x17");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2;
    return test_check("div 2", p, "-17000000000000000000000000000000000000000000000000000000000000000/8a851921000000008a851921000000008a851921", s);
}

auto test_divide_3() -> bool {
    bool res = true;

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::rational d0(2000,7);
    c8::rational d1(0,1);
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

auto test_divide_4() -> bool {
    c8::rational d0(-1000000000000000000LL,23923758279);
    c8::rational d1(-99999999999999999LL,28279753);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2;
    return test_check("div 4", p, "28279753000000000000000000/2392375827899999976076241721", s);
}

/*
 * Test to_double functionality.
 */
auto test_to_double_0() -> bool {
    c8::rational r(0, 1);
    auto t = get_start_time_ticks();
    double d = to_double(r);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d;
    return test_check("todouble 0", p, "0", s);
}

/*
 * Test to_double functionality.
 */
auto test_to_double_1() -> bool {
    c8::rational r(-3000, 59);
    auto t = get_start_time_ticks();
    double d = to_double(r);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d;
    return test_check("todouble 1", p, "-50.8475", s);
}

/*
 * Test to_double functionality.
 */
auto test_to_double_2() -> bool {
    c8::rational r2("47895748574857485728747548237543205782573485472759047548275024574207/389275892758257298");
    auto t = get_start_time_ticks();
    double d = to_double(r2);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d;
    return test_check("todouble 2", p, "1.23038e+50", s);
}

/*
 * Test to_double functionality.
 */
auto test_to_double_3() -> bool {
    c8::rational r(0.1);
    auto t = get_start_time_ticks();
    double d = to_double(r);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d;
    return test_check("todouble 3", p, "0.1", s);
}

/*
 * Test to_double functionality.
 */
auto test_to_double_4() -> bool {
    bool res = true;

    c8::integer i4 = 1;
    c8::rational r4(i4 << 2048, c8::natural(1));
    auto t = get_start_time_ticks();
    try {
        double d4 = to_double(r4);
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << d4;
        res &= test_nocheck("todouble 4", p, "failed to throw exception", false);
    } catch (const c8::overflow_error &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("todouble 4", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("todouble 4", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test to_parts functionality.
 */
auto test_to_parts_0() -> bool {
    c8::rational r(0, 1);
    auto t = get_start_time_ticks();
    auto x = to_parts(r);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << x.first << ',' << x.second;
    return test_check("toparts 0", p, "0,1", s);
}

/*
 * Test to_parts functionality.
 */
auto test_to_parts_1() -> bool {
    c8::rational r(-3000, 58);
    auto t = get_start_time_ticks();
    auto x = to_parts(r);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << x.first << ',' << x.second;
    return test_check("toparts 1", p, "-1500,29", s);
}

/*
 * Test printing.
 */
auto test_print_0() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 0", p, "-4701397401952099592073/65689", s);
}

/*
 * Test printing.
 */
auto test_print_1() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 1", p, "-fedcfedc0123456789/10099", s);
}

/*
 * Test printing.
 */
auto test_print_2() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 2", p, "-FEDCFEDC0123456789/10099", s);
}

/*
 * Test printing.
 */
auto test_print_3() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 3", p, "-775563766700044321263611/200231", s);
}

/*
 * Test printing.
 */
auto test_print_4() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 4", p, "-4701397401952099592073/65689", s);
}

/*
 * Test printing.
 */
auto test_print_5() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 5", p, "-0xfedcfedc0123456789/0x10099", s);
}

/*
 * Test printing.
 */
auto test_print_6() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 6", p, "-0XFEDCFEDC0123456789/0X10099", s);
}

/*
 * Test printing.
 */
auto test_print_7() -> bool {
    c8::rational v("-0xfedcfedc0123456789/65689");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 7", p, "-0775563766700044321263611/0200231", s);
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
    res &= test_multiply_0();
    res &= test_multiply_1();
    res &= test_multiply_2();
    res &= test_multiply_3();
    res &= test_divide_0();
    res &= test_divide_1();
    res &= test_divide_2();
    res &= test_divide_3();
    res &= test_divide_4();
    res &= test_to_double_0();
    res &= test_to_double_1();
    res &= test_to_double_2();
    res &= test_to_double_3();
    res &= test_to_double_4();
    res &= test_to_parts_0();
    res &= test_to_parts_1();
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

