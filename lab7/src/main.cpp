#include <chrono>
#include <iostream>

#include "novec.h"
#include "vec.h"
#include "blas.h"

template <typename MatType>
void testMat(int N, int M) {
    MatType matrix(N);
    matrix.fillRandom();

    // std::cout << "Original matrix: " << std::endl;
    // matrix.print();

    std::cout << "Calculating inverse matrix (" << typeid(MatType).name() << ")..." << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();
    MatType inverse = MatType::findInverseMatrix(matrix, M);
    const auto end = std::chrono::high_resolution_clock::now();
    const double duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    // std::cout << "Inverse matrix: " << std::endl;
    // inverse.print();

    MatType identity = inverse.multiply(matrix);
    MatType trueIdentity(N);
    trueIdentity.fillIdentity();

    // std::cout << "Identity matrix: " << std::endl;
    // identity.print();

    // std::cout << "Expected identity matrix: " << std::endl;
    // expectedIdentity.print();

    std::cout << "Time taken: " << duration << " seconds" << std::endl;
    std::cout << "Difference: " << identity.calcDiff(trueIdentity) << std::endl << std::endl;
}

int main(int argc, char** argv) {
    int N = 2048;
    int M = 10;

    if (argc == 3) {
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
    }

    testMat<MatNovec>(N, M);
    testMat<MatVec>(N, M);
    testMat<MatBlas>(N, M);

    return 0;
}