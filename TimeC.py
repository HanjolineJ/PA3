import re
import matplotlib.pyplot as plt
import numpy as np

def parse_c_timings(log_file):
    sizes = set()
    no_threads = {}
    pthreads = {}
    openmp = {}
    with open(log_file, "r") as f:
        for line in f:
            # Parse C No Threads: e.g., "C (No Threads), size=500 => 0.0829 sec"
            m = re.search(r"C \(No Threads\), size=(\d+)\s*=>\s*([\d\.]+) sec", line)
            if m:
                size = int(m.group(1))
                time_val = float(m.group(2))
                no_threads[size] = time_val
                sizes.add(size)
            # Parse C PThreads: e.g., "C (Threads=8), Matrix Size=500 => 0.015255 sec"
            m = re.search(r"C \(Threads=(\d+)\), Matrix Size=(\d+)\s*=>\s*([\d\.]+) sec", line)
            if m:
                thread_count = int(m.group(1))
                size = int(m.group(2))
                time_val = float(m.group(3))
                if thread_count not in pthreads:
                    pthreads[thread_count] = {}
                pthreads[thread_count][size] = time_val
                sizes.add(size)
            # Parse C OpenMP: e.g., "OpenMP (Threads=9), size=500 => 0.0174 sec"
            m = re.search(r"OpenMP \(Threads=(\d+)\), size=(\d+)\s*=>\s*([\d\.]+) sec", line)
            if m:
                thread_count = int(m.group(1))
                size = int(m.group(2))
                time_val = float(m.group(3))
                if thread_count not in openmp:
                    openmp[thread_count] = {}
                openmp[thread_count][size] = time_val
                sizes.add(size)
    sizes = sorted(list(sizes))
    return sizes, no_threads, pthreads, openmp

# Parse the log file
log_file = "timing_comparison.log"
sizes, no_threads, pthreads, openmp = parse_c_timings(log_file)

plt.figure(figsize=(10, 5))

# Plot C (No Threads)
times = [no_threads.get(s, None) for s in sizes]
plt.plot(sizes, times, marker='o', label='C (No Threads)')

# For C PThreads, select a specific thread count (e.g., the maximum available)
if pthreads:
    selected_threads = max(pthreads.keys())
    times = [pthreads[selected_threads].get(s, None) for s in sizes]
    plt.plot(sizes, times, marker='s', label=f'C (PThreads, {selected_threads} threads)')

# For C OpenMP, select a specific thread count (e.g., the maximum available)
if openmp:
    selected_threads = max(openmp.keys())
    times = [openmp[selected_threads].get(s, None) for s in sizes]
    plt.plot(sizes, times, marker='^', label=f'C (OpenMP, {selected_threads} threads)')

plt.xlabel('Matrix Size')
plt.ylabel('Execution Time (sec)')
plt.title('C Matrix Multiplication Timing Comparison (Parsed from log)')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.show()
