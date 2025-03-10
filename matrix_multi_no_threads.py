import random
import time

def generate_random_matrix(rows, cols):
    """Generate a rows x cols matrix with random integers."""
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

def matrix_multiplication(A, B):
    """
    Perform matrix multiplication between two matrices A and B.
    A is rows_A x cols_A, B is rows_B x cols_B
    """
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])

    if cols_A != rows_B:
        raise ValueError("Number of columns in A must be equal to number of rows in B")

    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Perform matrix multiplication
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                result[i][j] += A[i][k] * B[k][j]

    return result

if __name__ == "__main__":
    sizes = [10, 50, 100, 500]

    for size in sizes:
        # Generate random matrices A and B, each of dimension (size x size)
        A = generate_random_matrix(size, size)
        B = generate_random_matrix(size, size)

        start_time = time.time()
        _ = matrix_multiplication(A, B)
        end_time = time.time()

        print(f"Python (no threads), size={size} => {end_time - start_time:.4f} sec")