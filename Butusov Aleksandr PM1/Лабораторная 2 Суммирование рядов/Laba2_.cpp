#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float next_sin_term(float x, int n, float prev_term) {
    return -prev_term * x * x / ((2 * n) * (2 * n + 1));
}

float next_cos_term(float x, int n, float prev_term) {
    return -prev_term * x * x / ((2 * n - 1) * (2 * n));
}

float next_exp_term(float x, int n, float prev_term) {
    return prev_term * x / n;
}

float next_log_term(float x, int n, float prev_term) {
    return -prev_term * x / (n + 1);
}

// Прямое суммирование
float direct_sum(float x, float (*next_term)(float, int, float), float first_term, int max_terms) {
    float sum = first_term;
    float term = first_term;
    for (int n = 1; n < max_terms; n++) {
        term = next_term(x, n, term);
        sum += term;
    }
    return sum;
}

// Обратное суммирование
float reverse_sum(float x, float (*next_term)(float, int, float), float first_term, int max_terms) {
    // Выдение память для массива terms
    float* terms = (float*)malloc(max_terms * sizeof(float));
    if (terms == NULL) {
        // Обработка ошибки выделения памяти
        return 0.0; 
    }

    terms[0] = first_term;
    for (int n = 1; n < max_terms; n++) {
        terms[n] = next_term(x, n, terms[n - 1]);
    }

    float sum = 0.0;
    for (int n = max_terms - 1; n >= 0; n--) {
        sum += terms[n];
    }

    // Освобождение выделенную память
    free(terms);

    return sum;
}

// Попарное суммирование

float pairwise_sum(float x, float (*next_term)(float, int, float), float first_term, int max_terms) {
    float* terms = (float*)malloc(max_terms * sizeof(float));
    if (terms == NULL) {
        return 0.0; 
    }

    // Заполняем массив terms
    terms[0] = first_term;
    for (int n = 1; n < max_terms; n++) {
        terms[n] = next_term(x, n, terms[n - 1]);
    }


    // Выполняем попарное суммирование
    int size = max_terms;
    while (size > 1) {
        int new_size = (size + 1) / 2;
        for (int i = 0; i < new_size; i++) {
            if (2 * i + 1 < size) {
                terms[i] = terms[2 * i] + terms[2 * i + 1];
            }
            else {
                terms[i] = terms[2 * i];
            }
        }
        size = new_size;
    }

    float result = terms[0];

    free(terms);

    return result;
}

int main() {
    float x;
    int max_terms;

    printf("Введите значение x: ");
    scanf("%f", &x);


    printf("Введите количество слагаемых в сумме: ");
    scanf("%d", &max_terms);

    // Точные значения из библиотеки math.h
    float exact_sin = sinf(x);
    float exact_cos = cosf(x);
    float exact_exp = expf(x);
    float exact_log = log1pf(x); // log1pf(x) = log(1 + x)

    float sin_direct = direct_sum(x, next_sin_term, x, max_terms);
    float sin_reverse = reverse_sum(x, next_sin_term, x, max_terms);
    float sin_pairwise = pairwise_sum(x, next_sin_term, x, max_terms);
    printf("sin(%f):\n", x);
    printf("  Прямое суммирование: %f (ошибка: %e)\n", sin_direct, fabsf(sin_direct - exact_sin));
    printf("  Обратное суммирование: %f (ошибка: %e)\n", sin_reverse, fabsf(sin_reverse - exact_sin));
    printf("  Попарное суммирование: %f (ошибка: %e)\n", sin_pairwise, fabsf(sin_pairwise - exact_sin));
    printf("  Точное значение (math.h): %f\n", exact_sin);

    float cos_direct = direct_sum(x, next_cos_term, 1.0, max_terms);
    float cos_reverse = reverse_sum(x, next_cos_term, 1.0, max_terms);
    float cos_pairwise = pairwise_sum(x, next_cos_term, 1.0, max_terms);
    printf("cos(%f):\n", x);
    printf("  Прямое суммирование: %f (ошибка: %e)\n", cos_direct, fabsf(cos_direct - exact_cos));
    printf("  Обратное суммирование: %f (ошибка: %e)\n", cos_reverse, fabsf(cos_reverse - exact_cos));
    printf("  Попарное суммирование: %f (ошибка: %e)\n", cos_pairwise, fabsf(cos_pairwise - exact_cos));
    printf("  Точное значение (math.h): %f\n", exact_cos);

    float exp_direct = direct_sum(x, next_exp_term, 1.0, max_terms);
    float exp_reverse = reverse_sum(x, next_exp_term, 1.0, max_terms);
    float exp_pairwise = pairwise_sum(x, next_exp_term, 1.0, max_terms);
    printf("exp(%f):\n", x);
    printf("  Прямое суммирование: %f (ошибка: %e)\n", exp_direct, fabsf(exp_direct - exact_exp));
    printf("  Обратное суммирование: %f (ошибка: %e)\n", exp_reverse, fabsf(exp_reverse - exact_exp));
    printf("  Попарное суммирование: %f (ошибка: %e)\n", exp_pairwise, fabsf(exp_pairwise - exact_exp));
    printf("  Точное значение (math.h): %f\n", exact_exp);

    if (x > -1 && x < 1) { // Ряд для log(1+x) сходится только при |x| < 1
        float log_direct = direct_sum(x, next_log_term, x, max_terms);
        float log_reverse = reverse_sum(x, next_log_term, x, max_terms);
        float log_pairwise = pairwise_sum(x, next_log_term, x, max_terms);
        printf("log(1+%f):\n", x);
        printf("  Прямое суммирование: %f (ошибка: %e)\n", log_direct, fabsf(log_direct - exact_log));
        printf("  Обратное суммирование: %f (ошибка: %e)\n", log_reverse, fabsf(log_reverse - exact_log));
        printf("  Попарное суммирование: %f (ошибка: %e)\n", log_pairwise, fabsf(log_pairwise - exact_log));
        printf("  Точное значение (math.h): %f\n", exact_log);
    }
    else {
        printf("log(1+%f) не может быть вычислен с использованием данного ряда (|x| должен быть < 1).\n", x);
    }

    return 0;
}