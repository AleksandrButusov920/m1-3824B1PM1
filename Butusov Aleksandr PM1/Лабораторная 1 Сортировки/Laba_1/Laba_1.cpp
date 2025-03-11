#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "locale.h"
#include "Sortss.h"
#include "math.h"
#include "UsefulFunctionss.h"
#include "string.h"
int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int n; double z; double z2; int decision1;
	int a = -1000000; int b = 1000000;
	printf("Введите количество элементов исходного массива \n>> ");
	scanf_s("%i", &n);
	if ((n < 1) || (n > 100000)) {
		printf("\nОшибка!");
		return 0;
	}
	printf("\nВыберите сортировку:\n1 - Сортировка пузырьком\n2 - Сортировка расчёской\n3 - Сортировка слиянием\n4 - Поразрядная сортировка\n>> ");
	scanf_s("%i", &decision1);
	if ((decision1 < 1) || (decision1 > 4)) {
		printf("\nОшибка!");
		return 0;
	}
	else {
		//для слияния. Создаём 6 массивов для одной сортировки с одним элементом (пустых массивов)
		double* brr1 = (double*)malloc(sizeof(double));
		double* brr2 = (double*)malloc(sizeof(double));
		double* brr3 = (double*)malloc(sizeof(double));
		double* brr4 = (double*)malloc(sizeof(double));
		double* brr5 = (double*)malloc(sizeof(double));
		double* brr6 = (double*)malloc(sizeof(double));
		if (decision1 == 3) {
			double* brr1 = (double*)malloc(n * sizeof(double));
			double* brr2 = (double*)malloc(2 * n * sizeof(double));
			double* brr3 = (double*)malloc(4 * n * sizeof(double));
			double* brr4 = (double*)malloc(8 * n * sizeof(double));
			double* brr5 = (double*)malloc(16 * n * sizeof(double));
			double* brr6 = (double*)malloc(32 * n * sizeof(double));
		}
		double* arr1 = (double*)malloc(n * sizeof(double));
		double* arr2 = (double*)malloc(2 * n * sizeof(double));
		double* arr3 = (double*)malloc(4 * n * sizeof(double));
		double* arr4 = (double*)malloc(8 * n * sizeof(double));
		double* arr5 = (double*)malloc(16 * n * sizeof(double));
		double* arr6 = (double*)malloc(32 * n * sizeof(double));
		CreateRandDouble(arr1, n, a, b);
		CreateRandDouble(arr2, 2 * n, a, b);
		CreateRandDouble(arr3, 4 * n, a, b);
		CreateRandDouble(arr4, 8 * n, a, b);
		CreateRandDouble(arr5, 16 * n, a, b);
		CreateRandDouble(arr6, 32 * n, a, b);
		
		decision(decision1, n, arr1, arr2, arr3, arr4, arr5, arr6, brr1, brr2, brr3, brr4, brr5, brr6);
		int decision2;
		printf("\n0 - Дополнительная информация о массиве\n>> ");
		scanf_s("%i", &decision2);
		if (decision2 != 0) {
			return 0;
		}
		else {
			printf("\nВ исходном массиве %i случайных элементов типа double от %i до %i \n", n, a, b);
		}
		
	}
}