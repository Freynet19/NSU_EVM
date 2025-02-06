#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for clock_gettime
#include <sys/times.h> // for times
#include <unistd.h> // for sysconf

#define N_CONST 2300000000
#define UCGT 1 // 0 to use times, 1 to use clock_gettime

double PiLeibniz(long long const n) {
    double result = 0;
    for (long long i = 0; i < n; i++) {
        double t = 1.0 / (2.0 * i + 1.0);
        if (i % 2 != 0) t = -t;
        result += t;
    }
    return result * 4;
}

int main(void) {
    long long const n = N_CONST;
    double result = 0;
    double avgTime = 0;

    printf("n = %lld\n", n);
    if (UCGT) printf("Using clock_gettime\n");
    else printf("Using times\n");

    for (int i = 1; i <= 5; ++i) {
        double time;

        if (UCGT) {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);

            result = PiLeibniz(n);

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        }

        else { // using times
            struct tms start, end;
            long clocks_per_sec = sysconf(_SC_CLK_TCK);
            times(&start);

            result = PiLeibniz(n);

            times(&end);
            long clocks = end.tms_utime - start.tms_utime;
            time = (double)clocks / clocks_per_sec;
        }

        printf("Test %d, time taken: %lf sec.\n", i, time);
        avgTime += time;
    }

    printf("pi = %.8lf\n", result);
    printf("Average time: %lf sec.\n", avgTime / 5);
    return 0;
}
