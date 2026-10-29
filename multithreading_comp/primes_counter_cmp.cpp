#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <string>

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
        if (isPrime(i)) { count++; }
    }
    result = count; 
}

int main(int argc, char* argv[]) {
    // Read 'n' from the command line arguments
    if (argc < 2) return 1;
    int n = std::stoi(argv[1]);
    
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    std::vector<std::thread> threads;
    std::vector<int> results(num_threads, 0); 
    int chunk_size = n / num_threads;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? n : (start + chunk_size);
        if (start < 2) start = 2;
        
        threads.push_back(std::thread(countPrimes, start, end, std::ref(results[i])));
    }

    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    // Output strictly the duration so Python can parse it easily
    std::cout << duration.count();
    return 0;
}
