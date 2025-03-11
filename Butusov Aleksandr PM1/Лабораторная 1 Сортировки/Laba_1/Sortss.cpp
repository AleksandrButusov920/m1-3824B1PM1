#include "Sortss.h"
#include "UsefulFunctionss.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void Bubble(double a[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
#ifdef check_operations
            bubblechecks += 2;  
#endif
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void Comb(double a[], int n) {
    int step = n / 2;
    for (; step > 1; step /= 2) {
        for (int i = 0; i < step; i++) {
            for (int j = i; j + step < n; j += step) {
#ifdef check_operations
                combchecks += 2;  
#endif
                if (a[j] > a[j + step]) {
                    swap(a[j], a[j + step]);
                }
            }
        }
    }
    int i = 0; bool t = true;
    while (t) {
        t = false;
        for (int j = 0; j < n - i - 2; j++) {
#ifdef check_operations
            combchecks += 2;  
#endif
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                t = true;
            }
        }
        i = i + 1;
    }
}

void Merge(double* a, int n, double* b, int m, double* c) {
    int j = 0, k = 0;
    for (int i = 0; i < n + m; i++) {
        if (j == n) {
#ifdef check_operations
            mergechecks += 1;
#endif
            c[i] = b[k++];
        }
        else {
#ifdef check_operations
            mergechecks += 1;
#endif
            if (k == m) {
#ifdef check_operations
                mergechecks += 1;
#endif
                c[i] = a[j++];
            }
            else {
#ifdef check_operations
                mergechecks += 1;
#endif
                if (a[j] < b[k]) {
#ifdef check_operations
                    mergechecks += 1;
#endif
                    c[i] = a[j++];
                }
                else {
#ifdef check_operations
                    mergechecks += 1;
#endif
                    c[i] = b[k++];
                }
            }
        }
    }
}

void Mergesort(double* a, int n, double* b) {
    if (n == 1) { return; }
    Mergesort(&a[0], n / 2, &b[0]);
    Mergesort(&a[n / 2], n - (n / 2), &b[n / 2]);
    Merge(&a[0], n / 2, &a[n / 2], n - (n / 2), &b[0]);
    for (int i = 0; i < n; i++) {
#ifdef check_operations
        mergechecks += 1;   
#endif
        a[i] = b[i];
    }
}
void count(double mas[], unsigned int counter[], int n, int offset) {
    unsigned char* b = (unsigned char*)mas;
    b = b + offset;
    memset(counter, 0, 256 * sizeof(unsigned int));
    for (int i = 0; i < n; i++) {
        counter[*b]++;
        b += sizeof(double);
    }
    int s = 0, tmp;
    for (int i = 0; i < 256; i++) {
#ifdef check_operations
        radixchecks += 1;  
#endif
        tmp = counter[i];
        counter[i] = s;
        s += tmp;
    }
}

void Radix(double a[], int n) {
    unsigned int counter[256];
    double* b = (double*)malloc(n * sizeof(double));
    unsigned char* c = (unsigned char*)a;
    for (int i = 0; i < sizeof(double); i++) {
#ifdef check_operations
        radixchecks += 1;   
#endif
        count(a, counter, n, i);
        for (int j = 0; j < n; j++) {
#ifdef check_operations
            radixchecks += 1;   
#endif
            b[counter[*(c + i + j * sizeof(double))]++] = a[j];
        }
        for (int j = 0; j < n; j++) {
#ifdef check_operations
            radixchecks += 1;   
#endif
            a[j] = b[j];
        }
    }
    int index;
    for (index = 0; index < n; index++) {
#ifdef check_operations
        radixchecks += 1;  
#endif
        if (b[index] < 0.0) {
            break;
        }
    }
    for (int i = 0; i < n - index; i++) {
#ifdef check_operations
        radixchecks += 1;  
#endif
        a[i] = b[n - 1 - i];
    }
    for (int i = n - index; i < n; i++) {
#ifdef check_operations
        radixchecks += 1;  
#endif
        a[i] = b[i - n + index];
    }
    free(b);
}