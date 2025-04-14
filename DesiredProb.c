// Logan Wilmoth
// Program that calculates how many blocks one should wait given a desired probability of an attack

#include <stdio.h>
#include <math.h>

double calculateLogFactorial(int n) {
    double result = 0.0;
    for (int i = 1; i <= n; ++i) {
        result += log(i);
    }
    return result;
}

double AttackerSuccessProbability(double q, double desiredProbability) {
    double p = 1.0 - q;
    int z = 0;
    double currentProbability = 1.0;
    double lambda, answer, poisson1, poisson2, logFactorial, poisson, prob, exp_val, attackerProb, final;

    while (currentProbability > desiredProbability) {
        z++;
        lambda = z * (q / p);
        answer = 0.0;

        for (int k = 0; k <= z; k++) {
            poisson1 = k * log(lambda) - lambda;
            logFactorial = calculateLogFactorial(k);
            poisson = exp(poisson1 - logFactorial);

            prob = q / p;
            exp_val = z - k;
            attackerProb = 1.0 - pow(prob, exp_val);
            final = poisson * attackerProb;

            //printf("k = %d, poisson1 = %.20f, poisson = %.20f, attackerProb = %.20f, final = %.20f, answer = %.20f\n", k, poisson1, poisson, attackerProb, final, answer);
            answer += final;
        }

        currentProbability = 1.0 - answer;
    }

    return z;
}

int main() {
    double desiredProbability;
    double q;

    printf("Enter the desired probability of an attacker catching up (e.g., 0.0002 for 0.02%%): ");
    scanf("%lf", &desiredProbability);
    printf("Enter the probability of attacker finding the next block (q): ");
    scanf("%lf", &q);

    int blocksToWait = (int) AttackerSuccessProbability(q, desiredProbability);
    printf("\nA merchant should wait %d blocks before sending merchandise.\n", blocksToWait);

    return 0;
}
