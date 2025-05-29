#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <locale.h>
#include <locale>
#include <random>
#include <algorithm>
#include <chrono>
#include <string>
#include <memory>

using namespace std;

struct OperationCounter {
    size_t arithmetic_operations = 0;
    size_t comparisons = 0;
    size_t assignments = 0;

    void reset() {
        arithmetic_operations = 0;
        comparisons = 0;
        assignments = 0;
    }

    void print() const {
        cout << "Арифметические операции: " << arithmetic_operations << "\n";
        cout << "Операции сравнения: " << comparisons << "\n";
        cout << "Операции присваивания: " << assignments << "\n";
        cout << "Всего операций: "
            << arithmetic_operations + comparisons + assignments << "\n";
    }
};

OperationCounter op_counter;

void setRussianLocale() {
    setlocale(LC_ALL, "Russian");
    locale::global(locale(""));
    cout.imbue(locale("Russian_Russia.1251"));
    cin.imbue(locale("Russian_Russia.1251"));
}

class MatrixException : public runtime_error {
public:
    explicit MatrixException(const string& message)
        : runtime_error(message) {}
};

template <typename T>
class BaseVector {
protected:
    size_t capacity_ = 0;
    size_t size_ = 0;
    T* data_ = nullptr;

public:
    
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    size_t capacity() const { return capacity_; }

    virtual ~BaseVector() {
        delete[] data_;
    }
};


template <typename T>
class Vector : public BaseVector<T> {
private:
    using BaseVector<T>::capacity_;
    using BaseVector<T>::size_;
    using BaseVector<T>::data_;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    // Конструкторы
    Vector() = default;

    explicit Vector(size_t size) {
        capacity_ = size;
        size_ = size;
        data_ = new T[size];
    }

    Vector(size_t size, const T& value) {
        capacity_ = size;
        size_ = size;
        data_ = new T[size];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    // Доступ к элементам
    T& operator[](size_t index) {
        if (index >= size_) throw MatrixException("Индекс вектора выходит за границы");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) throw MatrixException("Индекс вектора выходит за границы");
        return data_[index];
    }

    
    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reallocate(new_size * 2);
        }
        size_ = new_size;
    }

    void resize(size_t new_size, const T& value) {
        size_t old_size = size_;
        resize(new_size);
        for (size_t i = old_size; i < new_size; ++i) {
            data_[i] = value;
        }
    }

    // Добавление элементов
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    // Удаление элементов
    void pop_back() {
        if (size_ == 0) throw MatrixException("Вектор пуст");
        size_--;
    }

    
    void clear() {
        size_ = 0;
    }

    // Итераторы
    T* begin() { return data_; }
    const T* begin() const { return data_; }
    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }

    // Норма вектора
    T norm() const {
        T sum = 0;
        for (size_t i = 0; i < size_; ++i) {
            sum += data_[i] * data_[i];
        }
        return sqrt(sum);
    }

    // Вывод вектора
    void print() const {
        cout << "[ ";
        for (size_t i = 0; i < size_; ++i) {
            cout << data_[i] << " ";
        }
        cout << "]" << endl;
    }
};


template <typename T> class VectorOfVectors;

template <typename T>
struct Solution {
    Vector<T> particular;
    VectorOfVectors<T> basis;
    bool is_consistent = false;
    bool has_unique = false;
};

template <typename T>
class VectorOfVectors {
private:
    Vector<Vector<T>> data_;

public:
    // Конструкторы
    VectorOfVectors() = default;
    explicit VectorOfVectors(size_t size) : data_(size) {}
    VectorOfVectors(size_t size, const Vector<T>& value) : data_(size, value) {}

    // Доступ к элементам
    Vector<T>& operator[](size_t index) { return data_[index]; }
    const Vector<T>& operator[](size_t index) const { return data_[index]; }

  
    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }

    void push_back(const Vector<T>& value) { data_.push_back(value); }

    // Итераторы
    typename Vector<Vector<T>>::iterator begin() { return data_.begin(); }
    typename Vector<Vector<T>>::const_iterator begin() const { return data_.begin(); }
    typename Vector<Vector<T>>::iterator end() { return data_.end(); }
    typename Vector<Vector<T>>::const_iterator end() const { return data_.end(); }
};


