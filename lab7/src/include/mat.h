#ifndef MAT_H
#define MAT_H

#include <vector>

class Mat {
public:
    int size;
    alignas(16) std::vector<float> data;

    explicit Mat(int n);
    float& at(int row, int col);
    void fillRandom();
    void fillIdentity();
    void transpose();
    float findMaxAbsSumByRows();
    float calcDiff(Mat& other) const;
    void print();

};


#endif //MAT_H
