#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** generate_random_matrix(int size) {
    int** mat = malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++) {
        mat[i] = malloc(size * sizeof(int));
        for(int j = 0; j < size; j++) {
            mat[i][j] = rand() % 10;
        }
    }
    return mat;
}

void matrix_multi(int **A, int **B, int **C, int size) {
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
    srand(time(NULL));
    int sizes[] = {10, 50, 100, 500};

    for(int s = 0; s < 4; s++) {
        int size = sizes[s];
        int **A = generate_random_matrix(size);
        int **B = generate_random_matrix(size);
        int **C = generate_random_matrix(size);

        clock_t start = clock();
        matrix_multi(A, B, C, size);
        clock_t end = clock();
        printf("C (No Threads), size=%d => %.4f sec\n", size, (double)(end - start) / CLOCKS_PER_SEC);
    }
    return 0;
}