template <typename T>
class Matrix {
private:
    size_t rows_ = 0;
    size_t cols_ = 0;
    VectorOfVectors<T> data_;

public:
    Matrix() = default;

    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows) {
        for (size_t i = 0; i < rows; ++i) {
            data_[i] = Vector<T>(cols);
        }
    }

    Matrix(size_t rows, size_t cols, const T& value) : rows_(rows), cols_(cols), data_(rows) {
        for (size_t i = 0; i < rows; ++i) {
            data_[i] = Vector<T>(cols, value);
        }
    }

    
    Vector<T>& operator[](size_t index) {
        if (index >= rows_) throw MatrixException("Индекс строки выходит за границы");
        return data_[index];
    }

    const Vector<T>& operator[](size_t index) const {
        if (index >= rows_) throw MatrixException("Индекс строки выходит за границы");
        return data_[index];
    }

    
    size_t GetRows() const { return rows_; }
    size_t GetCols() const { return cols_; }
    bool is_square() const { return rows_ == cols_; }

   
    void swap_rows(size_t i, size_t j) {
        if (i >= rows_ || j >= rows_) throw MatrixException("Индекс строки выходит за границы");
        swap(data_[i], data_[j]);
    }

    
    void print() const {
        for (size_t i = 0; i < rows_; ++i) {
            cout << "[ ";
            for (size_t j = 0; j < cols_; ++j) {
                cout << setw(10) << data_[i][j] << " ";
            }
            cout << "]" << endl;
        }
    }
};

template <typename T>
Matrix<T> GenerateRandomMatrix(size_t rows, size_t cols, T min_val, T max_val) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<T> dist(min_val, max_val);

    Matrix<T> matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
            op_counter.assignments++;
        }
    }
    return matrix;
}

template <typename T>
Vector<T> GenerateRandomVector(size_t size, T min_val, T max_val) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<T> dist(min_val, max_val);

    Vector<T> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = dist(gen);
        op_counter.assignments++;
    }
    return vec;
}

template <typename T>
void CheckSolution(const Matrix<T>& A, const Vector<T>& b, const Solution<T>& solution) {
    const T eps = numeric_limits<T>::epsilon() * static_cast<T>(1000);
    T max_residual = 0;
    T avg_residual = 0;
    size_t residual_count = 0;

    if (!solution.is_consistent) {
        cout << "Система несовместна (нет решений) - проверка пройдена!\n";
        return;
    }

    // Проверка частного решения
    cout << "\nПроверка частного решения:\n";
    for (size_t i = 0; i < A.GetRows(); ++i) {
        T sum = 0;
        for (size_t j = 0; j < A.GetCols(); ++j) {
            sum += A[i][j] * solution.particular[j];
            op_counter.arithmetic_operations += 2;
        }
        T residual = sum - b[i];
        T abs_residual = abs(residual);
        op_counter.arithmetic_operations++;

        if (abs_residual > max_residual) max_residual = abs_residual;
        avg_residual += abs_residual;
        residual_count++;

        cout << "Уравнение " << i + 1 << ": ";
        cout << "Невязка = " << scientific << setprecision(6) << residual;
        cout << " (|" << abs_residual << "|)";

        if (abs_residual > eps) {
            cout << " -> ОШИБКА: превышена допустимая погрешность!";
        }
        cout << "\n";
        op_counter.comparisons++;
    }

    if (residual_count > 0) {
        avg_residual /= residual_count;
        cout << "\nСтатистика невязок для частного решения:\n";
        cout << "Максимальная невязка: " << scientific << max_residual << "\n";
        cout << "Средняя невязка: " << scientific << avg_residual << "\n";
        cout << "Допустимая погрешность: " << scientific << eps << "\n";
    }

    if (solution.has_unique) {
        cout << "\nРешение корректно (единственное)! Максимальная невязка: "
            << scientific << max_residual << "\n";
    }
}

