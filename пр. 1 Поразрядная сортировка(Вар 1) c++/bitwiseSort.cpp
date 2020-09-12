#include <iostream>
#include <ctime>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;
void bitwiseSort(int* const, const size_t);
int main()
{
	srand(time(0));
	const int LENGTH = 20;
	int arr[LENGTH];
	for (size_t i = 0; i < LENGTH; i++)
		arr[i] = rand() % 2000;
	cout << "Start: ";
	for (size_t i = 0; i < LENGTH; i++)
		cout << arr[i] << " ";
	cout << "\nEnd: ";
	bitwiseSort(arr, LENGTH);
	for (size_t i = 0; i < LENGTH; i++)
		cout << arr[i] << " ";
	return 0;
}
void bitwiseSort(int* const arr, const size_t length)
{
	int maxDigits = to_string(*(max_element(arr, arr + length))).length();
	queue<int> q[10];
	for (size_t i = 0, c = 0; i < maxDigits; i++)
	{
		for (size_t j = 0; j < length; j++)
			q[(arr[j] % (int)pow(10, c + 1)) / (int)pow(10, c)].push(arr[j]);
		for (size_t j = 0, k = 0; j < 10; j++)
		{
			while (!q[j].empty())
			{
				arr[k] = q[j].front();
				q[j].pop();
				k++;
			}
		}
		c++;
	}
}