import random
import time
import concurrent.futures
import os

def generate_random_matrix(rows, cols):
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

def _multiply_rows(A, B, result, row_start, row_end):
    for i in range(row_start, row_end):
        for j in range(len(B[0])):
            total = 0
            for k in range(len(A[0])):
                total += A[i][k] * B[k][j]
            result[i][j] = total

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

from multiprocessing import Pool, cpu_count

def matrix_multiplication_parallel(args):
    A, B, row = args
    size = len(A)
    result_row = [sum(A[row][k] * B[k][j] for k in range(size)) for j in range(size)]
    return result_row

def matrix_multi_processes(A, B, num_workers):
    size = len(A)
    with Pool(processes=num_workers) as pool:
        result = pool.map(matrix_multiplication_parallel, [(A, B, i) for i in range(size)])
    return result

if __name__ == "__main__":
    sizes = [10, 50, 100, 500]
    thread_experiments = [3, 7, 9, 2]

    for size in sizes:
        A, B = generate_random_matrix(size, size), generate_random_matrix(size, size)

        for threads in thread_experiments:
            start_time = time.time()
            _ = matrix_multi_processes(A, B, num_workers=threads)
            end_time = time.time()

            print(f"Python (Processes={threads}), size={size} => {end_time - start_time:.4f} sec")
