Skip to content
Search or jump to…
Pull requests
Issues
Marketplace
Explore

@Yekteniya2202
Vaschilo
/
APXUTEKTYPA
Public
1
10
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
APXUTEKTYPA / 5.ompSections / 5.ompSections / main.cpp
@Vaschilo
Vaschilo 5opka
Latest commit 847fb97 11 days ago
History
1 contributor
147 lines(129 sloc)  3.08 KB

#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>
#include <string>
#include <functional>

void vectorSum();
void startSort(int* arr, int n, string sort_name, function<void(int*, int)> func);

void Merge(int* arr, int from, int to) {
	int middle, start, final, j;
	int* helperArr = new int[to + 1];
	middle = (from + to) / 2;
	start = from;
	final = middle + 1;
	for (j = from; j <= to; j++) {
		if ((start <= middle) && ((final > to) || (arr[start] < arr[final]))) {
			helperArr[j] = arr[start]; start++;
		}
		else {
			helperArr[j] = arr[final];
			final++;
		}
	}
	for (j = from; j <= to; j++)
		arr[j] = helperArr[j];
	delete[] helperArr;
};
void SimpleMergeSort(int* arr, int from, int to) {
	{
		if (from < to) {
			SimpleMergeSort(arr, from, (from + to) / 2);
			SimpleMergeSort(arr, (from + to) / 2 + 1, to);
			Merge(arr, from, to);
		}
	}
};
void MergeSortTrue(int* arr, int from, int to) {
	if (from >= to) return;
#pragma omp parallel num_threads(10)
	{
#pragma omp sections
		{
#pragma omp section
			{
				MergeSortTrue(arr, from, (from + to) / 2);
			}
#pragma omp section
			{
				MergeSortTrue(arr, (from + to) / 2 + 1, to);
			}
		}
		Merge(arr, from, to);
	}
}


void sorts() {
	int n = 0;
	srand(time(NULL));
	while (n <= 0)
	{
		cout << "Ââåäèòå êîëè÷åñòâî ýëåìåíòîâ â ìàññèâå: ";
		cin >> n;
		cout << endl;
	}

	int* myArray = new int(n);

	for (int i = 0; i < n; i++) myArray[i] = rand() % 2001 - 1000; // Îò -1000 äî 1000
	for (int i = 0; i < n; i++) cout << myArray[i] << "\t";

	startSort(myArray, n, "Îáû÷íàÿ ñîðòèðîâêà ñëèÿíèåì(Ïîñëåäîâàòåëüíûé àëãîðèòì)",
		[](int* arr, int size) {SimpleMergeSort(arr, 0, size - 1); });
	startSort(myArray, n, "Ñîðòèðîâêà ñëèÿíèåì ñî âëîæåííûì ïàðàëëåëèçìîì",
		[](int* arr, int size) {omp_set_nested(1); MergeSortTrue(arr, 0, size - 1); });

	delete[] myArray;
}

int main() {
	setlocale(LC_ALL, "rus");

	auto f = []() {cout << "\n\n\n"; system("pause"); system("cls"); };

	//vectorSum(); f();
	sorts(); f();

	return 0;
}

void vectorSum() {
	int n = 0;
	while (n <= 0)
	{
		cout << "Ââåäèòå äëèíó âåêòîðà: ";
		cin >> n;
		cout << endl;
	}
	vector<int> a; a.resize(n);
	vector<int> b; b.resize(n);
	vector<int> c; c.resize(n);
	auto f = [n](vector<int>& a, char name) {
		for (int i = 0; i < n; i++)
		{
			cout << name << "[" << i << "] = ";
			cin >> a[i];
		}
	};
	f(a, 'a');
	f(b, 'b');
#pragma omp parallel num_threads(3)
	{
#pragma omp sections
		{
#pragma omp section
			{
				for (int i = 0; i < n / 2; i++) c[i] = a[i] + b[i];
			}
#pragma omp section
			{
				for (int i = n / 2; i < n; i++) c[i] = a[i] + b[i];
			}
		}
	}
	cout << "Result: \n";
	for (int i = 0; i < n; i++)
		cout << "c[" << i << "] = " << c[i] << endl;
}

void startSort(int* arr, int n, string sort_name, function<void(int*, int)> func)
{
	int* copy = new int(n);
	for (int i = 0; i < n; i++) copy[i] = arr[i];

	cout << "\n\n" << sort_name << endl;
	double from = omp_get_wtime();

	func(copy, n);

	double to = omp_get_wtime();
	cout << "Âðåìÿ âûïîëíåíèÿ - " << to - from << endl << "Ðåçóëüòàò:\n";
	for (int i = 0; i < n; i++) cout << copy[i] << "\t";

	delete[] copy;
}