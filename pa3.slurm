#!/bin/bash
#SBATCH --job-name=PA3_run_Han            # Job name
#SBATCH --mail-type=END,FAIL          # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=hjulceus@ufl.edu  # Your UF email for notifications
#SBATCH --nodes=1                     # Run on a single node
#SBATCH --ntasks=1                    # Run a single task
#SBATCH --cpus-per-task=4             # Number of CPU cores per task
#SBATCH --mem=1gb                     # Memory request
#SBATCH --time=07:55:00               # Time limit (adjust if necessary)
#SBATCH --output=pa3_%j.log           # Output log file (job ID appended)

# Print out useful job info
pwd; hostname; date

# Load required modules
module load gcc
module load python

# === Compilation Steps ===
# Compile the non-threaded C program with the -std=c99 flag
gcc matrix_multi_no_threads.c -o matrix_no_threads -std=c99

# Compile the OpenMP C program; note the '-fopenmp' flag
gcc matrix_multi_openmp.c -o matrix_openmp -std=c99 -fopenmp

# Compile the pthreads version; note the '-pthread' flag
gcc matrix_multi_pthreads.c -o matrix_pthreads -std=c99 -pthread

# === Execution Steps ===
# Run each of compiled programs.
echo "Running non-threaded executable:"
./matrix_no_threads

echo "Running OpenMP executable:"
./matrix_openmp

echo "Running pthreads executable:"
./matrix_pthreads

echo "Running test suite:"
bash run_tests_og.sh


# Optionally, run  Python programs
echo "Running Python tests (non-threaded version):"
python3 matrix_multi_no_threads.py

echo "Running Python tests (threaded version):"
python3 matrix_multi_threads.py

# Timestamp at the end to know when the job completed.
date
