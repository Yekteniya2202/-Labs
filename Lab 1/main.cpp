#include <iostream>
#include <omp.h>

using namespace std;

void check_openmp_supprot() {
#ifdef _OPENMP
	cout << _OPENMP << endl;
#endif
}


void task_1() {
	cout << "Последовательная область 1" << endl;
#pragma omp parallel 
	{
		cout << "Параллельная область" << endl;
	}
	cout << "Последовательная область 2" << endl;
}

void task_2() {
#pragma omp parallel 
	{
		if (omp_get_thread_num() == 0)
			cout << "Число порождённых потоков: " << omp_get_num_threads() << endl;
		else
			cout << "Номер дочернего потока: " << omp_get_thread_num() << endl;
	}
}

void task_3() {
#pragma omp parallel
	{
		cout << "Параллельный регион 1" << endl;
	}

#pragma omp parallel num_threads(3)
	{
		cout << "Параллельный регион 2" << endl;
	}
	omp_set_num_threads(4);
#pragma omp parallel 
	{
		cout << "Параллельный регион 3" << endl;
	}
}

void task_4(int dynamic_mod) {
	omp_set_dynamic(dynamic_mod);
#pragma omp parallel 
	{
		if (omp_get_thread_num() == 0)
			cout << "Число порождённых потоков: " << omp_get_num_threads() << endl;
	}

}
void task_5(string parallel_mod) {

	if (parallel_mod == "parallel") {
#pragma omp parallel 
		{
			if (omp_get_thread_num() == 0)
				cout << "Число порождённых потоков: " << omp_get_num_threads() << endl;
		}

	}
	else if (parallel_mod == "serial")
	{
		if (omp_get_thread_num() == 0)
			cout << "Число порождённых потоков: " << omp_get_num_threads() << endl;

	}
}

void general_task(int th)
{
	cout << "Theards num " << th << endl;
	omp_set_num_threads(th);
	int N = 100000000;
	double start1 = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		int n = 0;
		n += i;
	}
	double delta1 = omp_get_wtime() - start1;
	cout << "Single thread time is " << delta1 << endl;
	double start2 = omp_get_wtime();
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

void helloworld_from_threads() {
#pragma omp parallel 
	{
		cout << "Чилос потоков после их создания " << omp_get_num_threads() << endl;
		cout << "Hello world from thread №" << omp_get_thread_num() << endl;
	}
}


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	check_openmp_supprot();
	cout << "Число потоков до их создания " << omp_get_num_threads() << endl;
	cout << "Число процессоров " << omp_get_num_procs() << endl;
	omp_set_num_threads(6);
	helloworld_from_threads();
	return 0;
}

