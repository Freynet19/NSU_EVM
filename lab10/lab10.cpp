#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

constexpr int NUM_THREADS = 6;
constexpr int NUM_ITERATIONS = 40000000;
constexpr int CACHE_LINE_SIZE = 64;

struct
// alignas(CACHE_LINE_SIZE)
FSData {
    int value;
    // char padding[CACHE_LINE_SIZE - sizeof(int)];
};

void FSAdder(FSData* data, int index) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        data[index].value += 1;
    }
}

int main() {
    FSData data[NUM_THREADS];
    std::vector<std::thread> threads;

    const auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(FSAdder, data, i);
    }
    for (auto& thread : threads) thread.join();

    const auto end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
