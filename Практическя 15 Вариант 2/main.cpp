#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
/*
* Возвращает минимальное колличество скалярных произведений
* и "двумерный" вектор заполненный значениями k, для 
* последующего вывода правильно расставленных скобок.
* Время алгоритма O(n^3)
*/
pair<uint32_t, vector<vector<uint32_t>>> multiplyOrder(vector<uint32_t> p)
{
	uint32_t n = p.size() - 1;
	vector<vector<uint32_t>> kVals(n, vector<uint32_t>(n, 0));
	vector<vector<uint32_t>> dp(n, vector<uint32_t>(n, 0));
	for (uint32_t l = 1; l < n; l++) 
	{
		for (uint32_t i = 0; i < n - l; i++) 
		{
			uint32_t j = i + l;
			dp[i][j] = UINT32_MAX;
			for (uint32_t k = i; k < j; k++) 
			{
				if (dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1] < dp[i][j])
				{
					dp[i][j] = dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
					kVals[i][j] = k;
				}
			}
		}
	}
	return make_pair(dp[0][n - 1], kVals);
}
/*
* Возвращает строку с правильно расставленными скобками для оптимального
* перемножения матриц.
*/
string getCorrectBracets(uint32_t i, uint32_t j, vector<vector<uint32_t>> kVals, bool zeroize = true)
{
	static string str;
	if (zeroize) str = "";
	if (i == j)
		str += "A" + to_string(i);
	else 
	{
		str += "(";
		getCorrectBracets(i, kVals[i][j], kVals, 0);
		str += "*";
		getCorrectBracets(kVals[i][j] + 1, j, kVals, 0);
		str += ")";
	}
	return str;
}
int main()
{
	setlocale(LC_ALL, "Russian");
	vector<uint32_t> sizes;
	cout << "Введите размеры матриц (конец ввода 0):\n";
	for (uint32_t i = 0;; i++)
	{
		uint32_t size;
		cout << "Размер[" << i << "]: ";
		cin >> size;
		if (size == 0)
			break;
		sizes.push_back(size);
	}
	pair<uint32_t, vector<vector<uint32_t>>> p = multiplyOrder(sizes);
	cout << "Минимальное колличество скалярных умножений: " << p.first << endl;
	cout << "Правильно расставленные скобки: " << getCorrectBracets(0, p.second.size()-1, p.second) << endl;
	cout << "Эмпирическая оценка сложности алгоритма: О(n^3)" << endl;
	system("pause");
	return 0;
}