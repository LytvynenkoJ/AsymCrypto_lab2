#include "Header.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	int len = 0;
	cout << "Введите желаемое количество битов в случайном простом числе:      ";
	cin >> len;
	int* number = new int[len];
	number = randomNum(len);
	cout << "p = ";
	for (int i = 0; i < len; i++)
	{
		cout << number[i];
	}
	cout << endl;
	int resMR = 0;
	resMR = MR(number, len);
	cout << endl << "------------------------------------------" << endl;
	return 0;
}