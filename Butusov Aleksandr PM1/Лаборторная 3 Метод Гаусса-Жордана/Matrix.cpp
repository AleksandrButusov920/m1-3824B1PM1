#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <locale>
#include <random>
#include <algorithm>
#include <chrono>

// Счётчик операций
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
        std::cout << "Арифметические операции: " << arithmetic_operations << "\n";
        std::cout << "Операции сравнения: " << comparisons << "\n";
        std::cout << "Операции присваивания: " << assignments << "\n";
        std::cout << "Всего операций: "
            << arithmetic_operations + comparisons + assignments << "\n";
    }
};

OperationCounter op_counter;

// Устанавливаем локаль для корректного отображения русских символов
void setRussianLocale() {
    setlocale(LC_ALL, "rus");
}

// Пользовательское исключение для матричных операций
class MatrixException : public std::runtime_error {
public:
    explicit MatrixException(const std::string& message)
        : std::runtime_error(message) {}
};

// Структура для хранения решения
template <typename T>
struct Solution {
    std::vector<T> particular;     // Частное решение
    std::vector<std::vector<T>> basis; // Базис фундаментальной системы решений
    bool is_consistent;       // Совместна ли система
    bool has_unique;          // Единственное ли решение
};

// Класс Vector
template <typename T>
class Vector : public std::vector<T> {
public:
    using std::vector<T>::vector;

    void print() const {
        std::cout << "[ ";
        for (const auto& elem : *this) {
            std::cout << elem << " ";
        }
        std::cout << "]" << std::endl;
    }
};

// Класс Matrix
template <typename T>
class Matrix : public Vector<Vector<T>> {
public:
    using Vector<Vector<T>>::Vector;

    Matrix(size_t rows, size_t cols, const T& initial = T()) {
        this->resize(rows, Vector<T>(cols, initial));
    }

    void print() const {
        for (const auto& row : *this) {
            std::cout << "[ ";
            for (const auto& elem : row) {
                std::cout << std::setw(10) << elem << " ";
            }
            std::cout << "]" << std::endl;
        }
    }

    size_t GetRows() const { return this->size(); }
    size_t GetCols() const { return this->empty() ? 0 : (*this)[0].size(); }
    bool is_square() const { return GetRows() == GetCols(); }
};

// Генерация случайной матрицы
template <typename T>
Matrix<T> GenerateRandomMatrix(size_t rows, size_t cols, T min_val, T max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(min_val, max_val);

    Matrix<T> matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
            op_counter.assignments++;
        }
    }
    return matrix;
}

// Генерация случайного вектора
template <typename T>
std::vector<T> GenerateRandomVector(size_t size, T min_val, T max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(min_val, max_val);

    std::vector<T> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = dist(gen);
        op_counter.assignments++;
    }
    return vec;
}

// Проверка решения
template <typename T>
void CheckSolution(const Matrix<T>& A, const std::vector<T>& b, const Solution<T>& solution) {
    const T eps = std::numeric_limits<T>::epsilon() * static_cast<T>(A.GetRows()) * 1000;

    if (!solution.is_consistent) {
        std::cout << "Система несовместна (нет решений) - проверка пройдена!\n";
        return;
    }

    if (!solution.has_unique) {
        std::cout << "Система имеет бесконечное число решений - проверка частного решения:\n";
    }

    // Проверяем частное решение
    for (size_t i = 0; i < A.GetRows(); ++i) {
        T sum = 0;
        for (size_t j = 0; j < A.GetCols(); ++j) {
            sum += A[i][j] * solution.particular[j];
            op_counter.arithmetic_operations += 2;
        }
        T error = std::abs(sum - b[i]);
        op_counter.arithmetic_operations++;
        T max_val = std::max(T(1), std::max(std::abs(sum), std::abs(b[i])));
        op_counter.comparisons += 2;
        if (error > eps * max_val) {
            throw MatrixException("Решение некорректно: невязка слишком велика");
        }
        op_counter.comparisons++;
    }

    if (solution.has_unique) {
        std::cout << "Решение корректно (единственное)!\n";
    }
    else {
        std::cout << "Частное решение корректно!\n";

        // Проверяем базисные решения
        for (const auto& basis_vec : solution.basis) {
            std::vector<T> zero_b(A.GetRows(), 0);
            for (size_t i = 0; i < A.GetRows(); ++i) {
                T sum = 0;
                for (size_t j = 0; j < A.GetCols(); ++j) {
                    sum += A[i][j] * basis_vec[j];
                    op_counter.arithmetic_operations += 2;
                }
                T error = std::abs(sum - zero_b[i]);
                op_counter.arithmetic_operations++;
                if (error > eps) {
                    throw MatrixException("Базисное решение некорректно");
                }
                op_counter.comparisons++;
            }
        }
        std::cout << "Базисные решения корректны!\n";
    }
}

