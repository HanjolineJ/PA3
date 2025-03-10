#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// We'll define a maximum number of workers, e.g., 4.
#define NUM_THREADS 4

typedef struct {
    int row_start;
    int row_end;
    int **A;
    int **B;
    int **C;
    int size;  // NxN
} thread_data;

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

void* multiply_rows(void* arg) {
    thread_data* data = (thread_data*)arg;
    int row_start = data->row_start;
    int row_end   = data->row_end;   // non-inclusive bound
    int size      = data->size;
    int** A       = data->A;
    int** B       = data->B;
    int** C       = data->C;

    // Compute each row in [row_start, row_end)
    for(int i = row_start; i < row_end; i++) {
        for(int j = 0; j < size; j++) {
            int sum = 0;
            for(int k = 0; k < size; k++) {
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

    int sizes[] = {10, 50, 100, 500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for(int s = 0; s < num_sizes; s++) {
        int size = sizes[s];

        int** A = generate_random_matrix(size, size);
        int** B = generate_random_matrix(size, size);
        int** C = generate_random_matrix(size, size); // to store the result

        // We will create up to NUM_THREADS threads total
        pthread_t threads[NUM_THREADS];
        
        // Figure out how many rows each thread should process
        int rows_per_thread = size / NUM_THREADS;         // integer division
        int remainder       = size % NUM_THREADS;         // leftover rows

        clock_t start = clock();

        int row_start = 0;
        for(int t = 0; t < NUM_THREADS; t++) {
            // Each thread can get either rows_per_thread or (rows_per_thread+1)
            int chunk_size = rows_per_thread + (remainder > 0 ? 1 : 0);
            remainder = remainder > 0 ? remainder - 1 : 0;

            int row_end = row_start + chunk_size;
            if(row_start >= size) {
                // No more rows left — spawn no thread
                break;
            }

            // Allocate data struct for this thread
            thread_data* data = (thread_data*)malloc(sizeof(thread_data));
            data->row_start = row_start;
            data->row_end   = row_end; // up to but not including row_end
            data->A         = A;
            data->B         = B;
            data->C         = C;
            data->size      = size;

            pthread_create(&threads[t], NULL, multiply_rows, (void*)data);

            row_start = row_end; // Next thread starts where this one left off
        }

        // Join all threads
        for(int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

        printf("C (POSIX thread pool=%d), size=%d => %.4f sec\n", NUM_THREADS, size, elapsed);

        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}
