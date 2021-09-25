// Lab 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
using namespace std;
void print_array(string mas_name, int A[], int n) {
    cout << "====================" << endl;
    for (int i = 0; i < n; i++) {
        cout << mas_name << "[" << i << "] = " << A[i] << endl;
    }
    cout << "====================" << endl;
}
void init_arrays_task_1(int A[], int B[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }
}
void task_1(int n) {
    omp_set_num_threads(4);
    int* A = new int[n];
    int* B = new int[n];
    int* C = new int[n];
    for (int i = 0; i < n; i++) {
        C[0] = 0;
    }
    init_arrays_task_1(A, B, n);

    print_array("A", A, n);
    print_array("B", B, n);
    #pragma omp parallel 
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            C[i] = A[i] + B[i];
            #pragma omp critical 
            cout << "Поток " << omp_get_thread_num() << " вычситал элемент C[" << i << "] = " << C[i] << endl;
        }
    }
     
    print_array("C", C, n);
}

void task_2(int n) {
    omp_set_num_threads(4);
    int* A = new int[n];
    int* B = new int[n];
    init_arrays_task_1(A, B, n);
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            sum += A[i] * B[i];
        }
    print_array("A", A, n);
    print_array("B", B, n);
    cout << "Сумма поэлементного умножения = " << sum << endl;

}

void init_matrix(int** B, int n) {
    if (B == nullptr) return;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = rand() % 10;
        }
    }
}
void init_vector(int* A, int n) {
    if (A == nullptr) return;
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 10;
    }
}
void print_square_matrix(int** A, int n) {
    cout << "==============" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "==============" << endl;
}
void task_3(int n) {
    omp_set_num_threads(4);
    int** B = new int*[n];
    int* A = new int[n];
    int* res = new int[n];
    for (int i = 0; i < n; i++) {
        B[i] = new int[n];
    }
    init_matrix(B, n);
    init_vector(A, n);
    init_vector(res, n);
    
    print_square_matrix(B, n);
    print_array("A", A, n);
    #pragma omp parallel
    {
        int sum = 0;
        #pragma omp for
            for (int j = 0; j < n; j++) {

                for (int i = 0; i < n; i++) {
                    sum += A[i] * B[j][i];
                }
                res[j] = sum;
                #pragma omp critical
                cout << "Поток " << omp_get_thread_num() << " вычситал элемент C[" << j << "] = " << res[j] << endl;
                sum = 0;
            }
    }
    print_array("C", res, n);

}
void task_4() {
    omp_set_num_threads(4);
    #pragma omp parallel 
    {
        #pragma omp for ordered
        for (int i = 2; i <= 9; i++) {
            #pragma omp ordered
            for (int j = 0; j <= 10; j++) {
                #pragma omp critical
                cout << i << " * " << j << " = " << i * j << endl;
            }
        }
    }
}
int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "rus");
    omp_set_dynamic(0);
    //task_1(10);
    //task_2(4);
    //task_3(3);
    task_4();
    system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
