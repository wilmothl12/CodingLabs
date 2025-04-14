// Logan Wilmoth
// OS Project 3

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    int nrows = 300;
    int PAGESIZE = sysconf(_SC_PAGESIZE);
    int ncols = PAGESIZE / sizeof(int);
    int *a = (int *)calloc(nrows * ncols, sizeof(int));

    struct timespec start, end;
    long row_time = 0, col_time = 0;

    // By rows
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int iter = 0; iter < 100; ++iter) {
        for (int row = 0; row < nrows; ++row) {
            for (int col = 0; col < ncols; ++col) {
                a[row * ncols + col] += 1;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    row_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    
    // By columns
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int iter = 0; iter < 100; ++iter) {
        for (int col = 0; col < ncols; ++col) {
            for (int row = 0; row < nrows; ++row) {
                a[row * ncols + col] += 1;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    col_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    // Convert to milliseconds
    row_time /= 1e6;
    col_time /= 1e6;

    printf("Row-wise access time: %ld ms\n", row_time);
    printf("Column-wise access time: %ld ms\n", col_time);

    free(a);

    return 0;
}


// Sample Output:
// Row-wise access time: 46 ms
// Column-wise access time: 206 ms

