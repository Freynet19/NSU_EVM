#ifndef BLAS_H
#define BLAS_H

#include "mat.h"

class MatBlas : public Mat {
public:
    explicit MatBlas(int n);

    void multiplyByNumber(float number);
    MatBlas multiply(MatBlas& other);
    static MatBlas findInverseMatrix(MatBlas& matrix, int M);
};


#endif //BLAS_H
