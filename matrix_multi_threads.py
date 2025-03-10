import random
import time
import threading

def generate_random_matrix(rows, cols):
    """Generate a rows x cols matrix with random integers."""
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

def matrix_multi_threaded(A, B):
    """
    Perform matrix multiplication between two matrices A and B using threads.
    """
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])

    if cols_A != rows_B:
        raise ValueError("Number of columns in A must be equal to number of rows in B")

    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    def calculate_element(i, j):
        for k in range(cols_A):
            result[i][j] += A[i][k] * B[k][j]

    threads = []
    # Create and start one thread per element
    for i in range(rows_A):
        for j in range(cols_B):
            thread = threading.Thread(target=calculate_element, args=(i, j))
            threads.append(thread)
            thread.start()

    # Wait for all threads to finish
    for t in threads:
        t.join()

    return result

if __name__ == "__main__":
    sizes = [10, 50, 100, 500] # 200 instead of 500 for the sake of time

    for size in sizes:
        A = generate_random_matrix(size, size)
        B = generate_random_matrix(size, size)

        start_time = time.time()
        _ = matrix_multi_threaded(A, B)
        end_time = time.time()

        print(f"Python (threads), size={size} => {end_time - start_time:.4f} sec")