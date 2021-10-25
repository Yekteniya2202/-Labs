#include <iostream>
#include <vector>
#include <omp.h>
#include <stack>
#include <functional>
using namespace std;
int count1 = 0;
int count2 = 0;
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
	count1++;
	if (end - start < 2)
		return;
	if (end - start == 2) {
		if (a[start] > a[start + 1])
			swap(a[start], a[start + 1]);
		return;
	}
#pragma omp parallel num_threads(2) if (fabs(end - start) > 30000)
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


//quicksort algorithm
void quickSort_nonmulti(vector<int>& a, int start, int end)
{
	int i = start;
	int j = end;
	int pivot = a[(i + j) / 2];    // pivot 

	while (i <= j) {
		while (a[i] < pivot) i++;
		while (a[j] > pivot) j--;
		if (i <= j) {
			swap(a[i], a[j]);
			i++;
			j--;
		}
	}
	if (j > start)
		quickSort_nonmulti(a, start, j);
	if (i < end)
		quickSort_nonmulti(a, i, end);
}

//quicksort algorithm
void quickSort_multi(vector<int>& a, int start, int end)
{
	int i = start;
	int j = end;
	int pivot = a[(i + j) / 2];    // pivot 

	while (i <= j) {
		while (a[i] < pivot) i++;
		while (a[j] > pivot) j--;
		if (i <= j) {
			swap(a[i], a[j]);
			i++;
			j--;
		}
	}

#pragma omp parallel num_threads(2) if (fabs(end - start) > 30000)
	{
#pragma omp sections
		{
#pragma omp section
			{

				if (j > start)
					quickSort_multi(a, start, j);
			}
#pragma omp section
			{
				if (i < end)
					quickSort_multi(a, i, end);
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
	omp_set_num_threads(2);
	vector<int> a(3000000);
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

	shuffle(a);
	b = a;
	c = a;

	start = omp_get_wtime();
	quickSort_nonmulti(a, 0, a.size() - 1);
	end = omp_get_wtime();
	cout << "Быстрая сортировка в последовательном режиме -> " << end - start << endl;

	omp_set_nested(false);
	start = omp_get_wtime();
	quickSort_multi(b, 0, b.size() - 1);
	end = omp_get_wtime();
	cout << "Быстрая сортировка в параллельном режиме без вложенности пар-зма -> " << end - start << endl;

	omp_set_nested(true);
	start = omp_get_wtime();
	quickSort_multi(c, 0, c.size() - 1);
	end = omp_get_wtime();
	cout << "Быстрая сортировка в параллельном режиме с вложенным пар-змом-> " << end - start << endl;
	system("pause");
}