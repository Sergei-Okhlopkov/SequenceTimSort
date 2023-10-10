#include "windows.h";
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <algorithm>

using namespace std;

const int N = 1000000;
const string inputFN = "array.txt";
const string outputFN = "sortedArray.txt";

int* GenerateArr();
void WriteArrToFile(int arr[]);

int* ReadArrToFile();
int* TimSort(int arr[]);
void InsertionSort(int arr[], int left, int right);
int* MergeSort(int arr[], int left[], int leftSize, int right[], int rightSize, int* k);
int GetMinrun(int n);

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int* arr = ReadArrToFile(); //считать массив из файла
	//int* arr = GenerateArr();
	//WriteArrToFile(arr);

	clock_t start = clock();
	
	arr = TimSort(arr);

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%f", seconds);

	WriteArrToFile(arr);

	//освобождение памяти
	delete[] arr;

	return 0;
}

int* GenerateArr()
{
	srand(time(0));

	int* arr = new int[N];

	for (int i = 0; i < N; i++)
	{
		arr[i] = rand() % N;
	}

	return arr;
}

void WriteArrToFile(int arr[])
{
	ofstream out;
	//out.open(inputFN);
	out.open(outputFN);
	if (out.is_open())
	{
		for (int i = 0; i < N; i++)
		{
			out << arr[i] << " ";
		}
	}
	out.close();
}

int* ReadArrToFile()
{
	int* arr = new int[N];
	ifstream in;
	in.open(inputFN);
	if (in.is_open())
	{
		for (int i = 0; i < N; i++)
		{
			in >> arr[i];
		}
	}
	in.close();

	return arr;
}

int* TimSort(int arr[])
{
	// 1-ый этап. Нахождение длины Run'а
	int minRunSize = GetMinrun(N);
	int* arr2 = new int[N];

	// 2-ой этап сортировка вставками подмассивов
	for (int i = 0; i < N; i += minRunSize) {
		InsertionSort(arr, i, min((i + minRunSize - 1), (N - 1)));
	}

	for (int i = 0; i < N; i++)
	{
		arr2[i] = arr[i];
	}

	for (int size = minRunSize; size <= N; size = 2 * size)
	{
		int k = 0;

		for (int left = 0; left < N; left += 2 * size) {
			int mid = min((left + size - 1), (N - 1));
			int right = min((left + 2 * size - 1), (N - 1));

			int leftSize = (mid - left + 1);
			int rightSize = (right - mid) < 0 ? 0 : right - mid;

			// копируем run'ы в вспомагательные массивы
			int* leftArr = new int[leftSize];
			int* rightArr = new int[rightSize];

			for (int i = 0; i < leftSize; i++)
			{
				leftArr[i] = arr2[i + left];
			}

			for (int i = 0; i < rightSize; i++)
			{
				rightArr[i] = arr2[i + mid + 1];
			}

			if (mid < right) {
				arr2 = MergeSort(arr2, leftArr, leftSize, rightArr, rightSize, &k);
			}

			delete[] leftArr, rightArr;
		}
	}
	return arr2;
}

void InsertionSort(int arr[], int left, int right)
{
	for (int i = left + 1; i <= right; i++) 
	{
		int key = arr[i];
		int j = i - 1;

		while (j >= left && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}

		arr[j + 1] = key;
	}
}

int* MergeSort(int arr[], int left[], int leftSize, int right[], int rightSize, int* k) //TODO: тут меняется основной массив при проходе. Нужно сделать копии
{
	int* mas = new int[N];

	for (int i = 0; i < N; i++)
	{
		mas[i] = arr[i];
	}

	int i = 0; // индекс на массиве слева
	int j = 0; // индекс на массиве справа
	while (i < leftSize && j < rightSize) {
		if (left[i] <= right[j]) {
			mas[(*k)++] = left[i++];	// сначала в результирующий массив по индексу k записывается число из левого массива по индексу i,
									// затем передвижение индекса в левом и результирующем массиве на один
		}
		else {
			mas[(*k)++] = right[j++];	// сначала в результирующий массив по индексу k записывается число из правого массива по индексу j,
									// затем передвижение индекса в правом и результирующем массиве на один
		}
	}

	// при окончании одного из массивов в результирующий дописывается остаток второго массива
	while (i < leftSize) {
		mas[(*k)++] = left[i++];
	}

	while (j < rightSize) {
		mas[(*k)++] = right[j++];
	}

	return mas;
}

int GetMinrun(int n)
{
	int r = 0;           /* станет 1 если среди сдвинутых битов будет хотя бы 1 ненулевой */
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}