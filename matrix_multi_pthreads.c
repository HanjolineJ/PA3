#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int row;
    int col;
    int **A;
    int **B;
    int **C;
    int cols_A;
} thread_data;

void *calculate_element(void *args) {
    thread_data *data = (thread_data *)args;
    int sum = 0;
    for (int k = 0; k < data->cols_A; k++) {
        sum += data->A[data->row][k] * data->B[k][data->col];
    }
    data->C[data->row][data->col] = sum;
    free(data);
    pthread_exit(NULL);
}

int main() {
    int rows_A = 3, cols_A = 3, rows_B = 3, cols_B = 3;

    // Allocate memory for matrices
    int** A = (int**)malloc(rows_A * sizeof(int*));
    int** B = (int**)malloc(rows_B * sizeof(int*));
    int** C = (int**)malloc(rows_A * sizeof(int*));
    for (int i = 0; i < rows_A; i++) {
        A[i] = (int*)malloc(cols_A * sizeof(int));
        C[i] = (int*)malloc(cols_B * sizeof(int));
    }
    for (int i = 0; i < rows_B; i++) {
        B[i] = (int*)malloc(cols_B * sizeof(int));
    }

    // Initialize matrices A and B with sample values
    int sample_A[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    int sample_B[3][3] = { {9, 8, 7}, {6, 5, 4}, {3, 2, 1} };
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_A; j++) {
            A[i][j] = sample_A[i][j];
            B[i][j] = sample_B[i][j];
        }
    }

    int thread_count = rows_A * cols_B;
    pthread_t threads[thread_count];
    int thread_index = 0;

    // Create threads for each element in the result matrix
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            thread_data *data = (thread_data *)malloc(sizeof(thread_data));
            data->row = i;
            data->col = j;
            data->A = A;
            data->B = B;
            data->C = C;
            data->cols_A = cols_A;
            pthread_create(&threads[thread_index], NULL, calculate_element, (void *)data);
            thread_index++;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print matrix A
    printf("Matrix A:\n");
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_A; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    // Print matrix B
    printf("\nMatrix B:\n");
    for (int i = 0; i < rows_B; i++) {
        for (int j = 0; j < cols_B; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // Print result matrix
    printf("\nResultant Matrix C:\n");
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < rows_A; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (int i = 0; i < rows_B; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
