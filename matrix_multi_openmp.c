#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int** generate_random_matrix(int rows, int cols) {
    int** mat = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++) {
        mat[i] = (int*)malloc(cols * sizeof(int));
        for(int j = 0; j < cols; j++) {
            mat[i][j] = rand() % 10;
        }
    }
    return mat;
}

void free_matrix(int** mat, int rows) {
    for(int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
}

void matrix_multi_omp(int **A, int **B, int **C, int size) {
    // Parallelize the outer two loops
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand((unsigned)time(NULL));

    int sizes[] = {10, 50, 100, 500};
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);

    for(int s = 0; s < num_sizes; s++) {
        int size = sizes[s];

        int** A = generate_random_matrix(size, size);
        int** B = generate_random_matrix(size, size);
        int** C = generate_random_matrix(size, size); // to store the result

        // Record time
        clock_t start = clock();
        // Or optionally use double start_omp = omp_get_wtime(); if you want better resolution.

        matrix_multi_omp(A, B, C, size);

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        printf("C (OpenMP), size=%d => %.4f sec\n", size, elapsed);

        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}