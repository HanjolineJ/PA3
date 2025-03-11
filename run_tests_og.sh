#!/bin/bash

# Output log file
LOG_FILE="timing_comparison.log"
echo "Timing Comparisons - Matrix Multiplication" > $LOG_FILE

# Use GCC with OpenMP Support
GCC_COMPILER="gcc-14"  # Update this if your GCC version is different

# Compile C Programs
gcc -o matrix_no_threads matrix_multi_no_threads.c -O2
gcc -o matrix_pthreads matrix_multi_pthreads.c -O2 -lpthread
$GCC_COMPILER -o matrix_openmp matrix_multi_openmp.c -O2 -fopenmp

# Run C Programs and Capture Output
echo -e "\nRunning C (No Threads)..." | tee -a $LOG_FILE
./matrix_no_threads | tee -a $LOG_FILE

echo -e "\nRunning C (PThreads)..." | tee -a $LOG_FILE
./matrix_pthreads | tee -a $LOG_FILE

echo -e "\nRunning C (OpenMP)..." | tee -a $LOG_FILE
./matrix_openmp | tee -a $LOG_FILE

# Run Python Programs
echo -e "\nRunning Python (No Threads)..." | tee -a $LOG_FILE
python3 matrix_multi_no_threads.py | tee -a $LOG_FILE

echo -e "\nRunning Python (Threads)..." | tee -a $LOG_FILE
python3 matrix_multi_threads.py | tee -a $LOG_FILE

echo -e "\nTiming comparisons stored in $LOG_FILE"
