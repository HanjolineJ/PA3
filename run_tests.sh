#!/bin/bash

# CSV Files for storing timing results
CSV_C_NO_THREADS="timing_c_no_threads.csv"
CSV_C_PTHREADS="timing_c_pthreads.csv"
CSV_C_OPENMP="timing_c_openmp.csv"
CSV_PY_NO_THREADS="timing_python_no_threads.csv"
CSV_PY_THREADS="timing_python_threads.csv"

# Write headers for CSV files
echo "Matrix Size,Time (sec)" > $CSV_C_NO_THREADS
echo "Matrix Size,Threads,Time (sec)" > $CSV_C_PTHREADS
echo "Matrix Size,Threads,Time (sec)" > $CSV_C_OPENMP
echo "Matrix Size,Time (sec)" > $CSV_PY_NO_THREADS
echo "Matrix Size,Threads,Time (sec)" > $CSV_PY_THREADS

# Use GCC with OpenMP Support
GCC_COMPILER="gcc-14"  # Update this if your GCC version is different

# Compile C Programs
gcc -o matrix_no_threads matrix_multi_no_threads.c -O2
gcc -o matrix_pthreads matrix_multi_pthreads.c -O2 -lpthread
$GCC_COMPILER -o matrix_openmp matrix_multi_openmp.c -O2 -fopenmp

# Function to extract timing data and store it in CSV
extract_and_store_data() {
    local file="$1"
    local csv_file="$2"
    local is_threaded="$3"

    while read -r line; do
        if [[ $is_threaded -eq 1 ]]; then
            if [[ $line =~ ([0-9]+) ]]; then
                size=$(echo $line | grep -oP '(?<=size=)[0-9]+')
                threads=$(echo $line | grep -oP '(?<=Threads=)[0-9]+')
                time=$(echo $line | grep -oP '[0-9]+\.[0-9]+')
                echo "$size,$threads,$time" >> "$csv_file"
            fi
        else
            if [[ $line =~ ([0-9]+) ]]; then
                size=$(echo $line | grep -oP '(?<=size=)[0-9]+')
                time=$(echo $line | grep -oP '[0-9]+\.[0-9]+')
                echo "$size,$time" >> "$csv_file"
            fi
        fi
    done < "$file"
}

# Run C Programs and Capture Output in CSV
./matrix_no_threads | tee temp_output.txt
extract_and_store_data temp_output.txt "$CSV_C_NO_THREADS" 0

./matrix_pthreads | tee temp_output.txt
extract_and_store_data temp_output.txt "$CSV_C_PTHREADS" 1

./matrix_openmp | tee temp_output.txt
extract_and_store_data temp_output.txt "$CSV_C_OPENMP" 1

# Run Python Programs and Capture Output in CSV
python3 matrix_multi_no_threads.py | tee temp_output.txt
extract_and_store_data temp_output.txt "$CSV_PY_NO_THREADS" 0

python3 matrix_multi_threads.py | tee temp_output.txt
extract_and_store_data temp_output.txt "$CSV_PY_THREADS" 1

# Cleanup temporary files
rm temp_output.txt
