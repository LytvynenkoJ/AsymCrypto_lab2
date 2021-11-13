#pragma once
#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <fstream>
#include "windows.h"
#include <iomanip>
#include "time.h"
#include <string>
#include "locale.h"
#include <random>
using namespace std;



int* randomNum(int len)
{
	int* number = new int[len];
	for (int i = 0; i < len; i++)
	{
		number[i] = 0;
	}
	mt19937 mersenne{ static_cast<mt19937::result_type>(time(nullptr)) };
	uniform_int_distribution<> rand_val{ 1, 100 };
	for (int i = 0; i < len; i++)
	{
		number[i] = rand_val(mersenne) % 2;
	}
	number[0] = 1;
	return number;
}

int binToHex(int* arr)
{
	int num = 0;
	for (int i = 0; i < 4; i++)
	{
		num += arr[3 - i] * (1 << i);
	}
	return num;
}

int* p_1(int* num, int len)
{
	int* diff = new int[len];
	int borrow = 0;
	cout << "p-1 = ";
	for (int i = 0; i < len; i++)
	{
		int temp = 0;
		if (i == 0)
			temp = num[len - i - 1] - 1 - borrow;
		else
			temp = num[len - i - 1] - borrow;
		if (temp >= 0)
		{
			diff[len - i - 1] = temp;
			borrow = 0;
			for (int j = 0; j < len - i - 1; j++)
			{
				diff[j] = num[j];
			}
			i = len;
		}
		else
		{
			diff[len - i - 1] = temp + 2;
			borrow = 1;
		}
	}
	for (int i = 0; i < len; i++)
	{
		cout << diff[i];
	}
	cout << endl;
	return diff;
}

int findS(int* diff, int len)
{
	int s = 0;
	for (int i = 0; i < len; i++)
	{
		if (diff[len - i - 1] == 0)
			s++;
		else
			i = len;
	}
	cout << "s = " << s << endl;
	return s;
}

int* findD(int* diff, int s, int len)
{
	int leng = len / 4;
	int* d = new int[leng];
	int* arr = new int[4];
	for (int i = 0; i < len - s; i++)
	{
		diff[len - i - 1] = diff[len - s - i - 1];
	}
	for (int i = 0; i < s; i++)
	{
		diff[i] = 0;
	}
	int t = 0;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			arr[j] = 0;
		}
		for (int j = 0; j < 4; j++)
		{
			if (i < len)
				arr[j] = diff[i];
			else
				arr[j] = 0;
			i++;
		}
		d[t] = binToHex(arr);
		t++;
		i--;
	}
	for (int i = 0; i < len - s; i++)
	{
		diff[i] = diff[i + s];
	}
	for (int i = len - s; i < len; i++)
	{
		diff[i] = 0;
	}
	return d;
}

int div2(int* num, int len)
{
	if (num[len - 1] == 0)
		return 1;
	else
		return 0;
}

int div3(int* num, int len)
{
	int even = 0; //тут сумма чисел на четных местах
	int odd = 0; //-||- на нечетных
	for (int i = 0; i < len; i++)
	{
		if (i % 2 == 0)
			even += num[i];
		else
			odd += num[i];
	}
	if (abs(even - odd) % 3 == 0)
		return 1;
	else
		return 0;
}

int div5(int* num, int len)
{
	int i1 = 0; int i2 = 0;
	int i3 = 0; int i4 = 0;
	int even = 0;
	int odd = 0;
	int q = len % 4;
	len -= len % 4;
	for (int i = 0; i < len; i++)
	{
		i1 = num[i];
		i++;
		i2 = num[i];
		i++;
		i3 = num[i];
		i++;
		i4 = num[i];
		even += 2 * i1 + i2;
		odd += 2 * i3 + i4;
	}
	if (q>=2)
	{
		i1 = num[len];
		i2 = num[len+1];
		even += 2 * i1 + i2;
	}
	if (abs(even - odd) % 5 == 0)
		return 1;
	else
		return 0;
}

int div7(int* num, int len)
{
	int i1 = 0; int i2 = 0;
	int i3 = 0; int i4 = 0;
	int even = 0;
	int odd = 0;
	int q = len % 4;
	len -= len % 4;
	for (int i = 0; i < len; i++)
	{
		i1 = num[i];
		i++;
		i2 = num[i];
		i++;
		i3 = num[i];
		i++;
		i4 = num[i];
		even += 2 * i1 + i2;
		odd += 2 * i3 + i4;
	}
	if (q >= 2)
	{
		i1 = num[len];
		i2 = num[len + 1];
		even += 2 * i1 + i2;
	}
	if (abs(even - odd) % 7 == 0)
		return 1;
	else
		return 0;
}

int pseudo(string x_num, mpz_class p, mpz_class d, int s)
{
	int result = 0;
	mpz_class res = mpz_class("0",16);
	mpz_class x = mpz_class(x_num, 16);
	mpz_powm(res.get_mpz_t(), d.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());
	if (res.get_ui()==1 || res.get_ui()==p.get_ui()-1)
	{
		return 1;
	}
	else
	{
		mpz_class pow2 = mpz_class("2", 16);
		for (int i = 1; i < s; i++)
		{
			mpz_powm(res.get_mpz_t(), pow2.get_mpz_t(), res.get_mpz_t(), p.get_mpz_t());
			if (res.get_ui()==1)
			{
				return 0;
			}
			if (res.get_ui() == p.get_ui()-1)
			{
				return 1;
			}
		}
	}
	cout << res.get_ui() << endl;
	return 0;
}

int MR(int* num, int len)
{
	int result = 1;

	int leng = len / 4;
	int* arr = new int[4];
	int* p = new int[leng];
	int* diff = new int[len];
	for (int i = 0; i < len; i++)
	{
		diff[i] = 0;
	}

	//переводим его в 16-ричную систему
	for (int i = 0; i < leng; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			arr[j] = 0;
		}
		for (int j = 0; j < 4; j++)
		{
			arr[j] = num[4 * i + j];
		}
		p[i] = binToHex(arr);
		//cout << hex << p[i];
	}

	if (div2(num, len) == 1)
		return 0;
	if (div3(num, len) == 1)
		return 0;
	if (div5(num, len) == 1)
		return 0;
	if (div7(num, len) == 1)
		return 0;

	//считаем р-1
	cout << endl;
	diff = p_1(num, len);
	
	//разделяем р-1=d*2^{s} и находим d, s
	int s = findS(diff, len);
	int* d = findD(diff, s, len);

	string init_number_d;
	for (int i = 0; i < leng; i++)
	{
		if (d[i] < 10)
			init_number_d += (char)(d[i] + 48);
		else
			init_number_d += (char)(d[i] + 55);
	}
	mpz_class d_num = mpz_class(init_number_d, 16);
	cout << "d = " << d_num.get_mpz_t() << endl;
	string init_number_p;
	for (int i = 0; i < leng; i++)
	{
		if (p[i] < 10)
			init_number_p += (char)(p[i] + 48);
		else
			init_number_p += (char)(p[i] + 55);
	}
	mpz_class p_num = mpz_class(init_number_p, 16);
	cout << "p = " <<  p_num.get_mpz_t() << endl;

	result = pseudo("2", p_num, d_num, s);
	if (result == 0)
	{
		return 0;
	}
	result = pseudo("3", p_num, d_num, s);
	if (result == 0)
	{
		return 0;
	}
	result = pseudo("5", p_num, d_num, s);
	if (result == 0)
	{
		return 0;
	}
	result = pseudo("2", p_num, d_num, s);
	if (result == 0)
	{
		return 0;
	}

	return 1;
}
