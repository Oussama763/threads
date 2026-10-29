import math
import multiprocessing
import time
import subprocess
import os
import matplotlib.pyplot as plt

# ==========================================
# PYTHON PARALLEL IMPLEMENTATION
# ==========================================
def is_prime(n):
    if n < 2: return False
    for i in range(2, int(math.sqrt(n)) + 1):
        if n % i == 0:
            return False
    return True

def count_primes_in_range(start, end):
    count = 0
    for i in range(start, end):
        if is_prime(i):
            count += 1
    return count

def run_python_parallel(n, num_cores):
    chunk_size = n // num_cores
    ranges = []
    
    for i in range(num_cores):
        start = i * chunk_size
        # Ensure the last chunk goes all the way to n
        end = n if i == num_cores - 1 else (i + 1) * chunk_size
        if start < 2: start = 2
        ranges.append((start, end))

    start_time = time.time()
    
    # Use multiprocessing to bypass the Python GIL
    with multiprocessing.Pool(num_cores) as pool:
        pool.starmap(count_primes_in_range, ranges)
        
    return time.time() - start_time

# ==========================================
# BENCHMARK RUNNER
# ==========================================
def main():
    # 1. Compile the C++ file automatically
    print("Compiling C++ code with -O3 optimizations...")
    compile_cmd = ["g++", "primes_counter_cmp.cpp", "-o", "prime_cpp", "-pthread", "-O3"]
    subprocess.run(compile_cmd, check=True)
    print("Compilation successful!\n")

    # 2. Values of N to test (from 1 million to 10 million)
    n_values = [1000000, 2500000, 5000000, 7500000, 10000000, 20000000]
    
    python_times = []
    cpp_times = []
    
    num_cores = multiprocessing.cpu_count()
    print(f"Running benchmarks using {num_cores} logical cores...\n")

    # 3. Run the tests
    for n in n_values:
        print(f"Testing N = {n:,} ...")
        
        # Run Python
        py_time = run_python_parallel(n, num_cores)
        python_times.append(py_time)
        
        # Run C++
        cpp_cmd = ["./prime_cpp", str(n)]
        cpp_result = subprocess.run(cpp_cmd, capture_output=True, text=True)
        cpp_time = float(cpp_result.stdout.strip())
        cpp_times.append(cpp_time)
        
        print(f"  -> Python: {py_time:.4f} sec | C++: {cpp_time:.4f} sec")

    # ==========================================
    # 4. PLOT THE RESULTS
    # ==========================================
    plt.figure(figsize=(10, 6))
    
    # Plot both lines
    plt.plot(n_values, python_times, marker='o', color='blue', linewidth=2, label='Python (Multiprocessing)')
    plt.plot(n_values, cpp_times, marker='s', color='red', linewidth=2, label='C++ (std::thread)')
    
    # Formatting
    plt.title('Performance Comparison: C++ vs Python\nMultithreaded Prime Counting', fontsize=14)
    plt.xlabel('Value of N (Max Number to Check)', fontsize=12)
    plt.ylabel('Execution Time (Seconds)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)
    
    # Format X-axis to show millions cleanly (e.g., 5,000,000)
    plt.gca().get_xaxis().set_major_formatter(plt.FuncFormatter(lambda x, loc: "{:,}".format(int(x))))
    
    plt.tight_layout()
    plt.savefig('performance_plot.png')
    print("\nBenchmark complete! Plot saved as 'performance_plot.png'")
    plt.show()

if __name__ == "__main__":
    main()
