#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

void merge(double* a, int first, int mid, int last) {
    //int merged[n*2];
    auto s1 = mid - first + 1;
    auto s2 = last - mid;
    auto* a1 = new double[s1];
    auto* a2 = new double[s2];

    for (auto i = 0; i < s1; i++)
        a1[i] = a[first + i];
    for (auto j = 0; j < s2; j++)
        a2[j] = a[mid + 1 + j];
    int p1 = 0; int p2 = 0; int pm = first;
    while (p1 < s1 && p2 < s2) {
        if (a1[p1] <= a2[p2]) {
            a[pm] = a1[p1];
            p1++; pm++;
        }
        else {
            a[pm] = a2[p2];
            p2++; pm++;
        }
    }
    while (p1 < s1) {
        a[pm] = a1[p1];
        pm++; p1++;
    }
    while (p2 < s2) {
        a[pm] = a2[p2];
        pm++; p2++;
    }
    delete[] a1;
    delete[] a2;
}

void mergeSort(double* a, int first, int last) {
    if (first >= last) {
        return;
    }
    else {
        auto mid = first + (last - first) / 2;

        mergeSort(a, first, mid);
        mergeSort(a, mid + 1, last);
        merge(a, first, mid, last);
    }

}

//Populate arrays
void populateArrays(double* unsorted, double* sorted, int len) {
    for (int j = 0; j < len; j++) {
        //Generate random real number to the 1,000th place from 0-999.999 by summing an integer and decimal component
        unsorted[j] = (rand() % 999 + 1) + (rand() % 1000 + 1)/1000.0;
        sorted[j] = unsorted[j];
    }
    mergeSort(sorted, 0, len - 1);
}

//Adding spacers makes outputs more readable
void spacer() {
    std::cout << "\n-=====================================-\n\n";
}

int main() {
    srand(time(nullptr));

    int len;
    int const numberOfArrays = 9;

    //The user needs to see both the unsorted and sorted copy at any time, so both need to be stored
    //By making the arrays into double pointers, the lengths of each inner array can be different,
    //as they can be defined just before being populated.
    //Mainly, it avoids running out of stack memory by allowing the use of new
    double** arrUnsorted = nullptr;
    arrUnsorted = new double*[numberOfArrays];
    double** arrSorted = nullptr;
    arrSorted = new double* [numberOfArrays];

    std::stringstream csv;
    csv << "\"Input size n for Array_i\",\"Value of n*logn\",\"Time spent (microseconds)\",\"Value of (n*logn)/time\",\n";
    for (int i = 0; i < numberOfArrays; i++) {
        len = (i + 1) * 1200;
        arrUnsorted[i] = new double[len];
        arrSorted[i] = new double[len];
        typedef std::chrono::high_resolution_clock Clock;
        auto start = Clock::now();
        populateArrays(arrUnsorted[i], arrSorted[i], len);
        auto stop = Clock::now();
        // Nanosecond precision may not work on some systems
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        
        float n = len * log2(len) / elapsed;
        float m = floor(log10(n));
        n = round(n / pow(10, m));

        csv << len << "," << len * log2(len) << "," << elapsed << "," << n << "e" << m << ",\n";
    }
    std::ofstream file("Mergesort_Time.csv", std::ofstream::trunc);
    file << csv.str();
    file.close();

    int userIn;
    int index;
    while (true) {
        std::cout << "Which array do you want to see? Type a number 1-9 and press enter to select an array, or type 0 and press enter to quit." << std::endl;
        std::cin >> userIn;
        spacer();

        if (userIn < 0 || userIn>9) {
            std::cout << "Invalid input. Try again." << std::endl;
        }
        else if (userIn == 0) break;
        else {
            index = userIn-1;
            std::cout << "Printing unsorted values of Array_" << userIn << "." << std::endl;
            for (int j = 0; j < userIn*1200; j++) {
                std::cout << arrUnsorted[index][j] << " ";
            }
            std::cout << std::endl << "End of unsorted array " << userIn << std::endl;

            spacer();
            std::cout << "Printing unsorted values of Array_" << userIn << "." << std::endl;
            for (int j = 0; j < userIn*1200; j++) {
                std::cout << arrSorted[index][j] << " ";
            }
            std::cout << std::endl << "End of sorted array " << userIn << std::endl;
        }
        spacer();
    }

    return 0;
}
