// Referenced Youtube and Chatgbt for coding assistance 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int row_start;
    int row_end;
    int **A;
    int **B;
    int **C;
    int size;
} thread_data;

int** generate_ones_matrix(int rows, int cols) {
    int** mat = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        mat[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            mat[i][j] = 1;  // Fill with ones
        }
    }
    return mat;
}

int** generate_random_matrix(int rows, int cols) {
    int** mat = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        mat[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            mat[i][j] = rand() % 10;  // Random values from 0 to 9
        }
    }
    return mat;
}

void free_matrix(int** mat, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
}

void print_matrix(int** mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* multiply_rows(void* arg) {
    thread_data* data = (thread_data*)arg;
    int row_start = data->row_start;
    int row_end = data->row_end;
    int size = data->size;
    int** A = data->A;
    int** B = data->B;
    int** C = data->C;

    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    free(data); // Free allocated thread data
    pthread_exit(NULL);
}

void multiply_matrix_with_threads(int size, int num_threads, int is_proof) {
    int **A, **B;

    if (is_proof) {
        printf("\n--- Matrix Multiplication ---\n");
        A = generate_ones_matrix(size, size);
        B = generate_ones_matrix(size, size);
    } else {
        A = generate_random_matrix(size, size);
        B = generate_random_matrix(size, size);
    }

    // Allocate C as a zero-initialized matrix
    int **C = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        C[i] = (int*)calloc(size, sizeof(int));
    }

    pthread_t threads[num_threads];
    int rows_per_thread = size / num_threads;
    int remainder = size % num_threads;
    int row_start = 0;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // High-resolution timing

    for (int i = 0; i < num_threads; i++) {
        int chunk_size = rows_per_thread + (remainder > 0 ? 1 : 0);
        remainder = remainder > 0 ? remainder - 1 : 0;
        int row_end = row_start + chunk_size;

        thread_data* data = (thread_data*)malloc(sizeof(thread_data));
        data->row_start = row_start;
        data->row_end = row_end;
        data->A = A;
        data->B = B;
        data->C = C;
        data->size = size;

        pthread_create(&threads[i], NULL, multiply_rows, (void*)data);
        row_start = row_end;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("C (Threads=%d), Matrix Size=%d => %.6f sec\n", num_threads, size, elapsed_time);

    if (is_proof) {
        printf("Result Matrix :\n");
        print_matrix(C, size);
    }

    // Free allocated matrices
    free_matrix(A, size);
    free_matrix(B, size);
    free_matrix(C, size);
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    // Proof of Correctness (3x3 Matrix with all ones)
    multiply_matrix_with_threads(3, 2, 1); // `1` means it's the proof test

    // Benchmarking with multiple sizes and thread counts
    int sizes[] = {10, 50, 100, 500, 1000, 1200, 1500};  
    int threads[] = {1, 2, 4, 8, 12, 16};  

    int size_count = sizeof(sizes) / sizeof(sizes[0]);
    int thread_count = sizeof(threads) / sizeof(threads[0]);

    for (int s = 0; s < size_count; s++) {
        for (int t = 0; t < thread_count; t++) {
            multiply_matrix_with_threads(sizes[s], threads[t], 0); // `0` means normal test
        }
    }

    return 0;

    
}
