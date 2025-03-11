import matplotlib.pyplot as plt
import numpy as np

# Array sizes (excluding the very small case: size 3)
sizes = np.array([10, 50, 100, 500, 1000, 1200, 1500])

# Timing data (in seconds) for each implementation:
python_no_threads = np.array([0.0000, 0.0044, 0.0346, 5.3068, 44.6754, 73.1255, 147.9886])
python_threads = np.array([0.0001, 0.0033, 0.0257, 3.7868, 31.4824, 56.7927, 112.9691])

plt.figure(figsize=(10, 5))

plt.plot(sizes, python_no_threads, marker='d', label='Python (No Threads)')
plt.plot(sizes, python_threads, marker='x', label='Python (Threads)')

plt.xlabel('Matrix Size')
plt.ylabel('Execution Time (sec)')
plt.title('Matrix Multiplication Timing Comparison')
#plt.xscale('linear')
#plt.yscale('log')  # log scale for y-axis due to wide range of times
plt.legend()
plt.grid(True, which="both", ls="--")
plt.tight_layout()

plt.show()
