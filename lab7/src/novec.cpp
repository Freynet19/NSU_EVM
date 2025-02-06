#include "novec.h"

MatNovec::MatNovec(int n): Mat(n) {}

void MatNovec::subtract(MatNovec& other) {
    for (int i = 0; i < size * size; ++i) {
        data[i] -= other.data[i];
    }
}

void MatNovec::add(const MatNovec& other) {
    for (int i = 0; i < size * size; ++i) {
        data[i] += other.data[i];
    }
}

void MatNovec::multiplyByNumber(float number) {
    for (auto& element : data) {
        element *= number;
    }
}

MatNovec MatNovec::multiply(MatNovec& other) {
    MatNovec result(size);

    for (int i = 0; i < size; ++i) {
        float *c = result.data.data() + i * size; // указатель на начало строки в результирующей матрице
        for (int k = 0; k < size; ++k) {
            const float *b = other.data.data() + k * size; // указатель на начало строки во второй матрице (other)
            float a = at(i, k); // элемент из текущей матрицы
            for (int j = 0; j < size; ++j) {
                c[j] += a * b[j];
            }
        }
    }

    return result;
}

MatNovec MatNovec::findInverseMatrix(MatNovec& matrix, int iterations) {
    int n = matrix.size;

    MatNovec B = matrix;
    B.transpose();

    // нахождение коэффициентов для масштабирования B
    // после масштабирования значения матрицы будут находиться в пределах, близких к 1, что
    //     минимизирует ошибки, возникающие при итерационном процессе
    float A1 = matrix.findMaxAbsSumByRows();
    float A2 = B.findMaxAbsSumByRows(); // так как B транспонирована, то A2 = сумма по столбцам
    B.multiplyByNumber(1 / (A1 * A2));

    MatNovec R(n);
    R.fillIdentity();

    // вычисление BA и R = E - BA
    MatNovec BA = B.multiply(matrix);
    R.subtract(BA);

    MatNovec result(n);
    result.fillIdentity();

    // Вычисление R^k и добавление к result.
    MatNovec RSeries = R;
    for (int i = 1; i < iterations; ++i) {
        RSeries = RSeries.multiply(R);
        result.add(RSeries);
    }

    // B.transpose();
    return result.multiply(B);

    // R = E - BA
    // A^{-1} = B * (E + R + R^2 + ... + R^{k})
}