template <typename T>
Solution<T> GaussJordan(Matrix<T> A, Vector<T> b) {
    const size_t n = A.GetRows();
    const size_t m = A.GetCols();

    Solution<T> solution;
    solution.is_consistent = true;
    solution.has_unique = true;

    Matrix<T> Ab(n, m + 1);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            Ab[i][j] = A[i][j];
            op_counter.assignments++;
        }
        Ab[i][m] = b[i];
        op_counter.assignments++;
    }

    Vector<bool> pivot_columns(m, false);
    size_t rank = 0;

    for (size_t col = 0, row = 0; col < m && row < n; ++col) {
        size_t max_row = row;
        T max_val = abs(Ab[row][col]);
        op_counter.arithmetic_operations++;
        for (size_t k = row + 1; k < n; ++k) {
            T current = abs(Ab[k][col]);
            op_counter.arithmetic_operations++;
            if (current > max_val) {
                max_val = current;
                max_row = k;
                op_counter.assignments += 2;
            }
            op_counter.comparisons++;
        }

        if (max_val < numeric_limits<T>::epsilon()) {
            continue;
        }

        if (max_row != row) {
            Ab.swap_rows(row, max_row);
            op_counter.assignments += 3;
        }

        T pivot = Ab[row][col];
        op_counter.assignments++;
        for (size_t j = col; j <= m; ++j) {
            Ab[row][j] /= pivot;
            op_counter.arithmetic_operations++;
            op_counter.assignments++;
        }

        for (size_t i = 0; i < n; ++i) {
            if (i != row && abs(Ab[i][col]) > numeric_limits<T>::epsilon()) {
                T factor = Ab[i][col];
                op_counter.assignments++;
                for (size_t j = col; j <= m; ++j) {
                    Ab[i][j] -= factor * Ab[row][j];
                    op_counter.arithmetic_operations += 2;
                    op_counter.assignments++;
                }
            }
            op_counter.comparisons++;
        }

        pivot_columns[col] = true;
        rank++;
        op_counter.assignments += 2;
    }

    for (size_t i = 0; i < n; ++i) {
        bool all_zero = true;
        op_counter.assignments++;
        for (size_t j = 0; j < m; ++j) {
            if (abs(Ab[i][j]) > numeric_limits<T>::epsilon()) {
                all_zero = false;
                op_counter.assignments++;
                break;
            }
            op_counter.arithmetic_operations++;
        }
        if (all_zero && abs(Ab[i][m]) > numeric_limits<T>::epsilon()) {
            solution.is_consistent = false;
            op_counter.assignments++;
            return solution;
        }
        op_counter.arithmetic_operations++;
        op_counter.comparisons++;
    }

    solution.particular.resize(m);
    op_counter.assignments += m;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (abs(Ab[i][j] - T(1)) < numeric_limits<T>::epsilon()) {
                solution.particular[j] = Ab[i][m];
                op_counter.assignments++;
                break;
            }
            op_counter.arithmetic_operations += 2;
        }
    }

    return solution;
}

template <typename T>
void RunGaussJordan() {
    try {
        auto start_time = chrono::high_resolution_clock::now();
        op_counter.reset();

        int choice;
        cout << "Выберите способ ввода:\n";
        cout << "1 - Ручной ввод\n";
        cout << "2 - Генерация случайных чисел\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice != 1 && choice != 2) {
            throw MatrixException("Некорректный выбор");
        }

        size_t n, m;
        cout << "Введите количество строк матрицы: ";
        cin >> n;
        cout << "Введите количество столбцов матрицы: ";
        cin >> m;

        Matrix<T> A(n, m);
        Vector<T> b(n);

        if (choice == 1) {
            cout << "Введите элементы матрицы A (" << n << "x" << m << "):" << endl;
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    cin >> A[i][j];
                }
            }

            cout << "Введите элементы вектора b (" << n << " элементов):" << endl;
            for (size_t i = 0; i < n; ++i) {
                cin >> b[i];
            }
        }
        else {
            T min_val, max_val;
            cout << "Введите минимальное значение: ";
            cin >> min_val;
            cout << "Введите максимальное значение: ";
            cin >> max_val;

            A = GenerateRandomMatrix<T>(n, m, min_val, max_val);
            b = GenerateRandomVector<T>(n, min_val, max_val);
        }

        cout << "\nИсходная матрица A:" << endl;
        A.print();
        cout << "\nВектор b:" << endl;
        b.print();

        Solution<T> solution = GaussJordan(A, b);

        if (!solution.is_consistent) {
            cout << "\nСистема несовместна (нет решений)\n";
        }
        else {
            cout << "\nСистема имеет единственное решение:\n";
            for (size_t i = 0; i < solution.particular.size(); ++i) {
                cout << "x" << i + 1 << " = " << solution.particular[i] << endl;
            }
        }

        cout << "\nПроверка решения:\n";
        CheckSolution(A, b, solution);

        auto end_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end_time - start_time;

        cout << "\nСтатистика выполнения:\n";
        cout << "Время выполнения: " << elapsed.count() << " секунд\n";
        op_counter.print();
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

int main() {
    setRussianLocale();

    int type_choice;
    cout << "Выберите тип данных:\n";
    cout << "1 - float (одинарная точность)\n";
    cout << "2 - double (двойная точность)\n";
    cout << "Ваш выбор: ";
    cin >> type_choice;

    if (type_choice == 1) {
        cout << "\nИспользуется тип float (одинарная точность)\n";
        RunGaussJordan<float>();
    }
    else if (type_choice == 2) {
        cout << "\nИспользуется тип double (двойная точность)\n";
        RunGaussJordan<double>();
    }
    else {
        cerr << "Некорректный выбор типа данных\n";
        return 1;
    }

    return 0;
}