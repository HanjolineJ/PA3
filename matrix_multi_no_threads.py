def matrix_multiplication(A, B):
    """
    Perform matrix multiplication between two matrices A and B.

    :param A: List of lists representing matrix A
    :param B: List of lists representing matrix B
    :return: Resultant matrix after multiplication
    """
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])

    if cols_A != rows_B:
        raise ValueError("Number of columns in A must be equal to number of rows in B")

    # Initialize the resultant matrix with zeros
    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Perform matrix multiplication
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                result[i][j] += A[i][k] * B[k][j]

    return result

if __name__ == "__main__":
    # Sample matrices for multiplication
    A = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]
    B = [
        [9, 8, 7],
        [6, 5, 4],
        [3, 2, 1]
    ]

    print("Matrix A:")
    for row in A:
        print(row)
    
    print("\nMatrix B:")
    for row in B:
        print(row)

    # Perform multiplication
    result = matrix_multiplication(A, B)

    print("\nResultant Matrix:")
    for row in result:
        print(row)
