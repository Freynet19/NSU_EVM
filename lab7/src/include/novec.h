#ifndef NOVEC_H
#define NOVEC_H
#include "mat.h"

class MatNovec : public Mat {
public:
    explicit MatNovec(int n);

    void subtract(MatNovec& other);
    void add(const MatNovec& other);
    void multiplyByNumber(float number);
    MatNovec multiply(MatNovec& other);
    static MatNovec findInverseMatrix(MatNovec& matrix, int iterations);
};


#endif //NOVEC_H
