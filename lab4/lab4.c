#include <stdio.h>

#define N_CONST 1000000000

double PiLeibniz(long long const n) {
    double result = 0;
    for (long long i = 0; i < n; ++i) {
        double const t = 1.0 / (2.0 * (double)i + 1.0);
        result = i % 2 == 0 ? result + t : result - t;
    }
    return result * 4;
}

int main(void) {
    double const result = PiLeibniz(N_CONST);
    printf("pi = %.8lf\n", result);
    return 0;
}
