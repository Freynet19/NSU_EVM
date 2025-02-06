#include "mat.h"
#include <iomanip>
#include <random>
#include <iostream>

Mat::Mat(int n) : size(n), data(n * n, 0.0f) {}

float& Mat::at(int row, int col) {
    return data[row * size + col];
}

void Mat::fillRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 10.0);
    size_t N = size;

    for (std::size_t i = 0; i < N; i++)
        for (std::size_t j = 0; j < N; j++)
            data[i * N + j] = dis(gen);

    for (std::size_t i = 0; i < N; i++) {
        float sum = 0.f;
        for (std::size_t j = 0; j < N; j++)
            sum += data[i * N + j];
        data[i * N + i] += sum;
    }
}

void Mat::fillIdentity() {
    std::fill(data.begin(), data.end(), 0);
    for (int i = 0; i < size; ++i) {
        at(i, i) = 1.0f;
    }
}

void Mat::transpose() {
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            std::swap(at(i, j), at(j, i));
        }
    }
}

float Mat::findMaxAbsSumByRows() {
    float result = 0.0f;

    for (int i = 0; i < size; i++) {
        float current_sum = 0.0f;
        for (int j = 0; j < size; j++) {
            current_sum += std::fabs(data[i * size + j]);
        }
        result = std::max(result, current_sum);
    }

    return result;
}

float Mat::calcDiff(Mat& other) const {
    float diff = 0.0f;

    for (int i = 0; i < size * size; ++i) {
        diff += std::fabs(data[i] - other.data[i]);
    }

    return diff;
}

void Mat::print() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << std::fixed << std::setprecision(3) << at(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
