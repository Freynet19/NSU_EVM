#include <cstdio>
#include <climits>
#include <cmath>
#include <vector>
#include <x86intrin.h>

#define CACHE_SIZE (32 * 1024)

#define ITERATIONS 64

typedef unsigned long long ull;

int* initArray(int fragCount, int offset, int arrSize) {
    const auto array = new int[offset * fragCount];
    for (int i = 0; i < arrSize / fragCount; ++i) {
        for (int j = 0; j < fragCount - 1; ++j) {
            array[i + j * offset] = i + (j + 1) * offset;
        }
        array[i + (fragCount - 1) * offset] = (i + 1) % (arrSize / fragCount);
    }
    return array;
}

ull traverseArray(const int *array, int fragCount) {
    ull minTicks = ULLONG_MAX;
    for (size_t j = 0; j < ITERATIONS; ++j) {
        const ull start = __rdtsc();

        for (volatile size_t k = 0, i = 0; i < fragCount; ++i) {
            k = array[k];
        }

        const ull end = __rdtsc();
        minTicks = std::min(minTicks, end - start);
    }
    return minTicks / fragCount;
}

void computeTicks(int fragCount, int offset, int arrSize) {
    const int* array = initArray(fragCount, offset, arrSize);
    const ull ticks = traverseArray(array, arrSize);
    delete[] array;

    printf("%d %llu\n", fragCount, ticks);
}

int main() {
    constexpr int offset = 16 * 1024 * 1024 / sizeof(int);
    constexpr int arrSize = CACHE_SIZE / sizeof(int);

    FILE *outputFile = freopen("output.txt", "w", stdout);

    for (int fragCount = 1; fragCount <= 32; ++fragCount) {
        computeTicks(fragCount, offset, arrSize);
    }

    fclose(outputFile);
    return 0;
}
