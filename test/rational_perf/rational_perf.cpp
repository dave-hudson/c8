/*
 * rational_dep.c
 */
#include <ctime>

#include <rational.h>

/*
 * Multiply test.
 */
auto multiply_test(const c8::rational &r1, const c8::rational &r2) -> c8::rational {
    return r1 / r2;
}

/*
 * Divide test.
 */
auto divide_test(const c8::rational &r1, const c8::rational &r2) -> c8::rational {
    return r1 / r2;
}

/*
 * Entry point.
 */
auto main(int argc, char **argv) -> int {
    c8::rational r1("4975847591750184768091681057471987491740875821075048278327409237823905/289359320571571207328");
    c8::rational r2("78405718571641225890105715801578107321/3805127107810748923");

    std::time_t t = std::time(nullptr);
    std::time_t s;

    do {
        for (int i = 0; i < 1000; i++) {
            multiply_test(r1, r2);
            divide_test(r1, r2);
        }

        s = std::time(nullptr);
    } while ((s - t) < 30);

    return 0;
}

