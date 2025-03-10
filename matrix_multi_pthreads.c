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
    srand((unsigned)time(NULL));

    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("System has %d CPU cores.\n", num_cores);

    int sizes[] = {10, 50, 100, 500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int size = sizes[s];

        int** A = generate_random_matrix(size, size);
        int** B = generate_random_matrix(size, size);
        int** C = generate_random_matrix(size, size); // to store the result

        for (int t = 0; t < 4; t++) {
            int num_threads = thread_experiments[t];
            pthread_t threads[num_threads];

            // Adjust thread workload accordingly
            int rows_per_thread = size / num_threads;
            int remainder = size % num_threads;

            printf("Running with %d threads...\n", num_threads);

            clock_t start = clock();

            int row_start = 0;
            for (int i = 0; i < num_threads; i++) {
                // Each thread can get either rows_per_thread or (rows_per_thread + 1)
                int chunk_size = rows_per_thread + (remainder > 0 ? 1 : 0);
                remainder = remainder > 0 ? remainder - 1 : 0;

                int row_end = row_start + chunk_size;
                if (row_start >= size) {
                    // No more rows left — spawn no thread
                    break;
                }

                // Allocate data struct for this thread
                thread_data* data = (thread_data*)malloc(sizeof(thread_data));
                data->row_start = row_start;
                data->row_end = row_end; // up to but not including row_end
                data->A = A;
                data->B = B;
                data->C = C;
                data->size = size;

                pthread_create(&threads[i], NULL, multiply_rows, (void*)data);

                row_start = row_end; // Next thread starts where this one left off
            }

            // Join all threads
            for (int i = 0; i < num_threads; i++) {
                pthread_join(threads[i], NULL);
            }

            clock_t end = clock();
            double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

            printf("C (POSIX thread pool=%d), size=%d => %.4f sec\n", num_threads, size, elapsed);
        }

        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}