#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

std::vector<int> primes;
std::mutex mutex;
int LIMIT, numThreads;

bool check_prime(int n) {
    // 0 and 1 are not prime numbers
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    // User input for the upper bound
    std::cout << "Enter the upper bound of integers: ";
    std::cin >> LIMIT;

    // User input for the number of threads
    std::cout << "Enter number of threads: ";
    std::cin >> numThreads;

    // Check if the input is lower than 1, set numThreads to 1
    if (numThreads < 1) {
        std::cout << "Invalid input. Using default value of 1" << std::endl;
        numThreads = 1;
    }

    // The timer starts after user input is collected
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;

    // Implement threading to split the range of integers across the specified number of threads and implement mutual exclusion for the `primes` list.
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([i]() {
            int start = 2;
            for (int num = start + i; num <= LIMIT; num += numThreads) {
                if (check_prime(num)) {
                    std::lock_guard<std::mutex> lock(mutex);
                    primes.push_back(num);
                }
            }
            });
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // The timer stops before printing the number of primes found
    auto end = std::chrono::high_resolution_clock::now();

    double runTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    //runTime = runTime * 1e-9;
    std::cout << "Runtime: " << runTime << " seconds" << std::endl;

    std::cout << primes.size() << " prime numbers were found." << std::endl;

    return 0;
}
