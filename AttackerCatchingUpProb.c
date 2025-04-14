// Logan Wilmoth
// Program that calculates the probability of an attacker catching up to the block at the end of the blockchain

#include <stdio.h>
#include <math.h>

double calculateFactorial(int n) {
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double AttackerSuccessProbability(double q, int z) {
    double p = 1.0 - q;
    double lambda = z * (q / p);
    double answer = 0.0;

    for (int k = 0; k <= z; k++) {
        double poisson1 = pow(lambda, k);
        double poisson2 = exp(-lambda);
        double factorial = calculateFactorial(k);
        double poisson = (poisson1 * poisson2) / factorial;

        double prob = q / p;
        double exp_val = z - k;
        double attackerProb = 1.0 - pow(prob, exp_val);
        double final = poisson * attackerProb;

        printf("k = %d, final = %.20f\n", k, final);
        answer += final;
    }
    return 1.0 - answer;
}

int main() {
    int z;
    double q;

    printf("Enter the lead block (z): ");
    scanf("%d", &z);
    printf("Enter the probability of attacker finding the next block (q): ");
    scanf("%lf", &q);
    double probability = AttackerSuccessProbability(q, z);
    printf("\nThe probability of an attacker with \033[1;4m%.2f%%\033[0m of the networks computing power to catch up to the current block in the blockchain when starting \033[1;4m%d\033[0m blocks behind is \033[1;4m%.20f%%\033[0m\n", q * 100, z, probability * 100);

    return 0;
}
