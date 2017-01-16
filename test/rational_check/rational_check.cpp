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
auto test_construct() -> bool {
    bool res = true;

    /*
     * Construct with long integers.
     */
    auto t0 = get_start_time_ticks();
    c8::rational v0(0, 1);
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << v0;
    res &= test_check("cons 0", p0, "0/1", s0);

    auto t1 = get_start_time_ticks();
    c8::rational v1(1024, 384);
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << v1;
    res &= test_check("cons 1", p1, "8/3", s1);

    auto t2 = get_start_time_ticks();
    c8::rational v2(-1313, 39);
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << v2;
    res &= test_check("cons 2", p2, "-101/3", s2);

    auto t3 = get_start_time_ticks();
    c8::rational v3("-99999187951591759827592578257927598275980/25");
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << v3;
    res &= test_check("cons 3", p3, "-19999837590318351965518515651585519655196/5", s3);

    auto t4 = get_start_time_ticks();
    c8::rational v4("2/5154875894574578457805710875418754097512875120572105234652346059");
    auto p4 = get_end_time_ticks() - t4;
    std::stringstream s4;
    s4 << v4;
    res &= test_check("cons 4", p4, "2/5154875894574578457805710875418754097512875120572105234652346059", s4);

    auto t5 = get_start_time_ticks();
    c8::rational v5("0x2/0x2000000000000000000000000000000000000000000000000000000000000000");
    auto p5 = get_end_time_ticks() - t5;
    std::stringstream s5;
    s5 << std::hex << std::showbase << v5;
    res &= test_check("cons 5", p5, "0x1/0x1000000000000000000000000000000000000000000000000000000000000000", s5);

    /*
     * Attempt to construct with an invalid octal string.
     */
    auto t6 = get_start_time_ticks();
    try {
        c8::rational v6("2323/01185415157637671751");
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "failed to throw exception", false);
    } catch (const c8::invalid_argument &e) {
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "unexpected exception thrown", false);
    }

    auto t7 = get_start_time_ticks();
    c8::rational v7(1.125);
    auto p7 = get_end_time_ticks() - t7;
    std::stringstream s7;
    s7 << v7;
    res &= test_check("cons 7", p7, "9/8", s7);

    auto t8 = get_start_time_ticks();
    c8::rational v8(-1.0/1048576);
    auto p8 = get_end_time_ticks() - t8;
    std::stringstream s8;
    s8 << v8;
    res &= test_check("cons 8", p8, "-1/1048576", s8);

    /*
     * Attempt to construct with an invalid rational using a double precision infinity.
     */
    auto t9 = get_start_time_ticks();
    try {
        c8::rational v6(std::numeric_limits<double>::quiet_NaN());
        auto p9 = get_end_time_ticks() - t9;
        res &= test_nocheck("cons 9", p9, "failed to throw exception", false);
    } catch (const c8::not_a_number &e) {
        auto p9 = get_end_time_ticks() - t9;
        res &= test_nocheck("cons 9", p9, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p9 = get_end_time_ticks() - t9;
        res &= test_nocheck("cons 9", p9, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test addition.
 */
auto test_add() -> bool {
    bool res = true;

    /*
     * Add two positive values.
     */
    c8::rational a0_0("31/3");
    c8::rational a1_0("42/3");
    auto t0 = get_start_time_ticks();
    auto a2_0 = a0_0 + a1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << a2_0;
    res &= test_check("add 0", p0, "73/3", s0);

    /*
     * Add a positive and a negative value.
     */
    c8::rational a0_1("42/13");
    c8::rational a1_1("-1/2");
    auto t1 = get_start_time_ticks();
    auto a2_1 = a0_1 + a1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << a2_1;
    res &= test_check("add 1", p1, "71/26", s1);

    /*
     * Add a positive and a negative value that gives a negative result
     */
    c8::rational a0_2("12345678/287923");
    c8::rational a1_2("-34738957485741895748957485743809574812345678/287923");
    auto t2 = get_start_time_ticks();
    auto a2_2 = a0_2 + a1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << a2_2;
    res &= test_check("add 2", p2, "-34738957485741895748957485743809574800000000/287923", s2);

    /*
     * Add two very large negative values.
     */
    c8::rational a0_3("-10000000000000000000000000000000000000000000000000000000000000000008789/3");
    c8::rational a1_3("-88888880000000000000000000000000000000000000000000000000000000999992000/31");
    auto t3 = get_start_time_ticks();
    auto a2_3 = a0_3 + a1_3;
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << a2_3;
    res &= test_check("add 3", p3, "-192222213333333333333333333333333333333333333333333333333333334333416153/31", s3);

    return res;
}

/*
 * Test subtraction.
 */
auto test_subtract() -> bool {
    bool res = true;

    /*
     * Subtract a 1 digit value from another 1 digit value.
     */
    c8::rational s0_0(52, 3);
    c8::rational s1_0(2, 4);
    auto t0 = get_start_time_ticks();
    auto s2_0 = s0_0 - s1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << s2_0;
    res &= test_check("sub 0", p0, "101/6", s0);

    /*
     * Subtract a large negative value from another large negative value.
     */
    c8::rational s0_1("-5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897/3");
    c8::rational s1_1("-842758978027689671615847509157087514875097509475029454785478748571507457514754190754/3");
    auto t1 = get_start_time_ticks();
    auto s2_1 = s0_1 - s1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << s2_1;
    res &= test_check("sub 1", p1, "-5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143/3", s1);

    /*
     * Subtract a large negative value from a large value.
     */
    c8::rational s0_2("10000000000000000000000000000000000000000000000000000000000000000000000/707");
    c8::rational s1_2("-10000000000000000000000000000000000000000000000000000000000000000000000/7");
    auto t2 = get_start_time_ticks();
    auto s2_2 = s0_2 - s1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << s2_2;
    res &= test_check("sub 2", p2, "1020000000000000000000000000000000000000000000000000000000000000000000000/707", s2);

    /*
     * Subtract a large value from a smaller one.
     */
    c8::rational s0_3(2, 31459);
    c8::rational s1_3(52, 31459);
    auto t3 = get_start_time_ticks();
    auto s2_3 = s0_3 - s1_3;
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << s2_3;
    res &= test_check("sub 3", p3, "-50/31459", s3);

    return res;
}

/*
 * Test value comparisons.
 */
auto test_compare() -> bool {
    bool res = true;

    /*
     * Compare two positive values.
     */
    c8::rational co0_0(2, 303);
    c8::rational co1_0(1, 303);
    auto t0a = get_start_time_ticks();
    auto co2_0a = (co0_0 == co1_0);
    auto p0a = get_end_time_ticks() - t0a;
    std::stringstream s0a;
    s0a << co2_0a;
    res &= test_check("comp 0a", p0a, "0", s0a);

    auto t0b = get_start_time_ticks();
    auto co2_0b = (co0_0 != co1_0);
    auto p0b = get_end_time_ticks() - t0b;
    std::stringstream s0b;
    s0b << co2_0b;
    res &= test_check("comp 0b", p0b, "1", s0b);

    auto t0c = get_start_time_ticks();
    auto co2_0c = (co0_0 > co1_0);
    auto p0c = get_end_time_ticks() - t0c;
    std::stringstream s0c;
    s0c << co2_0c;
    res &= test_check("comp 0c", p0c, "1", s0c);

    auto t0d = get_start_time_ticks();
    auto co2_0d = (co0_0 >= co1_0);
    auto p0d = get_end_time_ticks() - t0d;
    std::stringstream s0d;
    s0d << co2_0d;
    res &= test_check("comp 0d", p0d, "1", s0d);

    auto t0e = get_start_time_ticks();
    auto co2_0e = (co0_0 < co1_0);
    auto p0e = get_end_time_ticks() - t0e;
    std::stringstream s0e;
    s0e << co2_0e;
    res &= test_check("comp 0e", p0e, "0", s0e);

    auto t0f = get_start_time_ticks();
    auto co2_0f = (co0_0 <= co1_0);
    auto p0f = get_end_time_ticks() - t0f;
    std::stringstream s0f;
    s0f << co2_0f;
    res &= test_check("comp 0f", p0f, "0", s0f);

    /*
     * Compare a negative value with a positive one.
     */
    c8::rational co0_1(-0x987654321LL, 2787539);
    c8::rational co1_1(1, 1);
    auto t1a = get_start_time_ticks();
    auto co2_1a = (co0_1 == co1_1);
    auto p1a = get_end_time_ticks() - t1a;
    std::stringstream s1a;
    s1a << co2_1a;
    res &= test_check("comp 1a", p1a, "0", s1a);

    auto t1b = get_start_time_ticks();
    auto co2_1b = (co0_1 != co1_1);
    auto p1b = get_end_time_ticks() - t1b;
    std::stringstream s1b;
    s1b << co2_1b;
    res &= test_check("comp 1b", p1b, "1", s1b);

    auto t1c = get_start_time_ticks();
    auto co2_1c = (co0_1 > co1_1);
    auto p1c = get_end_time_ticks() - t1c;
    std::stringstream s1c;
    s1c << co2_1c;
    res &= test_check("comp 1c", p1c, "0", s1c);

    auto t1d = get_start_time_ticks();
    auto co2_1d = (co0_1 >= co1_1);
    auto p1d = get_end_time_ticks() - t1d;
    std::stringstream s1d;
    s1d << co2_1d;
    res &= test_check("comp 1d", p1d, "0", s1d);

    auto t1e = get_start_time_ticks();
    auto co2_1e = (co0_1 < co1_1);
    auto p1e = get_end_time_ticks() - t1e;
    std::stringstream s1e;
    s1e << co2_1e;
    res &= test_check("comp 1e", p1e, "1", s1e);

    auto t1f = get_start_time_ticks();
    auto co2_1f = (co0_1 <= co1_1);
    auto p1f = get_end_time_ticks() - t1f;
    std::stringstream s1f;
    s1f << co2_1f;
    res &= test_check("comp 1f", p1f, "1", s1f);

    /*
     * Compare a positive value with a negative one.
     */
    c8::rational co0_2(1, 32787158);
    c8::rational co1_2(-0x987654321LL, 3);
    auto t2a = get_start_time_ticks();
    auto co2_2a = (co0_2 == co1_2);
    auto p2a = get_end_time_ticks() - t2a;
    std::stringstream s2a;
    s2a << co2_2a;
    res &= test_check("comp 2a", p2a, "0", s2a);

    auto t2b = get_start_time_ticks();
    auto co2_2b = (co0_2 != co1_2);
    auto p2b = get_end_time_ticks() - t2b;
    std::stringstream s2b;
    s2b << co2_2b;
    res &= test_check("comp 2b", p2b, "1", s2b);

    auto t2c = get_start_time_ticks();
    auto co2_2c = (co0_2 > co1_2);
    auto p2c = get_end_time_ticks() - t2c;
    std::stringstream s2c;
    s2c << co2_2c;
    res &= test_check("comp 2c", p2c, "1", s2c);

    auto t2d = get_start_time_ticks();
    auto co2_2d = (co0_2 >= co1_2);
    auto p2d = get_end_time_ticks() - t2d;
    std::stringstream s2d;
    s2d << co2_2d;
    res &= test_check("comp 2d", p2d, "1", s2d);

    auto t2e = get_start_time_ticks();
    auto co2_2e = (co0_2 < co1_2);
    auto p2e = get_end_time_ticks() - t2e;
    std::stringstream s2e;
    s2e << co2_2e;
    res &= test_check("comp 2e", p2e, "0", s2e);

    auto t2f = get_start_time_ticks();
    auto co2_2f = (co0_2 <= co1_2);
    auto p2f = get_end_time_ticks() - t2f;
    std::stringstream s2f;
    s2f << co2_2f;
    res &= test_check("comp 2f", p2f, "0", s2f);

    /*
     * Compare two negative values.
     */
    c8::rational co0_3(-0x2f987654321LL);
    c8::rational co1_3(-0x2f987654321LL);
    auto t3a = get_start_time_ticks();
    auto co2_3a = (co0_3 == co1_3);
    auto p3a = get_end_time_ticks() - t3a;
    std::stringstream s3a;
    s3a << co2_3a;
    res &= test_check("comp 3a", p3a, "1", s3a);

    auto t3b = get_start_time_ticks();
    auto co2_3b = (co0_3 != co1_3);
    auto p3b = get_end_time_ticks() - t3b;
    std::stringstream s3b;
    s3b << co2_3b;
    res &= test_check("comp 3b", p3b, "0", s3b);

    auto t3c = get_start_time_ticks();
    auto co2_3c = (co0_3 > co1_3);
    auto p3c = get_end_time_ticks() - t3c;
    std::stringstream s3c;
    s3c << co2_3c;
    res &= test_check("comp 3c", p3c, "0", s3c);

    auto t3d = get_start_time_ticks();
    auto co2_3d = (co0_3 >= co1_3);
    auto p3d = get_end_time_ticks() - t3d;
    std::stringstream s3d;
    s3d << co2_3d;
    res &= test_check("comp 3d", p3d, "1", s3d);

    auto t3e = get_start_time_ticks();
    auto co2_3e = (co0_3 < co1_3);
    auto p3e = get_end_time_ticks() - t3e;
    std::stringstream s3e;
    s3e << co2_3e;
    res &= test_check("comp 3e", p3e, "0", s3e);

    auto t3f = get_start_time_ticks();
    auto co2_3f = (co0_3 <= co1_3);
    auto p3f = get_end_time_ticks() - t3f;
    std::stringstream s3f;
    s3f << co2_3f;
    res &= test_check("comp 3f", p3f, "1", s3f);

    return res;
}

/*
 * Test multiplication.
 */
auto test_multiply() -> bool {
    bool res = true;

    c8::rational mu0_0(1, 50);
    c8::rational mu1_0(1, 25);
    auto t0 = get_start_time_ticks();
    auto mu2_0 = mu0_0 * mu1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << mu2_0;
    res &= test_check("mul 0", p0, "1/1250", s0);

    c8::rational mu0_1(1000000000000000000LL, 7);
    c8::rational mu1_1("-9999999999999999999/999");
    auto t1 = get_start_time_ticks();
    auto mu2_1 = mu0_1 * mu1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << mu2_1;
    res &= test_check("mul 1", p1, "-1111111111111111111000000000000000000/777", s1);

    c8::rational mu0_2(-0x3000000000000000LL, 1);
    c8::rational mu1_2(0x4000000000000000LL, 3);
    auto t2 = get_start_time_ticks();
    auto mu2_2 = mu0_2 * mu1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << std::hex << mu2_2;
    res &= test_check("mul 2", p2, "-4000000000000000000000000000000/1", s2);

    c8::rational mu0_3("-12345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    c8::rational mu1_3("-1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890/13");
    auto t3 = get_start_time_ticks();
    auto mu2_3 = mu0_3 * mu1_3;
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << mu2_3;
    res &= test_check("mul 3", p3, "15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100/169", s3);

    return res;
}

/*
 * Test division.
 */
auto test_divide() -> bool {
    bool res = true;

    c8::rational d0_0(1000000000000000000LL,7);
    c8::rational d1_0(99999999999999999LL,7);
    auto t0 = get_start_time_ticks();
    auto d2_0 = d0_0 / d1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << d2_0;
    res &= test_check("div 0", p0, "1000000000000000000/99999999999999999", s0);

    c8::rational d0_1("7829238792751875818917817519758789749174743847389742871867617465710657162/39");
    c8::rational d1_1(-99999999999999999LL, 39);
    auto t1 = get_start_time_ticks();
    auto d2_1 = d0_1 / d1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << d2_1;
    res &= test_check("div 1", p1, "-7829238792751875818917817519758789749174743847389742871867617465710657162/99999999999999999", s1);
    std::stringstream s1b;

    c8::rational d0_2("-0x100000000000000000000000000000000000000000000000000000000000000000000000/2323978529");
    c8::rational d1_2("0x10000000000000001000000000000000100000000/0x17");
    auto t2 = get_start_time_ticks();
    auto d2_2 = d0_2 / d1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << std::hex << d2_2;
    res &= test_check("div 2", p2, "-17000000000000000000000000000000000000000000000000000000000000000/1150a3242000000008a851921", s2);
    std::stringstream s2b;

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::rational d0_3(2000,7);
    c8::rational d1_3(0,1);
    auto t3 = get_start_time_ticks();
    try {
        auto d2_3 = d0_3 / d1_3;
        auto p3 = get_end_time_ticks() - t3;
        std::stringstream s3;
        s3 << d2_3;
        res &= test_nocheck("div 3", p3, "failed to throw exception", false);
    } catch (const c8::divide_by_zero &e) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("div 3", p3, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("div 3", p3, "unexpected exception thrown", false);
    }

    c8::rational d0_4(-1000000000000000000LL,23923758279);
    c8::rational d1_4(-99999999999999999LL,28279753);
    auto t4 = get_start_time_ticks();
    auto d2_4 = d0_4 / d1_4;
    auto p4 = get_end_time_ticks() - t4;
    std::stringstream s4;
    s4 << d2_4;
    res &= test_check("div 4", p4, "28279753000000000000000000/2392375827899999976076241721", s4);

    return res;
}

/*
 * Test printing.
 */
auto test_print() -> bool {
    bool res = true;

    return res;
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

    res &= test_construct();
    res &= test_add();
    res &= test_subtract();
    res &= test_compare();
    res &= test_multiply();
    res &= test_divide();
    res &= test_print();

    if (!res) {
        std::cout << "TESTS FAILED!\n";
        exit(-1);
    }

    std::cout << "All tests passed\n";
    return 0;
}

