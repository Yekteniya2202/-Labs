// Lab 2.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

void task_1(double& timedelta1, double& timedelta2, int N);
void analyse_task_1(int iterations_num, int iterations_per_session);

void task_1(double& timedelta1, double& timedelta2, int N) {

	srand(time(NULL));
	

	//генерируем два случайных вещественных числа
	//volatile - запрещает компилятору оптимизировать данные переменные
	volatile double lfnum1 = (double)(rand() / RAND_MAX * 10000);
	volatile double lfnum2 = (double)(rand() / RAND_MAX * 10000);
	volatile double res;

	//засекаем время в однопотоке
	double start1 = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		res = lfnum1 * lfnum2;
	}
	double end1 = omp_get_wtime();
	timedelta1 = end1 - start1;

	//динамическая корректировка отключена
	omp_set_dynamic(0);
	//два потока - результаты неоднозначны, четыре потока - результаты однозначны, цикл выполняется быстрее
	omp_set_num_threads(2);

	//засекаем время в многопотоке
	//при двух потока результаты смазанные и неоднозначные, при четырёх результаты такие, как и ожидались
	double start2 = omp_get_wtime();
	#pragma omp parallel
	{
	#pragma omp for //schedule(static)  //firstprivate(lfnum1, lfnum2)
		for (int i = 0; i < N; i++) {
			res = lfnum1 * lfnum2;
		}
	}

	double end2 = omp_get_wtime();

	timedelta2 = end2 - start2;
}

void analyse_task_1(int iterations_num, int iterations_per_session) {
	cout << "Внешних итераций: " << iterations_num << endl;
	cout << "Внутренних итераций: " << iterations_per_session << endl;

	int expected_sessions = 0, unexpected_sessions = 0;
	//для сессии засекания времени делаем цикл - прогоняем его несколько раз (iterations_num), при этом засекая время
	//delta1 - в однопотоке, delta2 - в многопотоке
	for (int i = 0; i < iterations_num; i++) {
		double delta1, delta2;
		//iterations_per_session - число итераций в цикле при операциях с вещественными числами
		task_1(delta1, delta2, iterations_per_session);
		//если в однопотоке время выполнения больше, чем в многопотоке, то для нас это ожидаемый результат
		//увеличиваем счетчик ожидаемых результатов
		//иначе - увеличиваем счетчик неожидаемых результатов
		delta1 > delta2 ? expected_sessions++ : unexpected_sessions++;
	}
	cout << "Ожидаемых результатов: " << expected_sessions << endl;
	cout << "Неожидаемых результатов: " << unexpected_sessions << endl;
}
void task_2(double& timedelta) {
	double start = omp_get_wtime();
	#pragma omp parallel
	{
	}
	timedelta = omp_get_wtime() - start;
}

void analyse_task_2(int iterations_count) {
	double delta;
	for (int i = 0; i < iterations_count; i++) {
		task_2(delta);
		cout << "Время создания региона: " << fixed << delta << endl;
	}
}

void task_3() {
	#pragma omp parallel
	{
		#pragma omp critical
		cout << "Начало " << omp_get_thread_num() << endl;
	#pragma omp single
		{

		#pragma omp critical
			cout << "Одна нить " << omp_get_thread_num() << endl;
		}

		#pragma omp critical
		cout << "Окончание " << omp_get_thread_num() << endl;
	}
	cout << "\n\n================\n\n";
	#pragma omp parallel
	{
		#pragma omp critical
		cout << "Начало " << omp_get_thread_num() << endl;
		#pragma omp single nowait
		{

			#pragma omp critical
			cout << "Одна нить " << omp_get_thread_num() << endl;
		}
		#pragma omp critical
		cout << "Окончание " << omp_get_thread_num() << endl;
	}
}

void task_4() {

	omp_set_dynamic(0);
	omp_set_num_threads(3);
	auto f = []() {
		#pragma omp master
		{
			#pragma omp critical
			cout << "Главный поток " << omp_get_thread_num() << endl;
		}
	};
	#pragma omp parallel
	{
		#pragma omp critical
		cout << "Начало " << omp_get_thread_num() << endl;
		f();
		#pragma omp critical
		cout << "Середина " << omp_get_thread_num() << endl;
		f();
		#pragma omp critical
		cout << "Конец " << omp_get_thread_num() << endl;
	}
}

void task_5() {
	int n;
	n = 10;
	cout << "Последовательная область: n = " << n << endl;
	omp_set_num_threads(2);
	#pragma omp parallel private(n)
	{
		n = 5;
		#pragma omp critical
		cout << "Параллельная область: n = " << n << endl;
		n = omp_get_thread_num();
		#pragma omp critical
		cout << "Параллельная область: n = " << n << endl;
	}

	cout << "Последовательная область: n = " << n << endl;
}

void task_6() {
	int m[5] = { 0,0,0,0,0 };
	for (int i = 0; i < 5; i++) {
		cout << "m[" << i << "] = " << m[i] << endl;
	}

	omp_set_dynamic(0);
	omp_set_num_threads(5);
	#pragma omp parallel shared(m)
	{
		m[omp_get_thread_num()] = omp_get_thread_num();
	}
	cout << "\n\n================\n\n";
	for (int i = 0; i < 5; i++) {
		cout << "m[" << i << "] = " << m[i] << endl;
	}
}

void task_7() {
	srand(time(NULL));
	int counter = 0;
	omp_set_num_threads(rand() % 5 + 5);
	#pragma omp parallel reduction(+:counter)
	{
		#pragma omp critical
		cout << "Поток = " << omp_get_thread_num() << " Счетчик = " << ++counter << endl;
	}
	cout << "Число потоков = " << counter << endl;
}

void task_8() {

	srand(time(NULL));
	int counter = 0;
	omp_set_num_threads(4);
	#pragma omp parallel reduction(+:counter)
	{
		counter = omp_get_thread_num();
		#pragma omp critical
		cout << "Поток = " << omp_get_thread_num() << " Счетчик = " << counter << endl;
	}
	cout << "Сумма номеров потоков = " << counter << endl;
}
int main()
{
#ifdef _OPENMP
	cout << "asdf" << endl;
#endif
	setlocale(LC_ALL, "rus");
	analyse_task_1(500, 1000000);
	//analyse_task_2(50);
	//task_3();
	//task_4();
	//task_5();
	//task_6();
	//task_7();
	//task_8();
	system("pause");
}