#include <iostream>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <chrono>

void quicksort(std::vector<int>& arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    #pragma omp parallel sections
    {
    #pragma omp section
        {
            if (left < j)
                quicksort(arr, left, j);
        }

        #pragma omp section
        {
            if (i < right)
                quicksort(arr, i, right);
        }
    }
}

int main() {
    int sizes[] = { 1000, 10000, 100000, 1000000 };
    int num_sizes = sizeof(sizes) / sizeof(int);

    int num_threads[] = { 2, 4, 6, 8, 10, 20 };
    int num_threads_count = sizeof(num_threads) / sizeof(int);

    for (int i = 0; i < num_sizes; i++) {
        std::vector<int> arr(sizes[i]);
        std::generate(arr.begin(), arr.end(), [] { return rand() % 100; });

        for (int j = 0; j < num_threads_count; j++) {
            omp_set_num_threads(num_threads[j]);

            auto start = std::chrono::high_resolution_clock::now();
            #pragma omp parallel
            {
            #pragma omp single nowait
                {
                    quicksort(arr, 0, arr.size() - 1);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed_seconds = end - start;

            std::cout << "Size: " << sizes[i] << ", \tThreads: " << num_threads[j]
                << ", \tTime: " << elapsed_seconds.count() << "s" << std::endl;
        }
    }

    return 0;
}
