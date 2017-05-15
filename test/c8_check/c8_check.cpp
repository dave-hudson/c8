/*
 * c8_check.cpp
 */
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "result.h"
#include "natural_check.h"
#include "integer_check.h"
#include "rational_check.h"

/*
 * No-op test used to find timing information.
 */
auto test_nop() -> result {
    result r("nop");

    r.start_clock();
    r.stop_clock();

    r.check_pass("");
    return r;
}

/*
 * Report the usage for this test program.
 */
static auto usage(const char *name) -> void {
    std::cerr << "usage: " << name << " [OPTIONS]\n\n";
    std::cerr << "Options\n";
    std::cerr << "  -b  Generate benchmark results (optional)\n";
    std::cerr << "  -v  Verbose reporting (optional)\n\n";
}

/*
 * List of tests to run.
 */
const test tests[] = {
    test_natural_construct_0,
    test_natural_construct_1,
    test_natural_construct_2,
    test_natural_construct_3,
    test_natural_construct_4,
    test_natural_construct_5,
    test_natural_construct_6,
    test_natural_construct_7,
    test_natural_size_bits_0,
    test_natural_size_bits_1,
    test_natural_size_bits_2,
    test_natural_size_bits_3,
    test_natural_add_0a,
    test_natural_add_0b,
    test_natural_add_1a,
    test_natural_add_1b,
    test_natural_add_2a,
    test_natural_add_2b,
    test_natural_add_3a,
    test_natural_add_3b,
    test_natural_add_4a,
    test_natural_add_4b,
    test_natural_subtract_0a,
    test_natural_subtract_0b,
    test_natural_subtract_1a,
    test_natural_subtract_1b,
    test_natural_subtract_2a,
    test_natural_subtract_2b,
    test_natural_subtract_3a,
    test_natural_subtract_3b,
    test_natural_subtract_4a,
    test_natural_subtract_4b,
    test_natural_subtract_5a,
    test_natural_subtract_5b,
    test_natural_compare_0a,
    test_natural_compare_0b,
    test_natural_compare_0c,
    test_natural_compare_0d,
    test_natural_compare_0e,
    test_natural_compare_0f,
    test_natural_compare_1a,
    test_natural_compare_1b,
    test_natural_compare_1c,
    test_natural_compare_1d,
    test_natural_compare_1e,
    test_natural_compare_1f,
    test_natural_compare_2a,
    test_natural_compare_2b,
    test_natural_compare_2c,
    test_natural_compare_2d,
    test_natural_compare_2e,
    test_natural_compare_2f,
    test_natural_compare_3a,
    test_natural_compare_3b,
    test_natural_compare_3c,
    test_natural_compare_3d,
    test_natural_compare_3e,
    test_natural_compare_3f,
    test_natural_lshift_0a,
    test_natural_lshift_0b,
    test_natural_lshift_1a,
    test_natural_lshift_1b,
    test_natural_lshift_2a,
    test_natural_lshift_2b,
    test_natural_lshift_3a,
    test_natural_lshift_3b,
    test_natural_lshift_4a,
    test_natural_lshift_4b,
    test_natural_lshift_5a,
    test_natural_lshift_5b,
    test_natural_lshift_6a,
    test_natural_lshift_6b,
    test_natural_rshift_0a,
    test_natural_rshift_0b,
    test_natural_rshift_1a,
    test_natural_rshift_1b,
    test_natural_rshift_2a,
    test_natural_rshift_2b,
    test_natural_rshift_3a,
    test_natural_rshift_3b,
    test_natural_rshift_4a,
    test_natural_rshift_4b,
    test_natural_rshift_5a,
    test_natural_rshift_5b,
    test_natural_rshift_6a,
    test_natural_rshift_6b,
    test_natural_multiply_0a,
    test_natural_multiply_0b,
    test_natural_multiply_1a,
    test_natural_multiply_1b,
    test_natural_multiply_2a,
    test_natural_multiply_2b,
    test_natural_multiply_3a,
    test_natural_multiply_3b,
    test_natural_multiply_4a,
    test_natural_multiply_4b,
    test_natural_multiply_5a,
    test_natural_multiply_5b,
    test_natural_divide_0a,
    test_natural_divide_0b,
    test_natural_divide_0c,
    test_natural_divide_1a,
    test_natural_divide_1b,
    test_natural_divide_1c,
    test_natural_divide_2a,
    test_natural_divide_2b,
    test_natural_divide_2c,
    test_natural_divide_3a,
    test_natural_divide_3b,
    test_natural_divide_3c,
    test_natural_divide_4a,
    test_natural_divide_4b,
    test_natural_divide_4c,
    test_natural_divide_5a,
    test_natural_divide_5b,
    test_natural_divide_5c,
    test_natural_divide_6a,
    test_natural_divide_6b,
    test_natural_divide_6c,
    test_natural_divide_7a,
    test_natural_divide_7b,
    test_natural_divide_7c,
    test_natural_gcd_0,
    test_natural_gcd_1,
    test_natural_gcd_2,
    test_natural_gcd_3,
    test_natural_to_unsigned_long_long_0,
    test_natural_to_unsigned_long_long_1,
    test_natural_to_unsigned_long_long_2,
    test_natural_to_unsigned_long_long_3,
    test_natural_to_unsigned_long_long_4,
    test_natural_print_0,
    test_natural_print_1,
    test_natural_print_2,
    test_natural_print_3,
    test_natural_print_4,
    test_natural_print_5,
    test_natural_print_6,
    test_natural_print_7,
    test_integer_construct_0,
    test_integer_construct_1,
    test_integer_construct_2,
    test_integer_construct_3,
    test_integer_construct_4,
    test_integer_construct_5,
    test_integer_construct_6,
    test_integer_construct_7,
    test_integer_construct_8,
    test_integer_construct_9,
    test_integer_construct_10,
    test_integer_construct_11,
    test_integer_add_0a,
    test_integer_add_0b,
    test_integer_add_1a,
    test_integer_add_1b,
    test_integer_add_2a,
    test_integer_add_2b,
    test_integer_add_3a,
    test_integer_add_3b,
    test_integer_subtract_0a,
    test_integer_subtract_0b,
    test_integer_subtract_1a,
    test_integer_subtract_1b,
    test_integer_subtract_2a,
    test_integer_subtract_2b,
    test_integer_subtract_3a,
    test_integer_subtract_3b,
    test_integer_compare_0a,
    test_integer_compare_0b,
    test_integer_compare_0c,
    test_integer_compare_0d,
    test_integer_compare_0e,
    test_integer_compare_0f,
    test_integer_compare_1a,
    test_integer_compare_1b,
    test_integer_compare_1c,
    test_integer_compare_1d,
    test_integer_compare_1e,
    test_integer_compare_1f,
    test_integer_compare_2a,
    test_integer_compare_2b,
    test_integer_compare_2c,
    test_integer_compare_2d,
    test_integer_compare_2e,
    test_integer_compare_2f,
    test_integer_compare_3a,
    test_integer_compare_3b,
    test_integer_compare_3c,
    test_integer_compare_3d,
    test_integer_compare_3e,
    test_integer_compare_3f,
    test_integer_lshift_0a,
    test_integer_lshift_0b,
    test_integer_lshift_1a,
    test_integer_lshift_1b,
    test_integer_lshift_2a,
    test_integer_lshift_2b,
    test_integer_lshift_3a,
    test_integer_lshift_3b,
    test_integer_lshift_4a,
    test_integer_lshift_4b,
    test_integer_rshift_0a,
    test_integer_rshift_0b,
    test_integer_rshift_1a,
    test_integer_rshift_1b,
    test_integer_rshift_2a,
    test_integer_rshift_2b,
    test_integer_rshift_3a,
    test_integer_rshift_3b,
    test_integer_rshift_4a,
    test_integer_rshift_4b,
    test_integer_multiply_0a,
    test_integer_multiply_0b,
    test_integer_multiply_1a,
    test_integer_multiply_1b,
    test_integer_multiply_2a,
    test_integer_multiply_2b,
    test_integer_multiply_3a,
    test_integer_multiply_3b,
    test_integer_divide_0a,
    test_integer_divide_0b,
    test_integer_divide_1a,
    test_integer_divide_1b,
    test_integer_divide_2a,
    test_integer_divide_2b,
    test_integer_divide_3a,
    test_integer_divide_3b,
    test_integer_divide_4a,
    test_integer_divide_4b,
    test_integer_magnitude_0,
    test_integer_magnitude_1,
    test_integer_magnitude_2,
    test_integer_to_long_long_0,
    test_integer_to_long_long_1,
    test_integer_to_long_long_2,
    test_integer_to_long_long_3,
    test_integer_to_long_long_4,
    test_integer_print_0,
    test_integer_print_1,
    test_integer_print_2,
    test_integer_print_3,
    test_integer_print_4,
    test_integer_print_5,
    test_integer_print_6,
    test_integer_print_7,
    test_rational_construct_0,
    test_rational_construct_1,
    test_rational_construct_2,
    test_rational_construct_3,
    test_rational_construct_4,
    test_rational_construct_5,
    test_rational_construct_6,
    test_rational_construct_7,
    test_rational_construct_8,
    test_rational_construct_9,
    test_rational_construct_10,
    test_rational_add_0a,
    test_rational_add_0b,
    test_rational_add_1a,
    test_rational_add_1b,
    test_rational_add_2a,
    test_rational_add_2b,
    test_rational_add_3a,
    test_rational_add_3b,
    test_rational_subtract_0a,
    test_rational_subtract_0b,
    test_rational_subtract_1a,
    test_rational_subtract_1b,
    test_rational_subtract_2a,
    test_rational_subtract_2b,
    test_rational_subtract_3a,
    test_rational_subtract_3b,
    test_rational_compare_0a,
    test_rational_compare_0b,
    test_rational_compare_0c,
    test_rational_compare_0d,
    test_rational_compare_0e,
    test_rational_compare_0f,
    test_rational_compare_1a,
    test_rational_compare_1b,
    test_rational_compare_1c,
    test_rational_compare_1d,
    test_rational_compare_1e,
    test_rational_compare_1f,
    test_rational_compare_2a,
    test_rational_compare_2b,
    test_rational_compare_2c,
    test_rational_compare_2d,
    test_rational_compare_2e,
    test_rational_compare_2f,
    test_rational_compare_3a,
    test_rational_compare_3b,
    test_rational_compare_3c,
    test_rational_compare_3d,
    test_rational_compare_3e,
    test_rational_compare_3f,
    test_rational_multiply_0a,
    test_rational_multiply_0b,
    test_rational_multiply_1a,
    test_rational_multiply_1b,
    test_rational_multiply_2a,
    test_rational_multiply_2b,
    test_rational_multiply_3a,
    test_rational_multiply_3b,
    test_rational_divide_0a,
    test_rational_divide_0b,
    test_rational_divide_1a,
    test_rational_divide_1b,
    test_rational_divide_2a,
    test_rational_divide_2b,
    test_rational_divide_3a,
    test_rational_divide_3b,
    test_rational_divide_4a,
    test_rational_divide_4b,
    test_rational_to_double_0,
    test_rational_to_double_1,
    test_rational_to_double_2,
    test_rational_to_double_3,
    test_rational_to_double_4,
    test_rational_to_parts_0,
    test_rational_to_parts_1,
    test_rational_print_0,
    test_rational_print_1,
    test_rational_print_2,
    test_rational_print_3,
    test_rational_print_4,
    test_rational_print_5,
    test_rational_print_6,
    test_rational_print_7,
    nullptr
};

