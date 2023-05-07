#include<stdio.h>
#include<omp.h>
#include<iostream>
#include<fstream>

using namespace std;

int main() {
	const int N = 1000;
	int a[N];
	for (int i = 0; i < N; i++) {
		a[i] = 2 * rand() - RAND_MAX;
	}
	int min = 10000;
    #pragma omp parallel for shared(min)
	for (int i = 0; i < N; i++) {
		if (a[i] < min) {
        #pragma omp critical
			{
				if (a[i] < min)
					min = a[i];
			}
		}
	}
	printf("min = %d", min);
	return 0;
}
