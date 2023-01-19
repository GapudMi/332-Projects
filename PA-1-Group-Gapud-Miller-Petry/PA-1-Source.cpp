#include <iostream>
#include <time.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <functional>

int bruteForce1(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    int gcd;
    for (int i=1; i<=low; i++) {
        if (low % i == 0 && high % i == 0)
            gcd = i;
    }
    return gcd;
}

int bruteForce2(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a, b);
    for (int i=low; i>0; i--) {
        if (low % i == 0 && high % i == 0)
            return i;
    }
}

int originalEuclid(int a, int b) {
    int low = std::min(a, b);
    int high = std::max(a,b);
    int remainder = -1;
    while (remainder != 0) {
        remainder = high % low;
        high = low;
        low = remainder;
    }
    return high;
}

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
                    remainder = high - low * (high/low);
                }
            }
        }
        high = low;
        low = remainder;
    }
    return high;
}



void test(int arrA[], int arrB[], std::function<int(int, int)> func, std::string testName) {
    // Initialize values
    int timeLow = 2147483647;
    int timeHigh = 0;
    int totalTime = 0;
    int times[1000];
    std::stringstream csv;
    csv << "Number One,Number Two,Their GCD,Time Spent (Nanoseconds)\n";     // CSV header
    for (int i=0;i<1000;i++) {
        // Measure time of each calculation
        typedef std::chrono::high_resolution_clock Clock;
        auto start = Clock::now();
        int gcd = func(arrA[i], arrB[i]);
        auto stop = Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        times[i] = elapsed;
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
    std::sort(std::begin(times), std::end(times));
    int median = (times[499]+times[500])/2;
    // Generate statistics file
    std::stringstream stats;
    stats << "Statistics,Microseconds\n";
    stats << "Maximum Time," << timeHigh << "\n";
    stats << "Minimum Time," << timeLow << "\n";
    stats << "Average Time," << totalTime/1000 << "\n";
    stats << "Median Time," << median << "\n";
    // Output statistics file
    std::ofstream statsFile(testName + "_Statistics.csv", std::ofstream::trunc);
    statsFile << stats.str();
    statsFile.close();
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
    test(arrA, arrB, bruteForce1, "BF_v1");
    test(arrA, arrB, bruteForce2, "BF_v2");
    test(arrA, arrB, originalEuclid, "OE");
    test(arrA, arrB, v2Euclid, "SE");
}



