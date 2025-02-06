#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

double PiLeibniz(long long const n) {
    double result = 0;
    for (long long i = 0; i < n; i++) {
        double t = 1.0 / (2.0 * i + 1.0);
        if (i % 2 != 0) t = -t;
        result += t;
    }
    return result * 4;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong args count\n");
        return 1;
    }
    long long const n = atoll(argv[1]);
    double result = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    result = PiLeibniz(n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double const time = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);

    printf("pi = %.8lf\n", result);
    printf("Time taken: %.3lf sec.\n", time);
    return 0;
}
