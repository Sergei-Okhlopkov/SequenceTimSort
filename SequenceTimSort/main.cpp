#include "windows.h";
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

const int N = 1000000;
const int GALLOPCOUNT = 7; // ������� ��� ������ ������. ����� �������� ���������� �� ������ ������� ���������� ������
const string inputFN = "array.txt";
const string outputFN = "sortedArray.txt";

int* GenerateArr();
void WriteArrToFile(int* arr);

int* ReadArrToFile();
void TimSort(int* arr);
void InsertionSort(int* arr, int left, int right);
void MergeSort(int* arr, int* left, int* right, int* k);
int GetMinrun(int n);
void Gallop(int* arr, int* i, int j, int* k, int* one, int* two);
int GetArrSize(int* arr);

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int* arr = ReadArrToFile(); //������� ������ �� �����
		
	clock_t start = clock();
	
	TimSort(arr);

	clock_t end = clock();

	double seconds = (double)(end - start) / CLOCKS_PER_SEC;

	printf("%f", seconds);

	WriteArrToFile(arr);

	//������������ ������
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

void WriteArrToFile(int* arr)
{
	ofstream out;
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

void TimSort(int* arr)
{
	// 1-�� ����. ���������� ����� Run'�
	int minRunSize = GetMinrun(N);
	
	// 2-�� ���� ���������� ��������� �����������
	for (int i = 0; i < N; i += minRunSize) {
		InsertionSort(arr, i, min((i + minRunSize - 1), (N - 1)));
	}

	for (int size = minRunSize; size <= N; size = 2 * size)
	{
		int k = 0;

		for (int left = 0; left < N; left += 2 * size) {
			int mid = min((left + size - 1), (N - 1));
			int right = min((left + 2 * size - 1), (N - 1));

			int leftSize = (mid - left + 1);
			int rightSize = (right - mid) < 0 ? 0 : right - mid;

			// �������� run'� � ��������������� �������
			int* leftArr = new int[leftSize];
			int* rightArr = new int[rightSize];

			for (int i = 0; i < leftSize; i++)
			{
				leftArr[i] = arr[i + left];
			}

			for (int i = 0; i < rightSize; i++)
			{
				rightArr[i] = arr[i + mid + 1];
			}

			if (mid < right) {
				MergeSort(arr, leftArr, rightArr, &k);
			}

			// ������������ ������
			delete[] leftArr;
			delete[] rightArr;
		}
	}
}

void InsertionSort(int* arr, int left, int right)
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

void MergeSort(int* arr, int* left, int* right, int* k) 
{
	int gallopLeft = 0;
	int gallopRight = 0;

	int leftSize = GetArrSize(left);
	int rightSize = GetArrSize(right);

	int i = 0; // ������ �� ������� �����
	int j = 0; // ������ �� ������� ������
	while (i < leftSize && j < rightSize) 
	{
		if (left[i] <= right[j]) 
		{
			arr[(*k)++] = left[i++];	// ������� � �������������� ������ �� ������� k ������������ ����� �� ������ ������� �� ������� i,
										// ����� ������������ ������� � ����� � �������������� ������� �� ����
			gallopRight = 0;
			gallopLeft++;
		}
		else 
		{
			arr[(*k)++] = right[j++];	// ������� � �������������� 1������ �� ������� k ������������ ����� �� ������� ������� �� ������� j,
										// ����� ������������ ������� � ������ � �������������� ������� �� ����
			gallopLeft = 0;
			gallopRight++;
		}

		//����� ������ �����������
		if (gallopLeft >= GALLOPCOUNT) 
		{
			Gallop(arr, &i, j, k, left, right);
			gallopLeft = 0;
		}

		//������ ������ �����������
		if (gallopRight >= GALLOPCOUNT) {

			Gallop(arr, &j, i, k, right, left);
			gallopRight = 0;
		}
	}

	// ��� ��������� ������ �� �������� � �������������� ������������ ������� ������� �������
	while (i < leftSize) {
		arr[(*k)++] = left[i++];
	}

	while (j < rightSize) {
		arr[(*k)++] = right[j++];
	}
}

int GetMinrun(int n)
{
	int r = 0;           /* ������ 1 ���� ����� ��������� ����� ����� ���� �� 1 ��������� */
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

void Gallop(int* arr, int *i, int j, int* k, int* one, int* two)
{
	int gallopDegree = 0;
	int oneSize = GetArrSize(one);
	int twoSize = GetArrSize(two);

	while ((*i) + pow(2, gallopDegree) < oneSize && // ����� �� ������� �� ������� �������
		one[(*i) + (int)pow(2, gallopDegree)] <= two[j]) // ��������� �������� � ������� one (� ����� + �����) � ��������� � ������� two
	{
		gallopDegree++;
	}

	int h = 1; //���������� ��� ������ (����� �� ����������)

	if (gallopDegree > 0) {
		h = pow(2, gallopDegree) - pow(2, gallopDegree - 1); 
		gallopDegree--;
		int iEnd = (*i) + (int)pow(2, gallopDegree);

		while (true) {
			if (iEnd < oneSize && one[iEnd] <= two[j]) {
				if (h != 1) {
					h /= 2;
				}
				iEnd += h;
			}
			else {
				if (h != 1) {
					h /= 2;
				}
				
				iEnd -= h;
			}

			if (one[iEnd] <= two[j] &&
				((iEnd < oneSize - 1 && one[iEnd + 1] > two[j]) || (iEnd == oneSize - 1 && one[iEnd] <= two[j]))) { // ���� ������ ������ �������� ������� � ������� one || ���� ��������� ����� ������,
				//��� two[j], � ���������� ������ ���� ����� ��������, �� �� ����� �������� ������ ��� ����������� 
				break;
			}
		}

		for ((*i); (*i) < iEnd + 1; (*i)++)
		{
			arr[(*k)++] = one[*i];
		}
	}	
}

int GetArrSize(int* arr)
{
	return _msize(arr) / sizeof(int);
}
