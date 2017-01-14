/*
 * rational_check.c
 */
#include <iomanip>
#include <iostream>
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
        c8::natural v6("2323/01185415157637671751");
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

    return res;
}

/*
 * Test value comparisons.
 */
auto test_compare() -> bool {
    bool res = true;

    return res;
}

/*
 * Test multiplication.
 */
auto test_multiply() -> bool {
    bool res = true;

    return res;
}

/*
 * Test division.
 */
auto test_divide() -> bool {
    bool res = true;

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

