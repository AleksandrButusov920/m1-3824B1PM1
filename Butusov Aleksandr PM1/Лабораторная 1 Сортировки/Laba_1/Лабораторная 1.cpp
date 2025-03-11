#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "locale.h"
#include "Sorts.h"
#include "math.h"
#include "UsefulFunctions.h"
#include "string.h"
int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	int n; double z; double z2; int decision1;
	int a = -100000; int b = 100000;
	printf("Введите количество элементов исходного массива (не более 50 тысяч элементов)\n>> ");
	scanf_s("%i", &n);
	if ((n < 1) || (n > 50000)) {
		printf("\nОшибка!");
		return 0;
	}
	double* arr = (double*)malloc(n * sizeof(double));
	double* brr = (double*)malloc(5 * n * sizeof(double));
	CreateRandDouble(arr, n, a,b);
	CreateRandDouble(brr, 5 * n, a, b);
	printf("\nВыберите сортировку:\n1 - Сортировка пузырьком\n2 - Сортировка вставками\n3 - Сортировка расчёской\n4 - Сортировка слиянием\n5 - Поразрядная сортировка\n>> ");
	scanf_s("%i", &decision1);
	if ((decision1 < 1) || (decision1 > 5)) {
		printf("\nОшибка!");
		return 0;
	}
	else {
		decision(decision1, arr,brr, n);
		int decision2;
		printf("0 - Дополнительная информация о массиве\n>> ");
		scanf_s("%i", &decision2);
		if (decision2 != 0) {
			return 0;
		}
		else {
			printf("\nВ исходном массиве %i случайных элементов типа double от %i до %i \n", n, a, b);
		}
		//OutputArr(arr, n);
		//return 0;
	}
}