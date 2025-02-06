#include <chrono>
#include <random>
#include <climits>
#include <x86intrin.h>

constexpr int minSize = 4 * 1024 / sizeof(int);
constexpr int maxSize = 2 * 1024 * 1024 / sizeof(int);
constexpr int loops = 16;

void traverseAndPrint(const int* data, const int size) {
    volatile int k;
    unsigned long long i, minTicks = ULLONG_MAX;
    for (int j = 0; j < loops; j++) {
        unsigned long long start = __rdtsc();
        for (k = 0, i = 0; i < size; i++) {
            k = data[k];
        }
        unsigned long long end = __rdtsc();
        minTicks = std::min(minTicks, end - start);
    }
    printf("%llu %0.2lf\n",
        size * sizeof(int) / 1024, static_cast<double>(minTicks) / size);
}

void shuffleAndTest(int* data, const int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // фишер-йетс
    auto randomPermutation = new int[size];
    for (int i = 0; i < size; ++i) {
        randomPermutation[i] = i;
    }
    for (int i = size - 1; i > 0; --i) {
        std::uniform_int_distribution dis(0, i);
        int j = dis(gen);
        std::swap(randomPermutation[i], randomPermutation[j]);
    }
    for (int i = 0; i < size; ++i) {
        data[randomPermutation[i]] = randomPermutation[(i + 1) % size];
    }

    traverseAndPrint(data, size);
    delete[] randomPermutation;
}

int main() {
    FILE* file = freopen("output.txt", "w", stdout);

    std::vector cacheWarming(maxSize, 0.1);
    for (size_t i = 0; i < maxSize; ++i) {
        for (size_t j = 0; j < minSize; ++j) {
            cacheWarming[i] = std::sqrt(cacheWarming[i] * cacheWarming[i] + cacheWarming[j] * cacheWarming[j]);
        }
    }

    auto data = new int[maxSize];
    for (int size = minSize; size <= maxSize; size += minSize) {
        shuffleAndTest(data, size);
    }

    delete[] data;
    fclose(file);
    return 0;
}
