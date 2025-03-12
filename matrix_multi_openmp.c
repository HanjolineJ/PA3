// Referenced Youtube and Chatgbt for coding assistance 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// Generate a random matrix
int** generate_random_matrix(int size) {
    int** mat = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) { 
        mat[i] = (int*)malloc(size * sizeof(int)); 
        for (int j = 0; j < size; j++) {
            mat[i][j] = rand() % 10; // Random values from 0 to 9
        }
    }
    return mat;
}

// Generate a matrix filled with 1's
int** generate_ones_matrix(int size) {
    int** mat = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        mat[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            mat[i][j] = 1;
        }
    }
    return mat;
}

// Zero matrix
int** allocate_zero_matrix(int size) {
    int** mat = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        mat[i] = (int*)calloc(size, sizeof(int)); // Allocates & initializes to 0
    }
    return mat;
}

// Free allocated memory
void free_matrix(int** mat, int size) {
    for (int i = 0; i < size; i++) {
        free(mat[i]);
    }
    free(mat);
}

// OpenMP Matrix Multiplication Function
void matrix_multi_omp(int** A, int** B, int** C, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));

    #ifdef _OPENMP
    omp_set_num_threads(sysconf(_SC_NPROCESSORS_ONLN)); // Set thread count dynamically
    #endif

    // **Performance Testing on Larger Matrices**
    int sizes[] = {10, 50, 100, 500, 1000, 1200, 1500}; 
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int thread_experiments[] = {3, 7, 9, 2};

    for (int s = 0; s < num_sizes; s++) { 
        int size = sizes[s];
        int **A = generate_random_matrix(size); 
        int **B = generate_random_matrix(size);
        int **C = allocate_zero_matrix(size);  // Properly allocating C as a zero matrix

        for (int t = 0; t < 4; t++) { 
            int num_threads = thread_experiments[t];

            #ifdef _OPENMP
            omp_set_num_threads(num_threads);
            double start = omp_get_wtime();
            #else
            clock_t start = clock();
            #endif

            matrix_multi_omp(A, B, C, size);

            #ifdef _OPENMP
            double end = omp_get_wtime();
            #else
            clock_t end = clock();
            double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
            #endif

            printf("OpenMP (Threads=%d), size=%d => %.4f sec\n", num_threads, size, end - start);
        }

        // Free allocated memory
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    // **Proof of Correctness: Small Matrix (3x3) of Ones**
    int size = 3;
    int** A = generate_ones_matrix(size);
    int** B = generate_ones_matrix(size);
    int** C = allocate_zero_matrix(size);  // Correctly allocating C

    #ifdef _OPENMP
    omp_set_num_threads(2); // Using 2 threads for correctness proof
    #endif

    matrix_multi_omp(A, B, C, size);

    // Print Resultant Matrix
    printf("\nMatrix Multiplication):\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory for correctness test
    free_matrix(A, size);
    free_matrix(B, size);
    free_matrix(C, size);

    return 0;

    
}
