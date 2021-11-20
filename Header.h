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

std::random_device rd;
mpz_class pA; mpz_class pB;
mpz_class qA; mpz_class qB;
mpz_class dA; mpz_class dB;
mpz_class eA; mpz_class eB;
mpz_class nA; mpz_class nB;
mpz_class phiA; mpz_class phiB;
mpz_class S1; mpz_class k1;


int* randomNum(int len)
{
	int* number = new int[len];
	for (int i = 0; i < len; i++)
	{
		number[i] = 0;
	}
	mt19937 mersenne{ static_cast<mt19937::result_type>(time(nullptr)) };
	mersenne.seed(rd());
	uniform_int_distribution<> rand_val{ 1, 100 };
	for (int i = 0; i < len; i++)
	{
		number[i] = rand_val(mersenne) % 16;
	}
	number[0] = 1 + rand_val(mersenne) % 15;
	return number;
}

int stringTo16(char letter)
{
	switch (letter)
	{
	case '0': return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;
	case 'A':return 10;
	case 'a':return 10;
	case 'B':return 11;
	case 'b':return 11;
	case 'C':return 12;
	case 'c':return 12;
	case 'D':return 13;
	case 'd':return 13;
	case 'E':return 14;
	case 'e':return 14;
	case 'F':return 15;
	case 'f':return 15;
	}
	return -1;
}
int* strToArr(string num, int len)
{
	int* arr = new int[len];
	for (int i = 0; i < len; i++)
	{
		arr[i] = 0;
	}
	for (int i = 0; i < num.length(); i++)
	{
		arr[i] = stringTo16(num[i]);
	}
	return arr;
}
string arrToStr(int* arr, int len)
{
	string init_number;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] < 10)
			init_number += (char)(arr[i] + 48);
		else
			init_number += (char)(arr[i] + 55);
	}
	return init_number;
}
string initialString(int size)
{
	mt19937 mersenne{ static_cast<mt19937::result_type>(time(nullptr)) };
	mersenne.seed(rd());
	uniform_int_distribution<> rand_v{ 1, 100 };
	uniform_int_distribution<> rand_val{ 'a', 'f' };
	uniform_int_distribution<> rand_val2{ '0', '9' };
	srand(time(0));
	int rand_size = 1 + rand_v(mersenne) % (size - 2);
	string number;
	for (int i = 0; i < rand_size; i++)
	{
		int temp = rand_v(mersenne) % 15;
		//cout << temp << " ";
		if (temp < 10)
			number += rand_val2(mersenne);
		else
			number += rand_val(mersenne);
	}
	return number;
}

int* p_1(int* num, int len, int var=2)
{
	int* diff = new int[len];
	for (int i = 0; i < len; i++)
	{
		diff[i] = 0;
	}
	int borrow = 0;
	//cout << "p-1 = ";
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
			diff[len - i - 1] = temp + var;
			borrow = 1;
		}
	}
	return diff;
}

int findS(int* diff, int len, int var=2)
{
	if (var != 2)
	{
		len = len / 4;
	}
	int s = 0;
	for (int i = 0; i < len; i++)
	{
		for (int k = 1; k < 4; k++)
		{
			if (diff[len - i - 1]%(1<<k) == 0)
				s++;
			else
			{ 
				i = len;
				k = 4;
			}	
		}	
	}
	//cout << "s = " << s << endl;
	return s;
}
mpz_class findD(int* diff, int s, int len)
{
	string differ = arrToStr(diff,len);
	mpz_class d = mpz_class(differ, 16);
	string pow2s;
	switch (s%4)
	{
	case 0: pow2s += '1'; break;
	case 1: pow2s += '2'; break;
	case 2: pow2s += '4'; break;
	case 3: pow2s += '8'; break;
	}
	for (int i = 0; i < s/4; i++)
	{
		pow2s += '0';
	}
	mpz_class power = mpz_class(pow2s, 16);
	mpz_divexact(d.get_mpz_t(),d.get_mpz_t(),power.get_mpz_t());
	return d;
}

