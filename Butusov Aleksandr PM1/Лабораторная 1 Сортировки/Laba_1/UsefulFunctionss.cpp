#include "Sortss.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "locale.h"
#include "math.h"
void swap(double& a, double& b) {
	double tmp = a;
	a = b;
	b = tmp;
}
void CreateRandDouble(double arr[], int n, int a, int b) {
	for (int i = 0; i < n; i++) {
		arr[i] = (double)rand() / RAND_MAX * (b - a) + a;
	}
}
void OutputArr(double a[], int n) {
	printf("\n");
	for (int i = 0; i < n; i++) {
		printf("%lf ", a[i]);
	}
}

#define check_operations

#ifdef check_operations

extern unsigned long long bubblechecks = 0;
extern unsigned long long combchecks = 0;
extern unsigned long long mergechecks = 0;
extern unsigned long long radixchecks = 0;
#endif 


void infoBubble(int n, double arr[]) {
	clock_t start = clock();
	Bubble(arr, n);
	clock_t finish = clock();
	double z = ((double)(finish - start)) / CLOCKS_PER_SEC;
	printf("%i элементов   |  %lf с  |  ", n, z);
#ifdef check_operations
	printf("количество операций % llu", bubblechecks);
#endif
	//OutputArr(arr, n);
	printf("\n\n");
}

void infoComb(int n, double arr[]) {
	clock_t start = clock();
	Comb(arr, n);
	clock_t finish = clock();
	double z = ((double)(finish - start)) / CLOCKS_PER_SEC;
	printf("%i элементов   |  %lf с  |  ", n, z);
#ifdef check_operations
	printf("количество операций % llu", combchecks);
#endif
	//OutputArr(arr, n);
	printf("\n\n");
}

void infoMerge(int n, double arr[], double brr[]) {
	clock_t start = clock();
	Mergesort(arr, n, brr);
	clock_t finish = clock();
	double z = ((double)(finish - start)) / CLOCKS_PER_SEC;
	printf("%i элементов   |  %lf с  |  ", n, z);
#ifdef check_operations
	printf("количество операций % llu", mergechecks);
#endif
	//OutputArr(arr, n);
	printf("\n\n");
}

void infoRadix(int n, double arr[]) {
	clock_t start = clock();
	Radix(arr, n);
	clock_t finish = clock();
	double z = ((double)(finish - start)) / CLOCKS_PER_SEC;
	printf("%i элементов   |  %lf с  |  ", n, z);
#ifdef check_operations
	printf("количество операций % llu", radixchecks);
#endif
	//OutputArr(arr, n);
	printf("\n\n");

}
void decision(int decision1, int n, double arr1[], double arr2[], double arr3[], double arr4[], double arr5[], double arr6[], double brr1[], double brr2[], double brr3[], double brr4[], double brr5[], double brr6[]) {
	if (decision1 == 1) {
		infoBubble(n, arr1);
		infoBubble(2 * n, arr2);
		infoBubble(4 * n, arr3);
		infoBubble(8 * n, arr4);
		infoBubble(16 * n, arr5);
		infoBubble(32 * n, arr6);
	}
	
	if (decision1 == 2) {
		infoComb(n, arr1);
		infoComb(2 * n, arr2);
		infoComb(4 * n, arr3);
		infoComb(8 * n, arr4);
		infoComb(16 * n, arr5);
		infoComb(32 * n, arr6);
	}
	//ƒл€ сортировки сли€нием нужна дополнительна€ пам€ть - массив с таким же 
	// количеством элементов, что и изначальный (brr - дополнительный массив), основной arr
	if (decision1 == 3) {
		infoMerge(n, arr1, brr1);
		infoMerge(2 * n, arr2, brr2);
		infoMerge(4 * n, arr3, brr3);
		infoMerge(8 * n, arr4, brr4);
		infoMerge(16 * n, arr5, brr5);
		infoMerge(32 * n, arr6, brr6);
	}
	if (decision1 == 4) {
		infoRadix(n, arr1);
		infoRadix(2 * n, arr2);
		infoRadix(4 * n, arr3);
		infoRadix(8 * n, arr4);
		infoRadix(16 * n, arr5);
		infoRadix(32 * n, arr6);
	}
}
