import random
import time
import concurrent.futures

def generate_random_matrix(rows, cols):
    """Generate a rows x cols matrix with random integers."""
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

def _multiply_rows(A, B, result, row_start, row_end):
    """
    Multiply a chunk of rows from A by B and store in 'result'.
    row_start and row_end define which rows this worker is responsible for.
    """
    # Dimensions
    rows_A = len(A)
    cols_A = len(A[0])
    # rows_B = len(B) == cols_A
    cols_B = len(B[0])

    for i in range(row_start, row_end):
        for j in range(cols_B):
            total = 0
            for k in range(cols_A):
                total += A[i][k] * B[k][j]
            result[i][j] = total

def matrix_multi_threads(A, B, num_workers=4):
    """
    Perform matrix multiplication using a pool of threads.
    Each thread operates on a chunk of rows in A.
    
    :param A: List of lists, dimension [rows_A x cols_A]
    :param B: List of lists, dimension [cols_A x cols_B]
    :param num_workers: Number of worker threads in the pool
    :return: Resultant matrix
    """
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])

    if cols_A != rows_B:
        raise ValueError("Number of columns in A must match rows in B.")

    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Determine how many rows each worker should process
    chunk_size = rows_A // num_workers
    remainder = rows_A % num_workers

    # Use ThreadPoolExecutor for a fixed number of threads
    with concurrent.futures.ThreadPoolExecutor(max_workers=num_workers) as executor:
        futures = []
        row_start = 0
        for _ in range(num_workers):
            # Distribute the remainder among some threads (one row extra)
            rows_for_this_thread = chunk_size + (1 if remainder > 0 else 0)
            remainder = max(0, remainder - 1)

            row_end = row_start + rows_for_this_thread
            if row_start >= rows_A:
                break  # No more rows left

            futures.append(executor.submit(_multiply_rows, A, B, result, row_start, row_end))
            row_start = row_end

        # Wait for all workers to finish
        for f in futures:
            f.result()

    return result

if __name__ == "__main__":
    # Example sizes (feel free to expand)
    sizes = [10, 50, 100, 500]  # 200 instead of 500 to keep runtime manageable
    num_threads = 4  # Adjust to match your CPU

    for size in sizes:
        A = generate_random_matrix(size, size)
        B = generate_random_matrix(size, size)

        start_time = time.time()
        _ = matrix_multi_threads(A, B, num_workers=num_threads)
        end_time = time.time()

        print(f"Python (thread pool, {num_threads} workers), size={size} => {end_time - start_time:.4f} sec")
