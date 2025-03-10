#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generate a rows x cols matrix with random integers in [0,9].
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

void matrix_multi(int **A, int **B, int **C, int rows_A, int cols_A, int cols_B) {
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            C[i][j] = 0;
            for (int k = 0; k < cols_A; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand((unsigned)time(NULL));  // seed random

    int sizes[] = {10, 50, 100, 500};
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);
    
    for(int s = 0; s < num_sizes; s++) {
        int size = sizes[s];
        // Generate random matrices
        int** A = generate_random_matrix(size, size);
        int** B = generate_random_matrix(size, size);
        int** C = generate_random_matrix(size, size); // we'll overwrite values in C

        clock_t start = clock();
        matrix_multi(A, B, C, size, size, size);
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

        printf("C (no threads), size=%d => %.4f sec\n", size, elapsed);

        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}