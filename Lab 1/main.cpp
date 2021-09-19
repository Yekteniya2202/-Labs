// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");
#ifdef _OPENMP
	cout << _OPENMP << endl;
#endif
	cout << "Proc numbers " << omp_get_num_procs() << endl;
	int th = 1024;
	omp_set_num_threads(th);
	int N = 100000000;
	double start1 = omp_get_wtime();
	cout << start1 << endl;
	for (int i = 0; i < N; i++) {
		int n = 0;
		n += i;
	}
	double delta1 = omp_get_wtime() - start1;
	cout << "Single thread time is "<< delta1 << endl;
	double start2 = omp_get_wtime();
	cout << start2 << endl;
	int nt = N / th;
#pragma omp parallel
	{
		for (int i = 0; i < nt; i++) {
			int n = 0;
			n += i;
		}
		//cout << "Hello world from thread №" << omp_get_thread_num() << endl;
	}
	double delta2 = omp_get_wtime() - start2;
	cout << "Multi thread time is " << delta2 << endl;
//#pragma omp 

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
