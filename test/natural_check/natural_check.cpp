/*
 * natural_check.c
 */
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include <natural.h>

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
     * Construct with a long integer 0.
     */
    auto t0 = get_start_time_ticks();
    c8::natural v0(0);
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << v0;
    res &= test_check("cons 0", p0, "0", s0);

    /*
     * Construct with a long integer.
     */
    auto t1 = get_start_time_ticks();
    c8::natural v1(0x123456789abcULL);
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << std::hex << v1;
    res &= test_check("cons 1", p1, "123456789abc", s1);

    /*
     * Construct with a string 0.
     */
    auto t2 = get_start_time_ticks();
    c8::natural v2("0");
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << v2;
    res &= test_check("cons 2", p2, "0", s2);

    /*
     * Construct with a hexadecimal string.
     */
    auto t3 = get_start_time_ticks();
    c8::natural v3("0x3837439787487386792386728abcd88379dc");
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << std::hex << v3;
    res &= test_check("cons 3", p3, "3837439787487386792386728abcd88379dc", s3);

    /*
     * Construct with a decimal string.
     */
    auto t4 = get_start_time_ticks();
    c8::natural v4("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
    auto p4 = get_end_time_ticks() - t4;
    std::stringstream s4;
    s4 << v4;
    res &= test_check("cons 4", p4, "3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097", s4);

    /*
     * Construct with an octal string.
     */
    auto t5 = get_start_time_ticks();
    c8::natural v5("0115415157637671751");
    auto p5 = get_end_time_ticks() - t5;
    std::stringstream s5;
    s5 << std::oct << v5;
    res &= test_check("cons 5", p5, "115415157637671751", s5);

    /*
     * Attempt to construct with an invalid octal string.
     */
    auto t6 = get_start_time_ticks();
    try {
        c8::natural v6("01185415157637671751");
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "failed to throw exception", false);
    } catch (const std::invalid_argument &e) {
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p6 = get_end_time_ticks() - t6;
        res &= test_nocheck("cons 6", p6, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test bit counting.
 */
auto test_count_bits() -> bool {
    bool res = true;

    c8::natural ct0(0);
    std::stringstream s0;
    auto t0 = get_start_time_ticks();
    auto b0 = ct0.count_bits();
    auto p0 = get_end_time_ticks() - t0;
    s0 << b0;
    res &= test_check("count 0", p0, "0", s0);

    c8::natural ct1(0xffffffffffffffffULL);
    std::stringstream s1;
    auto t1 = get_start_time_ticks();
    auto b1 = ct1.count_bits();
    auto p1 = get_end_time_ticks() - t1;
    s1 << b1;
    res &= test_check("count 1", p1, "64", s1);

    c8::natural ct2(0x12345ULL);
    std::stringstream s2;
    auto t2 = get_start_time_ticks();
    auto b2 = ct2.count_bits();
    auto p2 = get_end_time_ticks() - t2;
    s2 << b2;
    res &= test_check("count 2", p2, "17", s2);

    c8::natural ct3("0x123456789abcdef0123456789abcdef0123456789abcdef");
    std::stringstream s3;
    auto t3 = get_start_time_ticks();
    auto b3 = ct3.count_bits();
    auto p3 = get_end_time_ticks() - t3;
    s3 << b3;
    res &= test_check("count 3", p3, "185", s3);

    return res;
}

/*
 * Test addition.
 */
auto test_add() -> bool {
    bool res = true;

    /*
     * Add a 1 digit value and a 1 digit value.
     */
    c8::natural a0_0("31");
    c8::natural a1_0("42");
    auto t0 = get_start_time_ticks();
    auto a2_0 = a0_0 + a1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << a2_0;
    res &= test_check("add 0", p0, "73", s0);

    /*
     * Add a 0 digit value and a 1 digit value.
     */
    c8::natural a0_1("0");
    c8::natural a1_1("42");
    auto t1 = get_start_time_ticks();
    auto a2_1 = a0_1 + a1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << a2_1;
    res &= test_check("add 1", p1, "42", s1);

    /*
     * Add a 2 digit value and a 1 digit value, causing an overflow to 3 digits.
     */
    c8::natural a0_2(0xffffffffffffffffULL);
    c8::natural a1_2(0x2ULL);
    auto t2 = get_start_time_ticks();
    auto a2_2 = a0_2 + a1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << std::hex << a2_2;
    res &= test_check("add 2", p2, "10000000000000001", s2);

    /*
     * Add two very large values.
     */
    c8::natural a0_3("10000000000000000000000000000000000000000000000000000000000000000008789"); 
    c8::natural a1_3("88888880000000000000000000000000000000000000000000000000000000999992000"); 
    auto t3 = get_start_time_ticks();
    auto a2_3 = a0_3 + a1_3;
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << a2_3;
    res &= test_check("add 3", p3, "98888880000000000000000000000000000000000000000000000000000001000000789", s3);

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
    c8::natural s0_0(52);
    c8::natural s1_0(2);
    auto t0 = get_start_time_ticks();
    auto s2_0 = s0_0 - s1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << s2_0;
    res &= test_check("sub 0", p0, "50", s0);

    /*
     * Subtract a large value from another large value.
     */
    c8::natural s0_1("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1_1("842758978027689671615847509157087514875097509475029454785478748571507457514754190754");
    auto t1 = get_start_time_ticks();
    auto s2_1 = s0_1 - s1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << s2_1;
    res &= test_check("sub 1", p1, "5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143", s1);

    /*
     * Subtract a large value from another large value, resulting in a very much smaller value.
     */
    c8::natural s0_2("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1_2("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690000");
    auto t2 = get_start_time_ticks();
    auto s2_2 = s0_2 - s1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << s2_2;
    res &= test_check("sub 2", p2, "897", s2);

    /*
     * Subtract a large value from a smaller one.  This will throw an exception because there
     * aren't any negative natural numbers.
     */
    c8::natural s0_3(2);
    c8::natural s1_3(52);
    auto t3 = get_start_time_ticks();
    try {
        auto s2_3 = s0_3 - s1_3;
        auto p3 = get_end_time_ticks() - t3;
        std::stringstream s3;
        s3 << s2_3;
        res &= test_nocheck("sub 3", p3, "failed to throw exception", false);
    } catch (const std::underflow_error &e) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("sub 3", p3, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("sub 3", p3, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test value comparisons.
 */
auto test_compare() -> bool {
    bool res = true;

    /*
     * Compare two dissimilar 1 digit values.
     */
    c8::natural co0_0(2);
    c8::natural co1_0(1);
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
     * Compare a 2 digit value with a 1 digit value.
     */
    c8::natural co0_1(0x987654321ULL);
    c8::natural co1_1(1);
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
    res &= test_check("comp 1c", p1c, "1", s1c);

    auto t1d = get_start_time_ticks();
    auto co2_1d = (co0_1 >= co1_1);
    auto p1d = get_end_time_ticks() - t1d;
    std::stringstream s1d;
    s1d << co2_1d;
    res &= test_check("comp 1d", p1d, "1", s1d);

    auto t1e = get_start_time_ticks();
    auto co2_1e = (co0_1 < co1_1);
    auto p1e = get_end_time_ticks() - t1e;
    std::stringstream s1e;
    s1e << co2_1e;
    res &= test_check("comp 1e", p1e, "0", s1e);

    auto t1f = get_start_time_ticks();
    auto co2_1f = (co0_1 <= co1_1);
    auto p1f = get_end_time_ticks() - t1f;
    std::stringstream s1f;
    s1f << co2_1f;
    res &= test_check("comp 1f", p1f, "0", s1f);

    /*
     * Compare a 1 digit value with a 2 digit value.
     */
    c8::natural co0_2(1);
    c8::natural co1_2(0x987654321ULL);
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
    res &= test_check("comp 2c", p2c, "0", s2c);

    auto t2d = get_start_time_ticks();
    auto co2_2d = (co0_2 >= co1_2);
    auto p2d = get_end_time_ticks() - t2d;
    std::stringstream s2d;
    s2d << co2_2d;
    res &= test_check("comp 2d", p2d, "0", s2d);

    auto t2e = get_start_time_ticks();
    auto co2_2e = (co0_2 < co1_2);
    auto p2e = get_end_time_ticks() - t2e;
    std::stringstream s2e;
    s2e << co2_2e;
    res &= test_check("comp 2e", p2e, "1", s2e);

    auto t2f = get_start_time_ticks();
    auto co2_2f = (co0_2 <= co1_2);
    auto p2f = get_end_time_ticks() - t2f;
    std::stringstream s2f;
    s2f << co2_2f;
    res &= test_check("comp 2f", p2f, "1", s2f);

    /*
     * Compare two 2 digit values.
     */
    c8::natural co0_3(0x2f987654321ULL);
    c8::natural co1_3(0x2f987654321ULL);
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
 * Test left shifting.
 */
auto test_lshift() -> bool {
    bool res = true;

    c8::natural l0_0(0x349f);
    auto t0a = get_start_time_ticks();
    auto l1_0a = l0_0 << 0;
    auto p0a = get_end_time_ticks() - t0a;
    std::stringstream s0a;
    s0a << std::hex << l1_0a;
    res &= test_check("lsh 0a", p0a, "349f", s0a);

    auto t0b = get_start_time_ticks();
    auto l1_0b = l0_0 << 1;
    auto p0b = get_end_time_ticks() - t0b;
    std::stringstream s0b;
    s0b << std::hex << l1_0b;
    res &= test_check("lsh 0b", p0b, "693e", s0b);

    auto t0c = get_start_time_ticks();
    auto l1_0c = l0_0 << 18;
    auto p0c = get_end_time_ticks() - t0c;
    std::stringstream s0c;
    s0c << std::hex << l1_0c;
    res &= test_check("lsh 0c", p0c, "d27c0000", s0c);

    auto t0d = get_start_time_ticks();
    auto l1_0d = l0_0 << 187;
    auto p0d = get_end_time_ticks() - t0d;
    std::stringstream s0d;
    s0d << std::hex << l1_0d;
    res &= test_check("lsh 0d", p0d, "1a4f80000000000000000000000000000000000000000000000", s0d);

    c8::natural l0_1("0x349f298375323985afbce9837928798789dffeffee987678687678676756562");
    auto t1a = get_start_time_ticks();
    auto l1_1a = l0_1 << 69;
    auto p1a = get_end_time_ticks() - t1a;
    std::stringstream s1a;
    s1a << std::hex << l1_1a;
    res &= test_check("lsh 1a", p1a, "693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000", s1a);

    return res;
}

/*
 * Test right shifting.
 */
auto test_rshift() -> bool {
    bool res = true;

    c8::natural r0_0("0x23490000000000000000000000000000000000000000000000000000");
    auto t0a = get_start_time_ticks();
    auto r1_0a = r0_0 >> 0;
    auto p0a = get_end_time_ticks() - t0a;
    std::stringstream s0a;
    s0a << std::hex << r1_0a;
    res &= test_check("rsh 0a", p0a, "23490000000000000000000000000000000000000000000000000000", s0a);

    auto t0b = get_start_time_ticks();
    auto r1_0b = r0_0 >> 1;
    auto p0b = get_end_time_ticks() - t0b;
    std::stringstream s0b;
    s0b << std::hex << r1_0b;
    res &= test_check("rsh 0b", p0b, "11a48000000000000000000000000000000000000000000000000000", s0b);

    auto t0c = get_start_time_ticks();
    auto r1_0c = r0_0 >> 19;
    auto p0c = get_end_time_ticks() - t0c;
    std::stringstream s0c;
    s0c << std::hex << r1_0c;
    res &= test_check("rsh 0c", p0c, "469200000000000000000000000000000000000000000000000", s0c);

    auto t0d = get_start_time_ticks();
    auto r1_0d = r0_0 >> 197;
    auto p0d = get_end_time_ticks() - t0d;
    std::stringstream s0d;
    s0d << std::hex << r1_0d;
    res &= test_check("rsh 0d", p0d, "11a4800", s0d);

    c8::natural r0_1("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    auto t1a = get_start_time_ticks();
    auto r1_1a = r0_1 >> 123;
    auto p1a = get_end_time_ticks() - t1a;
    std::stringstream s1a;
    s1a << std::hex << r1_1a;
    res &= test_check("rsh 1a", p1a, "d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a", s1a);

    return res;
}

/*
 * Test multiplication.
 */
auto test_multiply() -> bool {
    bool res = true;

    c8::natural mu0_0(3);
    c8::natural mu1_0(22);
    auto t0 = get_start_time_ticks();
    auto mu2_0 = mu0_0 * mu1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0;
    s0 << mu2_0;
    res &= test_check("mul 0", p0, "66", s0);

    c8::natural mu0_1(1000000000000000000ULL);
    c8::natural mu1_1(9999999999999999999ULL);
    auto t1 = get_start_time_ticks();
    auto mu2_1 = mu0_1 * mu1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1;
    s1 << mu2_1;
    res &= test_check("mul 1", p1, "9999999999999999999000000000000000000", s1);

    c8::natural mu0_2(0x2000000000000000ULL);
    c8::natural mu1_2(0x4000000000000000ULL);
    auto t2 = get_start_time_ticks();
    auto mu2_2 = mu0_2 * mu1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2;
    s2 << std::hex << mu2_2;
    res &= test_check("mul 2", p2, "8000000000000000000000000000000", s2);

    c8::natural mu0_3("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::natural mu1_3("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    auto t3 = get_start_time_ticks();
    auto mu2_3 = mu0_3 * mu1_3;
    auto p3 = get_end_time_ticks() - t3;
    std::stringstream s3;
    s3 << mu2_3;
    res &= test_check("mul 3", p3, "15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100", s3);

    return res;
}

/*
 * Test division.
 */
auto test_divide() -> bool {
    bool res = true;

    c8::natural d0_0(1000000000000000000ULL);
    c8::natural d1_0(99999999999999999ULL);
    auto t0 = get_start_time_ticks();
    auto d2_0 = d0_0 / d1_0;
    auto mo2_0 = d0_0 % d1_0;
    auto p0 = get_end_time_ticks() - t0;
    std::stringstream s0a;
    s0a << d2_0;
    res &= test_check("div 0a", p0, "10", s0a);
    std::stringstream s0b;
    s0b << mo2_0;
    res &= test_check("div 0b", p0, "10", s0b);

    c8::natural d0_1("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::natural d1_1(99999999999999999ULL);
    auto t1 = get_start_time_ticks();
    auto d2_1 = d0_1 / d1_1;
    auto mo2_1 = d0_1 % d1_1;
    auto p1 = get_end_time_ticks() - t1;
    std::stringstream s1a;
    s1a << d2_1;
    res &= test_check("div 1a", p1, "78292387927518758972102054472775487212767983201652300846", s1a);
    std::stringstream s1b;
    s1b << mo2_1;
    res &= test_check("div 1b", p1, "35600667362958008", s1b);

    c8::natural d0_2("0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::natural d1_2("0x10000000000000001000000000000000100000000");
    auto t2 = get_start_time_ticks();
    auto d2_2 = d0_2 / d1_2;
    auto mo2_2 = d0_2 % d1_2;
    auto p2 = get_end_time_ticks() - t2;
    std::stringstream s2a;
    s2a << std::hex << d2_2;
    res &= test_check("div 2a", p2, "ffffffffffffffff000000000000000", s2a);
    std::stringstream s2b;
    s2b << std::hex << mo2_2;
    res &= test_check("div 2b", p2, "100000000000000000000000", s2b);

    /*
     * Divide by zero.  This will throw an exception!
     */
    c8::natural d0_3(2000);
    c8::natural d1_3(0);
    auto t3 = get_start_time_ticks();
    try {
        auto d2_3 = d0_3 / d1_3;
        auto p3 = get_end_time_ticks() - t3;
        std::stringstream s3;
        s3 << d2_3;
        res &= test_nocheck("div 3", p3, "failed to throw exception", false);
    } catch (const std::logic_error &e) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("div 3", p3, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p3 = get_end_time_ticks() - t3;
        res &= test_nocheck("div 3", p3, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test printing.
 */
auto test_print() -> bool {
    bool res = true;

    c8::natural v("0xfedcfedc0123456789");

    std::stringstream s0;
    auto t0 = get_start_time_ticks();
    s0 << v;
    auto p0 = get_end_time_ticks() - t0;
    res &= test_check("prn 0", p0, "4701397401952099592073", s0);

    std::stringstream s1;
    auto t1 = get_start_time_ticks();
    s1 << std::hex << v;
    auto p1 = get_end_time_ticks() - t1;
    res &= test_check("prn 1", p1, "fedcfedc0123456789", s1);

    std::stringstream s2;
    auto t2 = get_start_time_ticks();
    s2 << std::uppercase << std::hex << v;
    auto p2 = get_end_time_ticks() - t2;
    res &= test_check("prn 2", p2, "FEDCFEDC0123456789", s2);

    std::stringstream s3;
    auto t3 = get_start_time_ticks();
    s3 << std::oct << v;
    auto p3 = get_end_time_ticks() - t3;
    res &= test_check("prn 3", p3, "775563766700044321263611", s3);

    std::stringstream s4;
    auto t4 = get_start_time_ticks();
    s4 << std::showbase << v;
    auto p4 = get_end_time_ticks() - t4;
    res &= test_check("prn 4", p4, "4701397401952099592073", s4);

    std::stringstream s5;
    auto t5 = get_start_time_ticks();
    s5 << std::showbase << std::hex << v;
    auto p5 = get_end_time_ticks() - t5;
    res &= test_check("prn 5", p5, "0xfedcfedc0123456789", s5);

    std::stringstream s6;
    auto t6 = get_start_time_ticks();
    s6 << std::showbase << std::uppercase << std::hex << v;
    auto p6 = get_end_time_ticks() - t6;
    res &= test_check("prn 6", p6, "0XFEDCFEDC0123456789", s6);

    std::stringstream s7;
    auto t7 = get_start_time_ticks();
    s7 << std::showbase << std::oct << v;
    auto p7 = get_end_time_ticks() - t7;
    res &= test_check("prn 7", p7, "0775563766700044321263611", s7);

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
    res &= test_count_bits();
    res &= test_add();
    res &= test_subtract();
    res &= test_compare();
    res &= test_lshift();
    res &= test_rshift();
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