// Метод Гаусса-Жордана с выбором ведущего элемента
template <typename T>
Solution<T> GaussJordan(Matrix<T> A, std::vector<T> b) {
    const size_t n = A.GetRows();
    const size_t m = A.GetCols();

    Solution<T> solution;
    solution.is_consistent = true;
    solution.has_unique = true;

    // Создаем расширенную матрицу
    Matrix<T> Ab(n, m + 1);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            Ab[i][j] = A[i][j];
            op_counter.assignments++;
        }
        Ab[i][m] = b[i];
        op_counter.assignments++;
    }

    std::vector<bool> pivot_columns(m, false);
    size_t rank = 0;

    // Прямой ход метода Гаусса с выбором ведущего элемента
    for (size_t col = 0, row = 0; col < m && row < n; ++col) {
        // Поиск максимального элемента в текущем столбце
        size_t max_row = row;
        T max_val = std::abs(Ab[row][col]);
        op_counter.arithmetic_operations++;
        for (size_t k = row + 1; k < n; ++k) {
            T current = std::abs(Ab[k][col]);
            op_counter.arithmetic_operations++;
            op_counter.comparisons++;
            if (current > max_val) {
                max_val = current;
                max_row = k;
                op_counter.assignments += 2;
            }
            op_counter.comparisons++;
        }

        // Если максимальный элемент нулевой, пропускаем столбец
        op_counter.comparisons++;
        if (max_val < std::numeric_limits<T>::epsilon()) {
            continue;
        }

        // Меняем строки местами
        op_counter.comparisons++;
        if (max_row != row) {
            std::swap(Ab[row], Ab[max_row]);
            op_counter.assignments += 3;
        }

        // Нормализация текущей строки
        T pivot = Ab[row][col];
        op_counter.assignments++;
        for (size_t j = col; j <= m; ++j) {
            Ab[row][j] /= pivot;
            op_counter.arithmetic_operations++;
            op_counter.assignments++;
            op_counter.comparisons++;
        }

        // Исключение текущей переменной из других строк
        for (size_t i = 0; i < n; ++i) {
            op_counter.comparisons++;
            op_counter.comparisons++;
            if (i != row && std::abs(Ab[i][col]) > std::numeric_limits<T>::epsilon()) {
                T factor = Ab[i][col];
                op_counter.assignments++;
                for (size_t j = col; j <= m; ++j) {
                    Ab[i][j] -= factor * Ab[row][j];
                    op_counter.arithmetic_operations += 2;
                    op_counter.assignments++;
                    op_counter.comparisons++;
                }
            }
            op_counter.comparisons++;
        }

        pivot_columns[col] = true;
        rank++;
        op_counter.assignments += 2;
    }

    // Проверка на совместность системы
    for (size_t i = 0; i < n; ++i) {
        bool all_zero = true;
        op_counter.assignments++;
        for (size_t j = 0; j < m; ++j) {
            op_counter.comparisons++;
            op_counter.comparisons++;
            if (std::abs(Ab[i][j]) > std::numeric_limits<T>::epsilon()) {
                all_zero = false;
                op_counter.assignments++;
                break;
            }
            op_counter.arithmetic_operations++;
        }
        op_counter.comparisons++;
        if (all_zero && std::abs(Ab[i][m]) > std::numeric_limits<T>::epsilon()) {
            solution.is_consistent = false;
            op_counter.assignments++;
            return solution;
        }
        op_counter.arithmetic_operations++;
        op_counter.comparisons++;
    }

    // Находим свободные переменные
    std::vector<size_t> free_vars;
    for (size_t j = 0; j < m; ++j) {
        op_counter.comparisons++;
        op_counter.comparisons++;
        if (!pivot_columns[j]) {
            free_vars.push_back(j);
        }
    }

    // Если есть свободные переменные - система имеет бесконечное число решений
    op_counter.comparisons++;
    if (!free_vars.empty()) {
        solution.has_unique = false;
        op_counter.assignments++;

        // Формируем частное решение
        solution.particular.resize(m, 0);
        op_counter.assignments += m;
        for (size_t i = 0; i < n; ++i) {
            op_counter.comparisons++;
            for (size_t j = 0; j < m; ++j) {
                op_counter.comparisons++;
                op_counter.comparisons++;
                if (std::abs(Ab[i][j] - T(1)) < std::numeric_limits<T>::epsilon()) {
                    solution.particular[j] = Ab[i][m];
                    op_counter.assignments++;
                    break;
                }
                op_counter.arithmetic_operations += 2;
            }
        }

        // Формируем базисные решения для свободных переменных
        for (size_t free_var : free_vars) {
            std::vector<T> basis_vec(m, 0);
            op_counter.assignments += m;
            basis_vec[free_var] = 1;
            op_counter.assignments++;

            for (size_t i = 0; i < n; ++i) {
                op_counter.comparisons++;
                for (size_t j = 0; j < m; ++j) {
                    op_counter.comparisons++;
                    op_counter.comparisons++;
                    if (std::abs(Ab[i][j] - T(1)) < std::numeric_limits<T>::epsilon()) {
                        basis_vec[j] = -Ab[i][free_var];
                        op_counter.arithmetic_operations++;
                        op_counter.assignments++;
                        break;
                    }
                    op_counter.arithmetic_operations += 2;
                }
            }

            solution.basis.push_back(basis_vec);
        }

        return solution;
    }

    // Если нет свободных переменных - единственное решение
    solution.particular.resize(m);
    op_counter.assignments += m;
    for (size_t i = 0; i < n; ++i) {
        op_counter.comparisons++;
        for (size_t j = 0; j < m; ++j) {
            op_counter.comparisons++;
            op_counter.comparisons++;
            if (std::abs(Ab[i][j] - T(1)) < std::numeric_limits<T>::epsilon()) {
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
        auto start_time = std::chrono::high_resolution_clock::now();
        op_counter.reset();

        int choice;
        std::cout << "Выберите способ ввода:\n";
        std::cout << "1 - Ручной ввод\n";
        std::cout << "2 - Генерация случайных чисел\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        op_counter.comparisons++;

        if (choice != 1 && choice != 2) {
            throw MatrixException("Некорректный выбор");
        }

        size_t n, m;
        std::cout << "Введите количество строк матрицы: ";
        std::cin >> n;
        std::cout << "Введите количество столбцов матрицы: ";
        std::cin >> m;

        Matrix<T> A(n, m);
        std::vector<T> b(n);

        if (choice == 1) {
            std::cout << "Введите элементы матрицы A (" << n << "x" << m << "):" << std::endl;
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    std::cin >> A[i][j];
                    op_counter.assignments++;
                }
            }

            std::cout << "Введите элементы вектора b (" << n << " элементов):" << std::endl;
            for (size_t i = 0; i < n; ++i) {
                std::cin >> b[i];
                op_counter.assignments++;
            }
        }
        else {
            T min_val, max_val;
            std::cout << "Введите минимальное значение: ";
            std::cin >> min_val;
            std::cout << "Введите максимальное значение: ";
            std::cin >> max_val;

            A = GenerateRandomMatrix<T>(n, m, min_val, max_val);
            b = GenerateRandomVector<T>(n, min_val, max_val);
        }

        std::cout << "\nИсходная матрица A:" << std::endl;
        A.print();
        std::cout << "\nВектор b:" << std::endl;
        for (auto x : b) std::cout << x << " ";
        std::cout << std::endl;

        Solution<T> solution = GaussJordan(A, b);

        if (!solution.is_consistent) {
            std::cout << "\nСистема несовместна (нет решений)\n";
        }
        else if (!solution.has_unique) {
            std::cout << "\nСистема имеет множество решений:\n";
            std::cout << "Частное решение:\n";
            for (size_t i = 0; i < solution.particular.size(); ++i) {
                std::cout << "x" << i + 1 << " = " << solution.particular[i];
                for (size_t j = 0; j < solution.basis.size(); ++j) {
                    if (solution.basis[j][i] != 0) {
                        std::cout << " + (" << -solution.basis[j][i] << ")*t" << j + 1;
                        op_counter.arithmetic_operations++;
                    }
                    op_counter.comparisons++;
                }
                std::cout << std::endl;
            }

            std::cout << "\nФундаментальная система решений (t1..tk - свободные параметры):\n";
            for (size_t i = 0; i < solution.basis.size(); ++i) {
                std::cout << "t" << i + 1 << ": [ ";
                for (T val : solution.basis[i]) {
                    std::cout << val << " ";
                }
                std::cout << "]\n";
            }
        }
        else {
            std::cout << "\nСистема имеет единственное решение:\n";
            for (size_t i = 0; i < solution.particular.size(); ++i) {
                std::cout << "x" << i + 1 << " = " << solution.particular[i] << std::endl;
            }
        }

        std::cout << "\nПроверка решения:\n";
        CheckSolution(A, b, solution);

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        std::cout << "\nСтатистика выполнения:\n";
        std::cout << "Время выполнения: " << elapsed.count() << " секунд\n";
        op_counter.print();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

int main() {
    setRussianLocale();

    int type_choice;
    std::cout << "Выберите тип данных:\n";
    std::cout << "1 - float (одинарная точность)\n";
    std::cout << "2 - double (двойная точность)\n";
    std::cout << "Ваш выбор: ";
    std::cin >> type_choice;

    if (type_choice == 1) {
        std::cout << "\nИспользуется тип float (одинарная точность)\n";
        RunGaussJordan<float>();
    }
    else if (type_choice == 2) {
        std::cout << "\nИспользуется тип double (двойная точность)\n";
        RunGaussJordan<double>();
    }
    else {
        std::cerr << "Некорректный выбор типа данных\n";
        return 1;
    }

    return 0;
}