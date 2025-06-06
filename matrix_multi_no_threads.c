// Referenced Youtube and Chatgbt for coding assistance 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random matrix
int** generate_random_matrix(int size) {
    int** mat = (int**)malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++) {
        mat[i] = (int*)malloc(size * sizeof(int));
        for(int j = 0; j < size; j++) {
            mat[i][j] = rand() % 10; // Random values from 0 to 9
        }
    }
    return mat;
}

// Function to free allocated memory by a matrix 
void free_matrix(int** mat, int size) {
    for(int i = 0; i < size; i++) {
        free(mat[i]);
    }
    free(mat);
}

// Print a matrix
void print_matrix(int** mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Matrix multiplication
void matrix_multiplication(int** A, int** B, int** C, int size) {
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
    srand(time(NULL)); // Random number generation

    // Matrix sizes to test
    int sizes[] = {3, 10, 50, 100, 500, 1000, 1200, 1500};  
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for(int s = 0; s < num_sizes; s++) {
        int size = sizes[s];

        // Generate two random matrices
        int** A = generate_random_matrix(size);
        int** B = generate_random_matrix(size);
        int** C = (int**)malloc(size * sizeof(int*));
        for (int i = 0; i < size; i++) {
            C[i] = (int*)malloc(size * sizeof(int));
        }

        // Measure execution time
        clock_t start = clock();
        matrix_multiplication(A, B, C, size);
        clock_t end = clock();

        double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
        printf("C (No Threads), size=%d => %.4f sec\n", size, elapsed_time);

        // Print matrices only for small sizes
        if (size == 3) {
            printf("\nMatrix A:\n");
            print_matrix(A, size);
            printf("Matrix B:\n");
            print_matrix(B, size);
            printf("Result Matrix C:\n");
            print_matrix(C, size);
        }

        // Free allocated memory
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;

    
}
