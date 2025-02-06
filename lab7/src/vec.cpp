#include "vec.h"
#include <immintrin.h>

MatVec::MatVec(int n): Mat(n) {}

void MatVec::multiplyByNumber(float number) {
    for (auto& element : data) {
        element *= number;
    }
}

MatVec MatVec::multiply(MatVec& other) {
    MatVec result(size);

    for (int i = 0; i < size; ++i) {
        float *c = result.data.data() + i * size; // указатель на начало строки в результирующей матрице
        for (int k = 0; k < size; ++k) {
            float *b = other.data.data() + k * size; // указатель на начало строки во второй матрице (other)
            float a = data[i * size + k]; // элемент из текущей матрицы

            for (int j = 0; j < size; j += 4) {
                __m128 b_vec = _mm_load_ps(b + j);         // загружаем 4 элемента из b
                __m128 a_vec = _mm_set1_ps(a);              // загружаем a в каждую позицию вектора
                __m128 c_vec = _mm_load_ps(c + j);         // загружаем текущие значения результата
                c_vec = _mm_add_ps(c_vec, _mm_mul_ps(a_vec, b_vec));    // c[j:j+4] += a * b[j:j+4]
                _mm_store_ps(c + j, c_vec); // сохраняем обратно в c
            }
        }
    }
    return result;
}

MatVec MatVec::findInverseMatrix(MatVec& matrix, int iterations) {
    MatVec B = matrix;
    B.transpose();

    float A1 = matrix.findMaxAbsSumByRows();
    float A2 = B.findMaxAbsSumByRows();

    B.multiplyByNumber(1.0f / (A1 * A2));

    // инициализация матрицы ошибки R, изначально она единичная
    MatVec R(matrix.size);
    R.fillIdentity();

    // BA = (A^{T}/(A1 * A2)) * A
    MatVec BA = B.multiply(matrix);

    // R = E - BA
    for (int j = 0; j < matrix.size * matrix.size; j += 4) {
        __m128 r_vec = _mm_load_ps(R.data.data() + j);
        __m128 ba_vec = _mm_load_ps(BA.data.data() + j);
        r_vec = _mm_sub_ps(r_vec, ba_vec);
        _mm_store_ps(R.data.data() + j, r_vec);
    }

    MatVec result(matrix.size);
    result.fillIdentity();

    // Вычисление R^k и добавление к result
    MatVec RSeries = R;
    for (int i = 1; i < iterations; i++) {
        RSeries = RSeries.multiply(R);
        // result += R^k (R^k = Rseries)
        for (int j = 0; j < matrix.size * matrix.size; j += 4) {
            __m128 a_vec = _mm_load_ps(result.data.data() + j);
            __m128 b_vec = _mm_load_ps(RSeries.data.data() + j);
            a_vec = _mm_add_ps(a_vec, b_vec);
            _mm_store_ps(result.data.data() + j, a_vec);
        }
    }

    result = result.multiply(B);
    return result;
}

