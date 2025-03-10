#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>  // Needed for sysconf()

typedef struct {
    int row_start;
    int row_end;
    int **A;
    int **B;
    int **C;
    int size;  // NxN
} thread_data;

int thread_experiments[] = {3, 7, 9, 2};

int** generate_random_matrix(int rows, int cols) {
    int** mat = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        mat[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            mat[i][j] = rand() % 10;
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

void* multiply_rows(void* arg) {
    thread_data* data = (thread_data*)arg;
    int row_start = data->row_start;
    int row_end = data->row_end;   // non-inclusive bound
    int size = data->size;
    int** A = data->A;
    int** B = data->B;
    int** C = data->C;

    // Compute each row in [row_start, row_end)
    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    free(data); // Freed after use
    pthread_exit(NULL);
}

int main() {
    int size = 3; // Small matrix for correctness proof

    int **A = generate_ones_matrix(size);
    int **B = generate_ones_matrix(size);
    int **C = generate_ones_matrix(size);

    int num_threads = 2;  // Using 2 threads for correctness proof

    pthread_t threads[num_threads];

    int rows_per_thread = size / num_threads;
    int remainder = size % num_threads;

    int row_start = 0;
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

    printf("\nResultant Matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
}
