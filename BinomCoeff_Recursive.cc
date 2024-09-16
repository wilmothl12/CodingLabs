// Logan Wilmoth
// HW B1 Part 2

#include <iostream>

// Function to calculate the Binomial Coefficient
int binomialCoefficient(int n, int p) {
    if (p == 0 || p == n) {
        return 1;
    } else {
        return binomialCoefficient(n - 1, p - 1) + binomialCoefficient(n - 1, p);
    }
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


