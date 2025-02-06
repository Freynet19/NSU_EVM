#ifndef VEC_H
#define VEC_H

#include "mat.h"

class MatVec : public Mat {
public:
    explicit MatVec(int n);

    void multiplyByNumber(float number);
    MatVec multiply(MatVec& other);
    static MatVec findInverseMatrix(MatVec& matrix, int iterations);
};


#endif //VEC_H
