#include <iostream>
#include <cmath>
#include <utility>
#include <vector>

void merge(int* a, int first, int mid, int last) {
    //int merged[n*2];
    auto s1 = mid - first + 1;
    auto s2 = last - mid;
    auto *a1 = new int[s1];
    auto *a2 = new int[s2];

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

void mergeSort(int* a, int first, int last){
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
int main() {
    srand(time(NULL));
    int fuckyou[1000];
    for (int i=0; i<1000; i++) {
        fuckyou[i] = rand() % 10000 + 1;
    }
    mergeSort(fuckyou, 0, 999);
    for (int i=0; i<1000; i++) {
        std::cout << fuckyou[i] << " ";
    }




    return 0;
}