int pseudo(mpz_class x_num, mpz_class p, mpz_class d, int s)
{
	mpz_class res;
	mpz_powm(res.get_mpz_t(), x_num.get_mpz_t(), d.get_mpz_t(), p.get_mpz_t());
	if (res.get_ui()==1 || res.get_ui()==p.get_ui()-1)
	{
		return 1;
	}
	else
	{
		mpz_class pow2 = mpz_class("2", 16);
		for (int i = 1; i < s; i++)
		{
			mpz_powm(res.get_mpz_t(), res.get_mpz_t(), pow2.get_mpz_t(), p.get_mpz_t());
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
	//cout << res.get_ui() << endl;
	return 0;
}

int MR(int* num, int len, int var=2)
{
	int result = 1;
	int* arr = new int[4];
	int* diff = new int[len];
	for (int i = 0; i < len; i++)
	{
		diff[i] = 0;
	}

	//считаем р-1
	//cout << endl;
	diff = p_1(num, len, var);
	
	//разделяем р-1=d*2^{s} и находим d, s
	int s = findS(diff, len);

	mpz_class d_num = findD(diff, s, len);
	//cout << "d = " << d_num.get_mpz_t() << endl;
	string init_number_p = arrToStr(num, len);
	mpz_class p_num = mpz_class(init_number_p, 16);
	//cout << "p = " <<  p_num.get_mpz_t() << endl;

	for (int i = 0; i < 50; i++)
	{
		string init_number_x = initialString(len);
		mpz_class x_num = mpz_class(init_number_x, 16);
		mpz_class gcd;
		mpz_gcd(gcd.get_mpz_t(), p_num.get_mpz_t(), x_num.get_mpz_t());
		if (gcd.get_ui() == 1)
		{
			result = pseudo(x_num, p_num, d_num, s);
			if (result == 0)
				return 0;
		}
		else
			return 0;
	}
	return 1;
}

mpz_class primeGen(int len)
{
	int* primeNum = new int[len];
	for (int i = 0; i < len; i++)
	{
		primeNum[i] = 0;
	}
	int resMR = 0;
	while (resMR==0)
	{
		primeNum = randomNum(len);
		resMR = MR(primeNum, len, 16);
	}
	string num = arrToStr(primeNum, len);
	mpz_class primeNumber = mpz_class(num, 16);
	return primeNumber;
}

void GenerateKeyPair(int len)
{
	pA = primeGen(len);
	qA = primeGen(len);
	while (mpz_cmp(pA.get_mpz_t(),qA.get_mpz_t())==0)
	{
		qA = primeGen(len);
	}
	pB = primeGen(len);
	while (mpz_cmp(pA.get_mpz_t(), pB.get_mpz_t()) == 0 || mpz_cmp(pB.get_mpz_t(), qA.get_mpz_t()) == 0)
	{
		pB = primeGen(len);
	}
	qB = primeGen(len);
	while (mpz_cmp(pA.get_mpz_t(), qB.get_mpz_t()) == 0 || mpz_cmp(qB.get_mpz_t(), qA.get_mpz_t()) == 0 || mpz_cmp(qB.get_mpz_t(), pB.get_mpz_t()) == 0)
	{
		pB = primeGen(len);
	}
	mpz_class temp;
	if (mpz_cmp(pB.get_mpz_t(), pA.get_mpz_t())<0)
	{
		temp = pA;
		pA = pB;
		pB = temp;
	}
	if (mpz_cmp(qB.get_mpz_t(), qA.get_mpz_t()) < 0)
	{
		temp = qA;
		qA = qB;
		qB = temp;
	}
	mpz_mul(nA.get_mpz_t(), pA.get_mpz_t(), qA.get_mpz_t());
	mpz_mul(nB.get_mpz_t(), pB.get_mpz_t(), qB.get_mpz_t());
	mpz_class one = mpz_class("1", 16);
	mpz_class pA_1; mpz_class qA_1;
	mpz_class pB_1; mpz_class qB_1;
	mpz_sub(pA_1.get_mpz_t(), pA.get_mpz_t(), one.get_mpz_t());
	mpz_sub(qA_1.get_mpz_t(), qA.get_mpz_t(), one.get_mpz_t());
	mpz_sub(pB_1.get_mpz_t(), pB.get_mpz_t(), one.get_mpz_t());
	mpz_sub(qB_1.get_mpz_t(), qB.get_mpz_t(), one.get_mpz_t());
	mpz_mul(phiA.get_mpz_t(), pA_1.get_mpz_t(), qA_1.get_mpz_t());
	mpz_mul(phiB.get_mpz_t(), pB_1.get_mpz_t(), qB_1.get_mpz_t());
	eA = mpz_class("10001", 16);
	eB = mpz_class("10001", 16);
	mpz_invert(dA.get_mpz_t(), eA.get_mpz_t(), phiA.get_mpz_t());
	mpz_invert(dB.get_mpz_t(), eB.get_mpz_t(), phiB.get_mpz_t());
	
}

mpz_class Encrypt(mpz_class M, mpz_class e, mpz_class n)
{
	mpz_class C;
	mpz_powm(C.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
	return C;
}

mpz_class Decrypt(mpz_class C, mpz_class d, mpz_class n)
{
	mpz_class M;
	mpz_powm(M.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
	return M;
}

mpz_class Sign(mpz_class M, mpz_class d, mpz_class n)
{
	mpz_class S;
	mpz_powm(S.get_mpz_t(), M.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
	return S;
}

mpz_class Verify(mpz_class S, mpz_class e, mpz_class n)
{
	mpz_class M;
	mpz_powm(M.get_mpz_t(), S.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
	return M;
}

void SendKey(mpz_class k, mpz_class e1, mpz_class d, mpz_class n1, mpz_class n)
{
	mpz_class S;
	mpz_powm(k1.get_mpz_t(), k.get_mpz_t(), e1.get_mpz_t(), n1.get_mpz_t());
	mpz_powm(S.get_mpz_t(), k.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
	mpz_powm(S1.get_mpz_t(), S.get_mpz_t(), e1.get_mpz_t(), n1.get_mpz_t());
}

void ReceiveKey(mpz_class d1, mpz_class e, mpz_class n1, mpz_class n)
{
	mpz_class S;
	mpz_class k;
	mpz_class k2;
	mpz_powm(k.get_mpz_t(), k1.get_mpz_t(), d1.get_mpz_t(), n1.get_mpz_t());
	mpz_powm(S.get_mpz_t(), S1.get_mpz_t(), d1.get_mpz_t(), n1.get_mpz_t());
	mpz_powm(k2.get_mpz_t(), S.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
	if (mpz_cmp(k.get_mpz_t(),k2.get_mpz_t())==0)
	{
		cout << "Верификация прошла успешно" << endl;
		cout << "k = " << k.get_mpz_t() << endl;
	}
}
