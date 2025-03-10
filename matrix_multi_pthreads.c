#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int row;
    int col;
    int **A;
    int **B;
    int **C;
    int size;  // both dimension of A and B
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

void* calculate_element(void* args) {
    thread_data* data = (thread_data*)args;
    int sum = 0;
    for (int k = 0; k < data->size; k++) {
        sum += data->A[data->row][k] * data->B[k][data->col];
    }
    data->C[data->row][data->col] = sum;
    free(data); // free our thread_data struct
    pthread_exit(NULL);
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

        int thread_count = size * size;
        pthread_t* threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));

        clock_t start = clock();

        // Create a thread for each element C[i][j]
        int thread_index = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                thread_data* data = (thread_data*)malloc(sizeof(thread_data));
                data->row = i;
                data->col = j;
                data->A = A;
                data->B = B;
                data->C = C;
                data->size = size;
                pthread_create(&threads[thread_index], NULL, calculate_element, (void*)data);
                thread_index++;
            }
        }
        // Wait for all threads
        for(int i = 0; i < thread_count; i++) {
            pthread_join(threads[i], NULL);
        }

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

        printf("C (POSIX threads), size=%d => %.4f sec\n", size, elapsed);

        free(threads);
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}