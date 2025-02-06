#include "blas.h"
#include <cblas.h>

MatBlas::MatBlas(int n): Mat(n) {}

void MatBlas::multiplyByNumber(float number) {
    cblas_sscal(size * size, number, data.data(), 1);
}

MatBlas MatBlas::multiply(MatBlas& other) {
    MatBlas result(size);
    // size - количество строк/столбцов 1 матрицы
    // other.size - количество столбцов 2 матрицы
    // 1.0f - коэффицент умножения 1 (1 * m1 * m2)
    // 0.0f - коэффицент умножения 2 (0 * m3)
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                size, other.size, size,
                1.0f, 
                data.data(), size,
                other.data.data(), other.size,
                0.0f, result.data.data(), result.size);
    return result;
}

MatBlas MatBlas::findInverseMatrix(MatBlas& matrix, int M) {
    MatBlas B = matrix;
    B.transpose();

    float A1 = matrix.findMaxAbsSumByRows();
    float A2 = B.findMaxAbsSumByRows();

    B.multiplyByNumber(1.0f / (A1 * A2)); // масштабируем B приближая его к обратной матрице

    MatBlas R = MatBlas(matrix.size);
    R.fillIdentity();
    MatBlas result = R; // изначально result = E

    MatBlas BA = B.multiply(matrix); // BA - приближение умножить на аргумент
    cblas_saxpy(matrix.size * matrix.size, -1, BA.data.data(), 1, R.data.data(), 1); // R = E + (-1)BA

    MatBlas RSeries = R;

    for (int i = 1; i < M; i++) {
        RSeries = RSeries.multiply(R);
        // result += RSeries
        cblas_saxpy(result.size * result.size, 1.0f, RSeries.data.data(), 1, result.data.data(), 1); // result += 1 * R_series
    }

    return result.multiply(B);
}
