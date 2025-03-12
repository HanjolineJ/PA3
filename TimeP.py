#// Referenced Youtube and Chatgbt for coding assistance 

import re
import matplotlib.pyplot as plt
import numpy as np

def parse_python_timings(log_file):
    sizes = set()
    no_threads = {}
    threads = {}
    with open(log_file, "r") as f:
        for line in f:
            # Parse Python No Threads: e.g., "Python (No Threads), size=500 => 5.3068 sec"
            m = re.search(r"Python \(No Threads\), size=(\d+)\s*=>\s*([\d\.]+) sec", line)
            if m:
                size = int(m.group(1))
                time_val = float(m.group(2))
                no_threads[size] = time_val
                sizes.add(size)
            # Parse Python Threads: e.g., "Python (Threads=9), size=500 => 3.8151 sec"
            m = re.search(r"Python \(Threads=(\d+)\), size=(\d+)\s*=>\s*([\d\.]+) sec", line)
            if m:
                thread_count = int(m.group(1))
                size = int(m.group(2))
                time_val = float(m.group(3))
                if thread_count not in threads:
                    threads[thread_count] = {}
                threads[thread_count][size] = time_val
                sizes.add(size)
    sizes = sorted(list(sizes))
    return sizes, no_threads, threads

# Parse the log file
log_file = "timing_comparison.log"
sizes, no_threads, threads = parse_python_timings(log_file)

plt.figure(figsize=(10, 5))

# Plot Python (No Threads)
times = [no_threads.get(s, None) for s in sizes]
plt.plot(sizes, times, marker='d', label='Python (No Threads)')

# For Python Threads, select a specific thread count (e.g., the maximum available)
if threads:
    selected_threads = max(threads.keys())
    times = [threads[selected_threads].get(s, None) for s in sizes]
    plt.plot(sizes, times, marker='x', label=f'Python (Threads, {selected_threads} threads)')

plt.xlabel('Matrix Size')
plt.ylabel('Execution Time (sec)')
plt.title('Python Matrix Multiplication Timing Comparison (Parsed from log)')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.show()
