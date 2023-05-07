#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<iostream>
#include<fstream>

using namespace std;

double norm_1(double** matrix, int m, int n) {
    double norm = -1;
    for (int j = 0; j < n; j++) {
        double sum = 0;
        for (int i = 0; i < m; i++) {
            sum += matrix[i][j] >= 0 ? matrix[i][j] : -matrix[i][j];
            if (sum > norm) norm = sum;
        }
        return norm;
    }
}

double norm1_parallel(double** matrix, int m, int n, int thread_count) {
    double norm = -1;
#pragma omp parallel for num_threads (thread_count)
    for (int j = 0; j < n; j++) {
        double sum = 0;
        for (int i = 0; i < m; i++) {
            sum += matrix[i][j] >= 0 ? matrix[i][j] : -matrix[i][j];
            if (sum > norm)
#pragma omp critical
            {
                if (sum > norm) norm = sum;
            }
        }
        return norm;
    }
}

int main()
{
    const int DIM_COUNT = 7;
    int dims[DIM_COUNT] = { 100, 500, 1000, 2000, 3000, 5000, 10000 };
    const int THREADS_COUNT = 6;
    int threads[THREADS_COUNT] = { 2, 4, 6, 8, 10, 20 };
    double** matrix = new double* [dims[DIM_COUNT - 1]];

    for (int i = 0; i < dims[DIM_COUNT - 1]; i++) {
        matrix[i] = new double[dims[DIM_COUNT - 1]];
        for (size_t j = 0; j < dims[DIM_COUNT - 1]; j++)
            matrix[i][j] = 2 * (double)rand() / (RAND_MAX + 1) - 1;
    }

    for (int d = 0; d < DIM_COUNT; d++) {
        int m = dims[d], n = dims[d];
        double norm = norm_1(matrix, m, n);
        printf("\tIn serial mode ||A||_1 = %e; \n", norm);
        for (int t = 0; t < THREADS_COUNT; t++) {
            norm = norm1_parallel(matrix, m, n, threads[t]);
            printf("\tFor %d threads ||A||_1 = %e; \n", threads[t], norm);
        }
    }

    for (size_t i = 0; i < dims[DIM_COUNT - 1]; i++)
        delete[] matrix[i];
    delete[] matrix;
}
