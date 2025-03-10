#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

void matrix_multi_omp(int **A, int **B, int **C, int size) {
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
    srand(time(NULL));
    int sizes[] = {10, 50, 100, 500};
    int thread_experiments[] = {3, 7, 9, 2};

    for (int s = 0; s < 4; s++) {
        int size = sizes[s];
        int **A = generate_random_matrix(size);
        int **B = generate_random_matrix(size);
        int **C = generate_random_matrix(size);

        for (int t = 0; t < 4; t++) {
            int num_threads = thread_experiments[t];
            omp_set_num_threads(num_threads);
            double start = omp_get_wtime();

            matrix_multi_omp(A, B, C, size);

            double end = omp_get_wtime();
            printf("OpenMP (Threads=%d), size=%d => %.4f sec\n", num_threads, size, end - start);
        }
    }
    return 0;
}
