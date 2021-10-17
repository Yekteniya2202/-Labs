// Lab 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <omp.h>
#include <stack>
#include <functional>
using namespace std;

vector<double> vector_sum_multi(const vector<int>& a,
	const vector<int>& b,
	bool multithreading) {

	if (a.size() != b.size()) {
		throw exception("Размеры массива не совпадают");
	}

	const int len = a.size();
	vector<double> c(len);
	if (multithreading) {
		#pragma omp parallel num_threads(2)
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					for (int i = 0; i < len / 2; i++) {
						c[i] = a[i] + b[i];
					}
				}
				#pragma omp section
				{
					for (int i = len / 2; i < len; i++) {
						c[i] = a[i] + b[i];
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < len; i++) {
			c[i] = a[i] + b[i];
		}
	}

	return c;
}


void merge_sort_nonmulti(vector<int>& a, int start, int end) {
	if (end - start < 2)
		return;
	if (end - start == 2) {
		if (a[start] > a[start + 1])
			swap(a[start], a[start + 1]);
		return;
	}
	merge_sort_nonmulti(a, start, start + (end - start) / 2);
	merge_sort_nonmulti(a, start + start + (end - start) / 2, end);
	vector<int> b;
	//начало
	int b1 = start;
	//середина
	int e1 = start + (end - start) / 2;
	int b2 = e1;
	while (b.size() < end - start)
	{
		if (b1 >= e1 || (b2 < end && a[b2] <= a[b1])) {
			b.push_back(a[b2]);
			b2++;
		}
		else {
			b.push_back(a[b1]);
			b1++;
		}
	}
	for (int i = start; i < end; i++) {
		a[i] = b[i - start];
	}

}

void merge_sort_multi(vector<int>& a, int start, int end)
{
	if (end - start < 2)
		return;
	if (end - start == 2) {
		if (a[start] > a[start + 1])
			swap(a[start], a[start + 1]);
		return;
	}
	#pragma omp parallel 
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				merge_sort_multi(a, start, start + (end - start) / 2);
			}
			#pragma omp section
			{
				merge_sort_multi(a, start + start + (end - start) / 2, end);
			}
		}


		#pragma omp sections
		{
			#pragma omp section
			{
				vector<int> b;
				//начало
				int b1 = start;
				//середина
				int e1 = start + (end - start) / 2;
				int b2 = e1;
				while (b.size() < end - start)
				{
					if (b1 >= e1 || (b2 < end && a[b2] <= a[b1])) {
						b.push_back(a[b2]);
						b2++;
					}
					else {
						b.push_back(a[b1]);
						b1++;
					}
				}
				for (int i = start; i < end; i++) {
					a[i] = b[i - start];
				}
			}
		}
	}
	
} 

int partition(vector<int>& a, int start, int end)
{
	int pivot = a[end];    // pivot 
	int i = (start - 1);

	for (int j = start; j <= end - 1; j++)
	{
		//if current element is smaller than pivot, increment the low element
		//swap elements at i and j
		if (a[j] <= pivot)
		{
			i++;    // increment index of smaller element 
			swap(a[i], a[j]);
		}
	}
	swap(a[i + 1], a[end]);
	return (i + 1);
}


//quicksort algorithm
void quickSort_nonmulti(vector<int>& a, int start, int end)
{
	if (start < end)
	{
		//partition the array 
		int pivot = partition(a, start, end);

		//sort the sub arrays independently 
		quickSort_nonmulti(a, start, pivot - 1);
		quickSort_nonmulti(a, pivot + 1, end);
	}
}

//quicksort algorithm
void quickSort_multi(vector<int>& a, int start, int end)
{
	if (start < end)
	{
		int pivot = partition(a, start, end);
		#pragma omp parallel num_threads(2)
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					//sort the sub arrays independently 
					quickSort_nonmulti(a, start, pivot - 1);
				}
				#pragma omp section
				{
					quickSort_nonmulti(a, pivot + 1, end);
				}
			}
		}
	}
}

void shuffle(vector<int>& a) {
	for (auto& elem : a) {
		elem = rand() % 1000;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	omp_set_dynamic(0);
	omp_set_num_threads(8);
	vector<int> a(1000000);
	vector<int> b;
	vector<int> c;
	double start, end;
	shuffle(a);
	b = a;
	c = a;

	start = omp_get_wtime();
	vector_sum_multi(a, b, false);
	end = omp_get_wtime();
	cout << "Сумма векторов в последовательном режиме -> " << end - start << endl;

	start = omp_get_wtime();
	vector_sum_multi(a, b, true);
	end = omp_get_wtime();
	cout << "Сумма векторов в параллельном режиме -> " << end - start << endl;


	start = omp_get_wtime();
	merge_sort_nonmulti(a, 0, a.size() - 1);
	end = omp_get_wtime();
	cout << "Cортировка слиянием в последовательном режиме -> " << end - start << endl;

	omp_set_nested(false);
	start = omp_get_wtime();
	merge_sort_multi(b, 0, b.size() - 1);
	end = omp_get_wtime();
	cout << "Cортировка слиянием в параллельном режиме без вложенности пар-зма -> " << end - start << endl;

	omp_set_nested(true);
	start = omp_get_wtime();
	merge_sort_multi(c, 0, c.size() - 1);
	end = omp_get_wtime();
	cout << "Cортировка слиянием в параллельном режиме с вложенным пар-змом-> " << end - start << endl;
	
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
