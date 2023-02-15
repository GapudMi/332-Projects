#include <iostream>
#include <time.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <functional>
#include <numeric>
#include <algorithm>

// Brute force algorithm v1
int bruteForce1(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    int gcd;
    for (int i = 1; i <= low; i++) {
        if (low % i == 0 && high % i == 0)
            gcd = i;
    }
    return gcd;
}

// Brute force algorithm v2
int bruteForce2(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    for (int i = low; i > 0; i--) {
        if (low % i == 0 && high % i == 0)
            return i;
    }
}

// Euclid's algorithm
int originalEuclid(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    int remainder = -1;
    while (remainder != 0) {
        remainder = high % low;
        high = low;
        low = remainder;
    }
    return high;
}

// "Improved?" Euclid's algorithm
int v2Euclid(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    int remainder = -1;
    while (remainder != 0) {
        remainder = high - low;
        if (remainder >= low) {
            remainder = remainder - low;
            if (remainder >= low) {
                remainder = remainder - low;
                if (remainder >= low) {
                    remainder = high - low * (high / low);
                }
            }
        }
        high = low;
        low = remainder;
    }
    return high;
}


// Scalable function to test each algorithm, output its stats to file
std::vector<int> test(int arrA[], int arrB[], std::function<int(int, int)> func, std::string testName) {
    // Initialize values
    int timeLow = INT32_MAX;
    int timeHigh = 0;
    int totalTime = 0;
    std::vector<int> times;
    // Initialize statistics CSV file
    std::stringstream csv;
    csv << "Number One,Number Two,Their GCD,Time Spent (Nanoseconds)\n";
    for (int i = 0; i < 1000; i++) {
        // Measure time of each calculation
        typedef std::chrono::high_resolution_clock Clock;
        auto start = Clock::now();
        int gcd = func(arrA[i], arrB[i]);
        auto stop = Clock::now();
        // Nanosecond precision may not work on some systems
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        times.push_back(elapsed);
        totalTime += elapsed;
        // Update min and max times
        if (elapsed < timeLow)
            timeLow = elapsed;
        if (elapsed > timeHigh)
            timeHigh = elapsed;
        // Output to string stream
        csv << arrA[i] << "," << arrB[i] << "," << gcd << "," << elapsed << "\n";
    }

    // Output results file
    std::ofstream file(testName + "_Results.csv", std::ofstream::trunc);
    file << csv.str();
    file.close();
    int timesSorted[1000];
    std::copy(std::begin(times), std::end(times), std::begin(timesSorted));
    // Sort list of times in order to take median
    std::sort(std::begin(timesSorted), std::end(timesSorted));
    int median = (times[499] + times[500]) / 2;

    // Generate statistics CSV
    std::stringstream stats;
    stats << "Statistics,Nanoseconds\n";
    stats << "Maximum Time," << timeHigh << "\n";
    stats << "Minimum Time," << timeLow << "\n";
    stats << "Average Time," << totalTime / 1000.0 << "\n";
    stats << "Median Time," << median << "\n";
    // Output statistics file
    std::ofstream statsFile(testName + "_Statistics.csv", std::ofstream::trunc);
    statsFile << stats.str();
    statsFile.close();
    return times;
}

int vectorMean(std::vector<int> v) {
    return std::reduce(v.begin(), v.end()) / v.size();
}

void compare(std::vector<int> v1, std::vector<int> v2, std::string a, std::string b) {
    std::vector<int> save;
    for (int i = 0; i < v1.size(); i++) {
        if (v2[i] < v1[i]) {
            save.push_back((v1[i] - v2[i]));
        }
    }
    std::cout << b << " outperformed " << a << " in " << save.size() << " pairs, saving an average of " << vectorMean(save) << " ns.\n";
}



int main() {
    srand(time(NULL));
    // Generate 1000 pairs of ints
    int arrA[1000];
    int arrB[1000];
    for (int i = 0; i < 1000; i++) {
        arrA[i] = rand() % 1000 + 1;
        arrB[i] = rand() % 1000 + 1;
    }
    // Run each test
    std::vector<int> bf1times = test(arrA, arrB, bruteForce1, "BF_v1");
    std::vector<int> bf2times = test(arrA, arrB, bruteForce2, "BF_v2");
    std::vector<int> oetimes = test(arrA, arrB, originalEuclid, "OE");
    std::vector<int> setimes = test(arrA, arrB, v2Euclid, "SE");

    // Compare results
    compare(bf1times, bf2times, "brute-force V1", "Brute-force v2");
    compare(bf1times, oetimes, "brute-force v1", "Euclid (original)");
    compare(bf2times, oetimes, "brute-force v2", "Euclid (original)");
    compare(oetimes, setimes, "Euclid (original)", "Euclid V2");
    compare(bf1times, setimes, "brute-force v1", "Euclid V2");
    compare(bf2times, setimes, "brute-force v2", "Euclid V2");

    // Allow results to be read.
    std::cout << "Press enter to close.";
    std::cin.get();
}