/*
 * Entry point.
 */
auto main(int argc, char **argv) -> int {
    bool verbose = false;
    std::size_t loops = 1;

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
     * Run a prolonged no-op test to warm up any power-saving modes to move
     * into a higher performance state.  We do this by running the no-op test
     * repeatedly for 2 seconds.
     */
    std::vector<std::chrono::nanoseconds> nop_durations;
    nop_durations.reserve(1000);

    std::time_t t = std::time(nullptr);
    std::time_t s;

    do {
        for (std::size_t i = 1; i < 1000; i++) {
            result r = test_nop();
            nop_durations.push_back(r.get_elapsed());
        }

        s = std::time(nullptr);
    } while ((s - t) < 2);

    /*
     * Take the 0.01% value - we need to make sure we've picked up one after everything
     * is running in a performance state.
     */
    std::nth_element(nop_durations.begin(), nop_durations.begin() + (nop_durations.size() / 10000), nop_durations.end());
    auto nop_duration = nop_durations[nop_durations.size() / 10000];

    /*
     * Run the tests.
     */
    bool res = true;

    const test *p = tests;
    while (*p) {
        std::vector<std::chrono::nanoseconds> durations;
        durations.reserve(loops);

        /*
         * Run one test every time.  This gives the pass/fail status.
         */
        result r = (*p)();
        bool rp = r.get_pass();
        durations.push_back(r.get_elapsed());

        /*
         * Run more tests if we're running a benchmark.
         */
        for (std::size_t i = 1; i < loops; i++) {
            result l = (*p)();
            durations.push_back(l.get_elapsed());
        }

        /*
         * If we're being verbose then print the results.  We print the 20th percentile result if
         * this is a benchmark test.  Lower than the 10th percentile we can see some rather
         * odd timing results and greater than the 50th percentile we can run into problems
         * related to scheduling.  The 20th percentile number is an empirical choice.
         */
        if (verbose) {
            std::nth_element(durations.begin(), durations.begin() + (durations.size() / 5), durations.end());
            std::cout << std::setw(14) << std::left << r.get_name() << " | ";
            auto d = durations[durations.size() / 5] - nop_duration;
            std::cout << std::setw(10) << std::right << d.count() << " | " << (rp ? "pass" : "FAIL");
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

