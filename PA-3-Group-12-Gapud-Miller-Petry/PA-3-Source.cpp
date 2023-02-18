#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <chrono>
#include <filesystem>
#include <fstream>

int recursiveFib(int n) {
    if (n == 0 || n == 1) {
        return n;
    } else {
        return recursiveFib(n - 1) + recursiveFib(n - 2);
    }
}

int dynamicFib(int n) {
    std::vector<int> v;
    if (n <= 1)
        return n;
    v.push_back(0);
    v.push_back(1);
    for (int i=2; i<=n; i++) {
        v.push_back(v[i-1] + v[i-2]);
    }
    return v[n];
}

int test(int n, std::vector<int>& list1, std::vector<int>& list2, std::fstream& file) {
    typedef std::chrono::high_resolution_clock Clock;
    auto recurStart = Clock::now();
    list1.push_back(recursiveFib(n));
    auto recurStop = Clock::now();
    auto recurElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(recurStop - recurStart).count();
    auto dynStart = Clock::now();
    list2.push_back(dynamicFib(n));
    auto dynStop = Clock::now();
    auto dynElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(dynStop - dynStart).count();
    file << n << "," << list1.back() << "," << recurElapsed << "," << dynElapsed << ",tbd,tbd\n";
    return list1.back();
}

int main() {
    std::fstream file;
    file.open("Fibonacci_Time.csv", std::ios::out);
    std::string header = "n,f(n),T1: Time spent on the recursive algorithm (nanoseconds),T2: Time spent on the DP algorithm (nanoseconds),Value of (2^n)/n,Value of T1/T2\n";
    file << header;
    std::vector<int> recursive;
    std::vector<int> dynamic;
    std::string input;
    while (true) {
        std::cout << "Use recursive algorithm or dynamic programming algorithm, or exit?\n";
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower); // Convert input to lower case for case-nonsensitive input
        if (input == "exit") {
            file.close();
            return 0;
        }
        std::cout << "Enter an index.\n";
        std::cin >> input;
        try {
            int n = std::stoi(input); // Attempt to convert input to int.
            if (n < 0) {
                std::cout << "Invalid option";
                continue;
            }
            int result = test(n, recursive, dynamic, file);
            std::cout << "Index " << n << " in the Fibonacci sequence is: " << result << std::endl;
        }
        catch (std::invalid_argument e) {
            std::cout << "Invalid option.\n";
        }
    }
}

