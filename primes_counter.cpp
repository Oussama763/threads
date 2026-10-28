#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>


bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= std::sqrt(n); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}


void countPrimes(int start, int end, int& result) {
    int count = 0;
    for (int i = start; i < end; ++i) {
        if (isPrime(i)) {
            count++;
        }
    }
    result = count; 
}

int main() {
    const int MAX_NUM = 50000000; 

    std::cout << "Running single-threaded...\n";
    auto start_single = std::chrono::high_resolution_clock::now();
    
    int single_thread_result = 0;
    countPrimes(2, MAX_NUM, single_thread_result);
    auto end_single = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_single = end_single - start_single;
    
    std::cout << "Found " << single_thread_result << " primes in " 
              << duration_single.count() << " seconds.\n\n";
    std::cout << "Running multi-threaded...\n";
    auto start_multi = std::chrono::high_resolution_clock::now();

    // Ask the OS how many hardware threads (cores) are available
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // Fallback if OS doesn't answer
    
    std::cout << "Detected " << num_threads << " logical cores. Launching threads...\n";

    std::vector<std::thread> threads;
    std::vector<int> results(num_threads, 0); 
    
    int chunk_size = MAX_NUM / num_threads;
    
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? MAX_NUM : (start + chunk_size);
        
        if (start < 2) start = 2;
        threads.push_back(std::thread(countPrimes, start, end, std::ref(results[i])));
    }

    int multi_thread_result = 0;
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
        multi_thread_result += results[i];
    }

    auto end_multi = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_multi = end_multi - start_multi;
    
    std::cout << "Found " << multi_thread_result << " primes in " 
              << duration_multi.count() << " seconds.\n";
              
    std::cout << "\nSpeedup: " << duration_single.count() / duration_multi.count() 
              << "x faster!\n";

    return 0;
}
