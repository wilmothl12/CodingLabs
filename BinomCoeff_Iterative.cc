// Logan Wilmoth
// HW B1 Part 1

#include <iostream>

// Function to calculate the factorial
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// Function to calculate the Binomial Coefficient
double binomialCoefficient(int n, int p) {
    double C = factorial(n) / (factorial(p) * factorial(n - p));
    return C;
}

// Print 16 rows
int main() {
    int numRows = 16;

    for (int n = 0; n < numRows; ++n) {
        for (int k = 0; k <= n; ++k) {
            std::cout << binomialCoefficient(n, k);
            if (k < n)
                std::cout << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


