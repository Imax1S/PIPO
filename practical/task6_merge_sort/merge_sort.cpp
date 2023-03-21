#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cmath>

using namespace std;

template<typename T>
void merge(std::vector<T> &vec, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<T> left(n1);
    std::vector<T> right(n2);

    for (int i = 0; i < n1; i++) {
        left[i] = vec[l + i];
    }
    for (int j = 0; j < n2; j++) {
        right[j] = vec[m + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            vec[k] = left[i];
            i++;
        } else {
            vec[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = right[j];
        j++;
        k++;
    }
}

template<typename T>
void merge_sort(std::vector<T> &vec, int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;
        if (depth == 0) {
            merge_sort(vec, l, m, depth);
            merge_sort(vec, m + 1, r, depth);
        } else {
            std::thread thread_left(merge_sort<T>, std::ref(vec), l, m, depth - 1);
            std::thread thread_right(merge_sort<T>, std::ref(vec), m + 1, r, depth - 1);

            thread_left.join();
            thread_right.join();
        }
        merge(vec, l, m, r);
    }
}

template<typename T>
void merge_sort_parallel(std::vector<T> &vec, int num_threads) {
    int size = vec.size();
    int depth = log2(num_threads);

    merge_sort(vec, 0, size - 1, depth);
}

int main() {
    // Пример использования
    std::vector<int> arr = {3, 5, 1, 6, 8, 2, 9, 4, 7, 0};
    int numThreads = 8;

    cout << "Original array: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    merge_sort_parallel(arr, numThreads);

    cout << "Sorted array: ";
    for (int i: arr) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}