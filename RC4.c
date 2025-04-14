#include <stdio.h>

#define SIZE 8
#define PACKETSIZE 4

void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int A[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");   
}

int main() {

    int K[4] = {1, 2, 3, 6};
    int P[PACKETSIZE] = {1, 2, 2, 2};
    int C[PACKETSIZE] = {0};
    int S[SIZE] = {0};
    int T[SIZE] = {0};

    printf("Original Plaintext: ");
    printArray(P, PACKETSIZE);
    
    for (int i = 0; i < SIZE; i++) {
        S[i] = i;
        T[i] = K[i % 4];
    }

    //printf("Step 1\n");
    //printArray(S, SIZE);  

    int j = 0;
    for (int i = 0; i < SIZE; i++) {
        j = (j + S[i] + T[i]) % SIZE;
        Swap(&S[i], &S[j]);
        //printArray(S, SIZE);  
    }    

    //printf("Step 2\n");

    int i = 0;
    int h = 0;
    j = 0;
    while (h < PACKETSIZE) {
        i = (i + 1) % 8;
        j = (j + S[i]) % 8;
        //printf("i = %d, j = %d\n", i, j);
        Swap(&S[i], &S[j]);
        int t = (S[i] + S[j]) % 8;
        int k = S[t]; 
        //printArray(S, SIZE);
        //printf("k: %d xor p: %d\n", k, P[h]);
        C[h] = k ^ P[h];
        h++;
    }

    printf("Final Ciphertext: ");
    printArray(C, PACKETSIZE);

    return 0;
}