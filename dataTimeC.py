import matplotlib.pyplot as plt
import numpy as np

# Array sizes (excluding the very small case: size 3)
sizes = np.array([10, 50, 100, 500, 1000, 1200, 1500])

# Timing data (in seconds) for each implementation:
c_no_threads = np.array([0.0000, 0.0001, 0.0006, 0.0829, 0.8670, 1.0016, 2.0805])
c_pthreads = np.array([0.000033, 0.000067, 0.000174, 0.015255, 0.201138, 0.190634, 0.431612])
c_openmp   = np.array([0.0001,  0.0001,  0.0002,  0.0174,   0.2175,  0.2211,  0.4866])

plt.figure(figsize=(10, 5))

plt.plot(sizes, c_no_threads, marker='o', label='C (No Threads)')
plt.plot(sizes, c_pthreads, marker='s', label='C (PThreads)')
plt.plot(sizes, c_openmp, marker='^', label='C (OpenMP)')

plt.xlabel('Matrix Size')
plt.ylabel('Execution Time (sec)')
plt.title('Matrix Multiplication Timing Comparison')
#plt.xscale('linear')
#plt.yscale('log')  # log scale for y-axis due to wide range of times
plt.legend()
plt.grid(True, which="both", ls="--")
plt.tight_layout()

plt.show()
