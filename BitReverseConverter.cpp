// Logan Wilmoth
// ECE 3600 Project 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void printBinary(unsigned int n) {
    int num[32];
    for (int i = 0; i < 32; i++) {
        if ((n % 2) == 1) {
            num[31 - i] = 1;
        } else { 
            num[31 - i] = 0;
        }
        n /= 2;
    }
    for (int i = 0; i < 32; i++) {
        printf("%d", num[i]);
    }
}

unsigned int reverseBits(unsigned int n) {
    unsigned int bit_size = sizeof(n) * 8;
    unsigned int original_bits[bit_size];   
    unsigned int reversed_bits[bit_size];

    for (int i = 0; i < bit_size; i++) {
        original_bits[i] = (n >> i) & 1;
    }

    for (int i = 0; i < bit_size; i++) {
        reversed_bits[i] = original_bits[bit_size - 1 - i];
    }

    unsigned int reversed_num = 0;
    for (unsigned int i = 0; i < bit_size; i++) {
        reversed_num |= (reversed_bits[i] << i);
    }

    return reversed_num;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Format: %s <numbers>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        errno = 0;
        char *e;
        unsigned int num;

        if (strncmp(argv[i], "0b", 2) == 0) {
            num = strtoul(argv[i] + 2, &e, 2);
        } else {
            num = strtoul(argv[i], &e, 0);
        }

        if (errno != 0 || *e != '\0') {
            perror("strtoul");
            fprintf(stderr, "convert: bad argument: %s\n", argv[i]);

            continue;
        }

        printf("%s = %u = 0x%08x = 0b", argv[i], num, num);
        printBinary(num);
        printf("\n");

        unsigned int reversed = reverseBits(num);
        printf("bitrev = %u = 0x%08x = 0b", reversed, reversed);
        printBinary(reversed);
        printf("\n");
    }

    return 0;
}
