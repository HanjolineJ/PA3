import random
import time
import concurrent.futures
import os

# Function to generate a random matrix
def generate_random_matrix(rows, cols):
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

# Function to generate a matrix filled with 1's (for correctness proof)
def generate_ones_matrix(size):
    return [[1 for _ in range(size)] for _ in range(size)]

# Function to multiply a section of rows in A with B
def _multiply_rows(A, B, result, row_start, row_end):
    for i in range(row_start, row_end):
        for j in range(len(B[0])):
            total = 0
            for k in range(len(A[0])):
                total += A[i][k] * B[k][j]
            result[i][j] = total

# Function to perform matrix multiplication using threads
def matrix_multi_threads(A, B, num_workers):
    result = [[0] * len(B[0]) for _ in range(len(A))]
    chunk_size = len(A) // num_workers
    remainder = len(A) % num_workers

    with concurrent.futures.ThreadPoolExecutor(max_workers=num_workers) as executor:
        futures = []
        row_start = 0
        for _ in range(num_workers):
            rows_for_thread = chunk_size + (1 if remainder > 0 else 0)
            remainder = max(0, remainder - 1)
            row_end = row_start + rows_for_thread

            if row_start >= len(A):
                break

            futures.append(executor.submit(_multiply_rows, A, B, result, row_start, row_end))
            row_start = row_end

        for f in futures:
            f.result()

    return result

if __name__ == "__main__":
    # **Proof of Correctness: Small Matrix (3x3) with Ones**
    size = 3
    A = generate_ones_matrix(size)
    B = generate_ones_matrix(size)

    result = matrix_multi_threads(A, B, num_workers=2)

    print("\n Matrix Multiplication:")
    for row in result:
        print(row)

    # **Performance Testing with Larger Matrices**
    sizes = [10, 50, 100, 500, 1000, 1200, 1500]  # Matrix sizes to test
    thread_experiments = [3, 7, 9, 2]  # Different thread counts to test

    for size in sizes:
        A, B = generate_random_matrix(size, size), generate_random_matrix(size, size)

        for threads in thread_experiments:
            start_time = time.time()
            _ = matrix_multi_threads(A, B, num_workers=threads)
            end_time = time.time()

            print(f"Python (Threads={threads}), size={size} => {end_time - start_time:.4f} sec")
