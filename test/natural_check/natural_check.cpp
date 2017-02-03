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
 * Construct with a long integer 0.
 */
auto test_construct_0() -> bool {
    auto t = get_start_time_ticks();
    c8::natural v(0);
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
    c8::natural v(0x123456789abcULL);
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
    c8::natural v("0");
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
    c8::natural v("0x3837439787487386792386728abcd88379dc");
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
    c8::natural v("3897894117580750151618270927682762897697428275427542907478758957487582700682675349287325097");
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
    c8::natural v("0115415157637671751");
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
        c8::natural v6("01185415157637671751");
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
 * Construct with a hexadecimal string.
 */
auto test_construct_7() -> bool {
    auto t = get_start_time_ticks();
    c8::natural v("0x100000000000000000000000");
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << v;
    return test_check("cons 7", p, "100000000000000000000000", s);
}

/*
 * Test bit counting.
 */
auto test_count_bits_0() -> bool {
    c8::natural ct(0);
    std::stringstream s;
    auto t = get_start_time_ticks();
    auto b = ct.count_bits();
    auto p = get_end_time_ticks() - t;
    s << b;
    return test_check("count 0", p, "0", s);
}

/*
 * Test bit counting.
 */
auto test_count_bits_1() -> bool {
    c8::natural ct(0xffffffffffffffffULL);
    std::stringstream s;
    auto t = get_start_time_ticks();
    auto b = ct.count_bits();
    auto p = get_end_time_ticks() - t;
    s << b;
    return test_check("count 1", p, "64", s);
}

/*
 * Test bit counting.
 */
auto test_count_bits_2() -> bool {
    c8::natural ct(0x12345ULL);
    std::stringstream s;
    auto t = get_start_time_ticks();
    auto b = ct.count_bits();
    auto p = get_end_time_ticks() - t;
    s << b;
    return test_check("count 2", p, "17", s);
}

/*
 * Test bit counting.
 */
auto test_count_bits_3() -> bool {
    c8::natural ct("0x123456789abcdef0123456789abcdef0123456789abcdef");
    std::stringstream s;
    auto t = get_start_time_ticks();
    auto b = ct.count_bits();
    auto p = get_end_time_ticks() - t;
    s << b;
    return test_check("count 3", p, "185", s);
}

/*
 * Add a 1 digit value and a 1 digit value.
 */
auto test_add_0() -> bool {
    c8::natural a0("31");
    c8::natural a1("42");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 0", p, "73", s);
}

/*
 * Add a 0 digit value and a 1 digit value.
 */
auto test_add_1() -> bool {
    c8::natural a0("0");
    c8::natural a1("42");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 1", p, "42", s);
}

/*
 * Add a 2 digit value and a 1 digit value, causing an overflow to 3 digits.
 */
auto test_add_2() -> bool {
    c8::natural a0(0xffffffffffffffffULL);
    c8::natural a1(0x2ULL);
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << a2;
    return test_check("add 2", p, "10000000000000001", s);
}

/*
 * Add two very large values.
 */
auto test_add_3() -> bool {
    c8::natural a0("10000000000000000000000000000000000000000000000000000000000000000008789");
    c8::natural a1("88888880000000000000000000000000000000000000000000000000000000999992000");
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 3", p, "98888880000000000000000000000000000000000000000000000000000001000000789", s);
}

/*
 * Add a 1 digit value and a digit.
 */
auto test_add_4() -> bool {
    c8::natural a0("13");
    c8::natural_digit a1 = 42;
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << a2;
    return test_check("add 4", p, "55", s);
}

/*
 * Add a 3 digit value and a 1 digit value, causing an overflow to 3 digits.
 */
auto test_add_5() -> bool {
    c8::natural a0("0xffffffffffffffffffffffff");
    c8::natural_digit a1 = 2;
    auto t = get_start_time_ticks();
    auto a2 = a0 + a1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << a2;
    return test_check("add 5", p, "1000000000000000000000001", s);
}

/*
 * Subtract a 1 digit value from another 1 digit value.
 */
auto test_subtract_0() -> bool {
    c8::natural s0(52);
    c8::natural s1(2);
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 0", p, "50", s);
}

/*
 * Subtract a large value from another large value.
 */
auto test_subtract_1() -> bool {
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("842758978027689671615847509157087514875097509475029454785478748571507457514754190754");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 1", p, "5872488729698595999749602411500766185722239445613509099777952305512191704320129156897500143", s);
}

/*
 * Subtract a large value from another large value, resulting in a very much smaller value.
 */
auto test_subtract_2() -> bool {
    c8::natural s0("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690897");
    c8::natural s1("5872489572457574027439274027348275342809754320711018574807407090990940275827586671651690000");
    auto t = get_start_time_ticks();
    auto s2 = s0 - s1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 2", p, "897", s);
}

/*
 * Subtract a large value from a smaller one.  This will throw an exception because there
 * aren't any negative natural numbers.
 */
auto test_subtract_3() -> bool {
    bool res = true;

    c8::natural s0(2);
    c8::natural s1(52);
    auto t = get_start_time_ticks();
    try {
        auto s2 = s0 - s1;
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << s2;
        res &= test_nocheck("sub 3", p, "failed to throw exception", false);
    } catch (const c8::not_a_number &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("sub 3", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("sub 3", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Subtract a digit from another 1 digit value.
 */
auto test_subtract_4() -> bool {
    c8::natural s0(53);
    auto t = get_start_time_ticks();
    auto s2 = s0 - 15;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 4", p, "38", s);
}

/*
 * Subtract a larger value from a smaller one.  This will throw an exception because there
 * aren't any negative natural numbers.
 */
auto test_subtract_5() -> bool {
    bool res = true;

    c8::natural s0(100);
    auto t = get_start_time_ticks();
    try {
        auto s2 = s0 - 127;
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << s2;
        res &= test_nocheck("sub 5", p, "failed to throw exception", false);
    } catch (const c8::not_a_number &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("sub 5", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("sub 5", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Subtract a digit from another 1 digit value.
 */
auto test_subtract_6() -> bool {
    c8::natural s0(0);
    auto t = get_start_time_ticks();
    auto s2 = s0 - 0;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << s2;
    return test_check("sub 6", p, "0", s);
}

/*
 * Subtract a digit from a large value.
 */
auto test_subtract_7() -> bool {
    c8::natural s0("0x1000000000000000000000000");
    auto t = get_start_time_ticks();
    auto s2 = s0 - 1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << s2;
    return test_check("sub 7", p, "ffffffffffffffffffffffff", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0a() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0a", p, "0", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0b() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0b", p, "1", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0c() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0c", p, "1", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0d() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0d", p, "1", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0e() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0e", p, "0", s);
}

/*
 * Compare two dissimilar 1 digit values.
 */
auto test_compare_0f() -> bool {
    c8::natural co0(2);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 0f", p, "0", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1a() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1a", p, "0", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1b() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1b", p, "1", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1c() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1c", p, "1", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1d() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1d", p, "1", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1e() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1e", p, "0", s);
}

/*
 * Compare a 2 digit value with a 1 digit value.
 */
auto test_compare_1f() -> bool {
    c8::natural co0(0x987654321ULL);
    c8::natural co1(1);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 1f", p, "0", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2a() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2a", p, "0", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2b() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2b", p, "1", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2c() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2c", p, "0", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2d() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2d", p, "0", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2e() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2e", p, "1", s);
}

/*
 * Compare a 1 digit value with a 2 digit value.
 */
auto test_compare_2f() -> bool {
    c8::natural co0(1);
    c8::natural co1(0x987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 <= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 2f", p, "1", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3a() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 == co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3a", p, "1", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3b() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 != co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3b", p, "0", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3c() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 > co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3c", p, "0", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3d() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 >= co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3d", p, "1", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3e() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
    auto t = get_start_time_ticks();
    auto co2 = (co0 < co1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << co2;
    return test_check("comp 3e", p, "0", s);
}

/*
 * Compare two 2 digit values.
 */
auto test_compare_3f() -> bool {
    c8::natural co0(0x2f987654321ULL);
    c8::natural co1(0x2f987654321ULL);
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
    c8::natural l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 0;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 0", p, "349f", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_1() -> bool {
    c8::natural l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 1", p, "693e", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_2() -> bool {
    c8::natural l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 18;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 2", p, "d27c0000", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_3() -> bool {
    c8::natural l0(0x349f);
    auto t = get_start_time_ticks();
    auto l1 = l0 << 187;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 3", p, "1a4f80000000000000000000000000000000000000000000000", s);
}

/*
 * Test left shifting.
 */
auto test_lshift_4() -> bool {
    c8::natural l0("0x349f298375323985afbce9837928798789dffeffee987678687678676756562");
    auto t = get_start_time_ticks();
    auto l1 = l0 << 69;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << l1;
    return test_check("lsh 4", p, "693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_0() -> bool {
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 0;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 0", p, "23490000000000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_1() -> bool {
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 1", p, "11a48000000000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_2() -> bool {
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 19;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 2", p, "469200000000000000000000000000000000000000000000000", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_3() -> bool {
    c8::natural r0("0x23490000000000000000000000000000000000000000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 197;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 3", p, "11a4800", s);
}

/*
 * Test right shifting.
 */
auto test_rshift_4() -> bool {
    c8::natural r0("0x693e5306ea64730b5f79d306f250f30f13bffdffdd30ecf0d0ecf0ceceacac400000000000000000");
    auto t = get_start_time_ticks();
    auto r1 = r0 >> 123;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << r1;
    return test_check("rsh 4", p, "d27ca60dd4c8e616bef3a60de4a1e61e277ffbffba61d9e1a", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_0() -> bool {
    c8::natural mu0(3);
    c8::natural mu1(22);
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
    c8::natural mu0(1000000000000000000ULL);
    c8::natural mu1(9999999999999999999ULL);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 1", p, "9999999999999999999000000000000000000", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_2() -> bool {
    c8::natural mu0(0x2000000000000000ULL);
    c8::natural mu1(0x4000000000000000ULL);
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << mu2;
    return test_check("mul 2", p, "8000000000000000000000000000000", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_3() -> bool {
    c8::natural mu0("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    c8::natural mu1("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << mu2;
    return test_check("mul 3", p, "15241578753238836750495351562566681945008382873376009755225118122311263526910001371743100137174310012193273126047859425087639153757049236500533455762536198787501905199875019052100", s);
}

/*
 * Test multiplication.
 */
auto test_multiply_4() -> bool {
    c8::natural mu0("0x1000000000000000100000000000000100000000");
    c8::natural mu1("0xabcdef12");
    auto t = get_start_time_ticks();
    auto mu2 = mu0 * mu1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << mu2;
    return test_check("mul 4", p, "abcdef1200000000abcdef120000000abcdef1200000000", s);
}

/*
 * Test division.
 */
auto test_divide_0() -> bool {
    c8::natural d0(1000000000000000000ULL);
    c8::natural d1(99999999999999999ULL);
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
    c8::natural d0("7829238792751875818917817519758789749174743847389742871867617465710657162");
    c8::natural d1(99999999999999999ULL);
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << d2 << ',' << mo2;
    return test_check("div 1", p, "78292387927518758972102054472775487212767983201652300846,35600667362958008", s);
}

/*
 * Test division.
 */
auto test_divide_2() -> bool {
    c8::natural d0("0x100000000000000000000000000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000001000000000000000100000000");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2 << ',' << mo2;
    return test_check("div 2", p, "ffffffffffffffff000000000000000,100000000000000000000000", s);
}

/*
 * Divide by zero.  This will throw an exception!
 */
auto test_divide_3() -> bool {
    bool res = true;

    c8::natural d0(2000);
    c8::natural d1(0);
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
    c8::natural d0("0x10000000000000");
    c8::natural d1("0x100000000");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2 << ',' << mo2;
    return test_check("div 4", p, "100000,0", s);
}

/*
 * Test division.
 */
auto test_divide_5() -> bool {
    c8::natural d0("0x10000000000000001000000000000000100000000");
    c8::natural d1("1");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2 << ',' << mo2;
    return test_check("div 5", p, "10000000000000001000000000000000100000000,0", s);
}

/*
 * Trigger a divide that exercises a corner case in the divide estimation logic.
 */
auto test_divide_6() -> bool {
    c8::natural d0("0x1000000000000000000000002000000000000000000000000000000000000000000000000");
    c8::natural d1("0x10000000000000000000000010000000000000000");
    auto t = get_start_time_ticks();
    auto d2 = d0 / d1;
    auto mo2 = d0 % d1;
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << d2 << ',' << mo2;
    return test_check("div 6", p, "1000000000000000000000000ffffffff,ffffffffffffffff000000010000000000000000", s);
}

/*
 * Test greatest common divisor.
 */
auto test_gcd_0() -> bool {
    c8::natural g0(2000);
    c8::natural g1(56);
    auto t = get_start_time_ticks();
    c8::natural g2 = gcd(g0, g1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << g2;
    return test_check("gcd 0", p, "8", s);
}

/*
 * Test greatest common divisor.
 */
auto test_gcd_1() -> bool {
    c8::natural g0("47598475892456783750932574388878478947978888888");
    c8::natural g1("87987922283");
    auto t = get_start_time_ticks();
    c8::natural g2 = gcd(g0, g1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << g2;
    return test_check("gcd 1", p, "1", s);
}

/*
 * Test greatest common divisor.
 */
auto test_gcd_2() -> bool {
    c8::natural g0("8888888");
    c8::natural g1("8888888");
    auto t = get_start_time_ticks();
    c8::natural g2 = gcd(g0, g1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << g2;
    return test_check("gcd 2", p, "8888888", s);
}

/*
 * Test greatest common divisor.
 */
auto test_gcd_3() -> bool {
    c8::natural g0("2038355020176327696765561949673186971898109715960816150233379221718753632190267");
    c8::natural g1("1957628088684195906794648605131674616575412301467318480917205787195238636855999");
    auto t = get_start_time_ticks();
    c8::natural g2 = gcd(g0, g1);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << g2;
    return test_check("gcd 3", p, "20181732873032947492728336135378088830674353623374417329043358630878748833567", s);
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_to_unsigned_long_long_0() -> bool {
    c8::natural n(0);
    auto t = get_start_time_ticks();
    unsigned long long u = to_unsigned_long_long(n);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << u;
    return test_check("toull 0", p, "0", s);
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_to_unsigned_long_long_1() -> bool {
    c8::natural n(2000);
    auto t = get_start_time_ticks();
    unsigned long long u = to_unsigned_long_long(n);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << u;
    return test_check("toull 1", p, "2000", s);
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_to_unsigned_long_long_2() -> bool {
    bool res = true;

    c8::natural n("47895748574857485728747548237543205782573485472759047548275024574207");
    auto t = get_start_time_ticks();
    try {
        unsigned long long u = to_unsigned_long_long(n);
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << u;
        res &= test_nocheck("toull 2", p, "failed to throw exception", false);
    } catch (const c8::overflow_error &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toull 2", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("to_unsigned_long_long 2", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_to_unsigned_long_long_3() -> bool {
    c8::natural n(0x123456789a);
    auto t = get_start_time_ticks();
    unsigned long long u = to_unsigned_long_long(n);
    auto p = get_end_time_ticks() - t;
    std::stringstream s;
    s << std::hex << u;
    return test_check("toull 3", p, "123456789a", s);
}

/*
 * Test to_unsigned_long_long functionality.
 */
auto test_to_unsigned_long_long_4() -> bool {
    bool res = true;

    /*
     * Construct a natural number that is one bit too large to be able to convert.
     */
    c8::natural n0(1);
    c8::natural n1 = n0 << (sizeof(unsigned long long) * 8);
    auto t = get_start_time_ticks();
    try {
        unsigned long long u = to_unsigned_long_long(n1);
        auto p = get_end_time_ticks() - t;
        std::stringstream s;
        s << u;
        res &= test_nocheck("toull 4", p, "failed to throw exception", false);
    } catch (const c8::overflow_error &e) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toull 4", p, "exception thrown: " + std::string(e.what()), true);
    } catch (...) {
        auto p = get_end_time_ticks() - t;
        res &= test_nocheck("toull 4", p, "unexpected exception thrown", false);
    }

    return res;
}

/*
 * Test printing.
 */
auto test_print_0() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 0", p, "4701397401952099592073", s);
}

/*
 * Test printing.
 */
auto test_print_1() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 1", p, "fedcfedc0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_2() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 2", p, "FEDCFEDC0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_3() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 3", p, "775563766700044321263611", s);
}

/*
 * Test printing.
 */
auto test_print_4() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 4", p, "4701397401952099592073", s);
}

/*
 * Test printing.
 */
auto test_print_5() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 5", p, "0xfedcfedc0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_6() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::uppercase << std::hex << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 6", p, "0XFEDCFEDC0123456789", s);
}

/*
 * Test printing.
 */
auto test_print_7() -> bool {
    c8::natural v("0xfedcfedc0123456789");
    std::stringstream s;
    auto t = get_start_time_ticks();
    s << std::showbase << std::oct << v;
    auto p = get_end_time_ticks() - t;
    return test_check("prn 7", p, "0775563766700044321263611", s);
}

/*
 * Report the usage for this test program.
 */
static auto usage(const char *name) -> void {
    std::cerr << "usage: " << name << " [OPTIONS]\n\n";
    std::cerr << "Options\n";
    std::cerr << "  -v  Verbose reporting (optional)\n\n";
}

typedef bool (*test)();

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
    test_count_bits_0,
    test_count_bits_1,
    test_count_bits_2,
    test_count_bits_3,
    test_add_0,
    test_add_1,
    test_add_2,
    test_add_3,
    test_add_4,
    test_add_5,
    test_subtract_0,
    test_subtract_1,
    test_subtract_2,
    test_subtract_3,
    test_subtract_4,
    test_subtract_5,
    test_subtract_6,
    test_subtract_7,
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
    test_multiply_3,
    test_multiply_4,
    test_divide_0,
    test_divide_1,
    test_divide_2,
    test_divide_3,
    test_divide_4,
    test_divide_5,
    test_divide_6,
    test_gcd_0,
    test_gcd_1,
    test_gcd_2,
    test_gcd_3,
    test_to_unsigned_long_long_0,
    test_to_unsigned_long_long_1,
    test_to_unsigned_long_long_2,
    test_to_unsigned_long_long_3,
    test_to_unsigned_long_long_4,
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

    /*
     * Run the tests.
     */
    bool res = true;

    test *p = tests;
    while (*p) {
        res &= (*p)();
        p++;
    }

    if (!res) {
        std::cout << "TESTS FAILED!\n";
        exit(-1);
    }

    std::cout << "All tests passed\n";
    return 0;
}

