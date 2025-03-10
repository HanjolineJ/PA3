import random
import time

def generate_random_matrix(rows, cols):
    return [[random.randint(0, 9) for _ in range(cols)] for _ in range(rows)]

def matrix_multiplication(A, B):
    rows_A, cols_A = len(A), len(A[0])
    rows_B, cols_B = len(B), len(B[0])

    if cols_A != rows_B:
        raise ValueError("Number of columns in A must equal number of rows in B.")

    result = [[0] * cols_B for _ in range(rows_A)]
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                result[i][j] += A[i][k] * B[k][j]  # Restore normal multiplication

    return result

def generate_ones_matrix(size):
    return [[1 for _ in range(size)] for _ in range(size)]

if __name__ == "__main__":
    size = 3  # Small matrix for correctness proof
    A = generate_ones_matrix(size)
    B = generate_ones_matrix(size)

    result = matrix_multiplication(A, B)

    print("\nResult Matrix:")
    for row in result:
        print(row)


if __name__ == "__main__":
    sizes = [10, 50, 100, 500, 1000, 1200, 1500] 

    for size in sizes:
        A, B = generate_random_matrix(size, size), generate_random_matrix(size, size)

        start_time = time.time()
        _ = matrix_multiplication(A, B)
        end_time = time.time()

        print(f"Python (No Threads), size={size} => {end_time - start_time:.4f} sec")
