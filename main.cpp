#include "SparseMatrix.h"
#include <iostream>

int main() {
    SparseMatrix matrix;
    
    // Set some values
    matrix.set(0, 0, 1.0);
    matrix.set(1, 1, 2.0);
    matrix.set(2, 2, 3.0);
    
    // Get values
    std::cout << "Value at (1,1): " << matrix.get(1, 1) << std::endl;
    std::cout << "Value at (1,2): " << matrix.get(1, 2) << std::endl;
    
    // Get non-zero count
    std::cout << "Non-zero elements: " << matrix.nonZeroCount() << std::endl;
    
    // Test iterator
    std::cout << "\nIterating through non-zero elements:" << std::endl;
    for (const auto& [pos, value] : matrix) {
        std::cout << "(" << pos.first << "," << pos.second << "): " 
                 << value << std::endl;
    }
    
    // Test transpose
    SparseMatrix transposed = matrix.transpose();
    std::cout << "\nTransposed matrix:" << std::endl;
    std::cout << "Value at (1,1): " << transposed.get(1, 1) << std::endl;
    std::cout << "Value at (0,2): " << transposed.get(0, 2) << std::endl;
    
    return 0;
}