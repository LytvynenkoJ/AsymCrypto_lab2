#include "Header.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	int choise = 0;
	cout << "Вы хотите сгенерировать случайный ключ для пользователя А или использовать свой?" << endl;
	cout << "0 - генерировать" << endl << "1 - использовать свой" << endl;
	cin >> choise;
	while (choise!=0 && choise!=1)
	{
		cout << "Выберете, пожалуйста, один из предложенных вариантов:        " << endl;
		cin >> choise;
	}

	int len = 10;
	if (choise==0)
	{
		cout << "Введите желаемое количество байтов в случайных простых числах p, q:      ";
		cin >> len;
		GenerateKeyPair(len);
	}
	else
	{
		string n;
		cout << "Введите, пожалуйста ключ n" << endl;
		cin >> n;
		len = n.length() / 2 - 2;
		GenerateKeyPair(len);
		nA = mpz_class(n, 16);
		string e;
		cout << "Введите, пожалуйста ключ e" << endl;
		cin >> e;
		eA = mpz_class(e, 16);
	}
	cout << "nA = " << hex << nA.get_mpz_t() << endl << endl;
	cout << "eA = " << eA.get_mpz_t() << endl << endl;
	cout << "nB = " << nB.get_mpz_t() << endl << endl;
	cout << "eB = " << eB.get_mpz_t() << endl << endl;

	int* number = new int[2 * (len - 2)];
	cout << "========================================" << endl;
	cout << endl << "Генерируем сообщение для шифрования/расшифрования" << endl << endl;
	number = randomNum(2 * (len - 2));
	string num = arrToStr(number, 2 * (len - 2));
	mpz_class M = mpz_class(num, 16);
	cout << "M = " << M.get_mpz_t() << endl << endl;
	cout << "========================================" << endl;
	mpz_class CA; mpz_class CB;
	mpz_class MA; mpz_class MB;
	if (choise ==0 )
	{
		cout << "Получен шифротекст для пользователя А : " << endl;
		CA = Encrypt(M, eA, nA);
		cout << "CA = " << CA.get_mpz_t() << endl << endl;
		cout << "Расшифруем этот шифротекст с помощью секретного ключа А : " << endl;
		MA = Decrypt(CA, dA, nA);
		cout << "MA = " << MA.get_mpz_t() << endl << endl;
		if (mpz_cmp(M.get_mpz_t(), MA.get_mpz_t()) == 0)
		{
			cout << "Совпало" << endl << endl;
		}
		cout << "----------------------------------------" << endl;
		cout << "Получен шифротекст для пользователя В : " << endl;
		CB = Encrypt(M, eB, nB);
		cout << "CB = " << CB.get_mpz_t() << endl << endl;
		cout << "Расшифруем этот шифротекст с помощью секретного ключа В : " << endl;
		MB = Decrypt(CB, dB, nB);
		cout << "MB = " << MB.get_mpz_t() << endl << endl;
		if (mpz_cmp(M.get_mpz_t(), MB.get_mpz_t()) == 0)
		{
			cout << "Совпало" << endl << endl;
		}
	}
	else
	{
		cout << "Получен шифротекст для пользователя А : " << endl;
		CA = Encrypt(M, eA, nA);
		cout << "CA = " << CA.get_mpz_t() << endl << endl;
		cout << "Пожалуйста введите шифротекст полученный с сайта при использовании открытого ключа пользователя В" << endl;
		string c;
		cin >> c;
		CB = mpz_class(c, 16);
		cout << "Расшифруем этот шифротекст с помощью секретного ключа В : " << endl;
		MB = Decrypt(CB, dB, nB);
		cout << "MB = " << MB.get_mpz_t() << endl << endl;
	}
	
	cout << "========================================" << endl;
	cout << endl << "Генерируем сообщение для установки/проверки цифровой подписи" << endl << endl;
	number = randomNum(2 * (len - 2));
	num = arrToStr(number, 2 * (len - 2));
	M = mpz_class(num, 16);
	cout << "M = " << M.get_mpz_t() << endl << endl;
	cout << "========================================" << endl;
	mpz_class SA; mpz_class SB;
	if (choise==0)
	{
		cout << "Пользователь А поставил подпись : " << endl;
		SA = Sign(M, dA, nA);
		cout << "SA = " << SA.get_mpz_t() << endl << endl;
		cout << "Проверим подлинность подписи пользователя А : " << endl;
		MA = Verify(SA, eA, nA);
		cout << "MA = " << MA.get_mpz_t() << endl << endl;
		if (mpz_cmp(M.get_mpz_t(), MA.get_mpz_t()) == 0)
		{
			cout << "Верификация пройдена успешно" << endl << endl;
		}
		cout << "----------------------------------------" << endl;
		cout << "Пользователь В поставил подпись : " << endl;
		SB = Sign(M, dB, nB);
		cout << "SB = " << SB.get_mpz_t() << endl << endl;
		cout << "Проверим подлинность подписи пользователя B : " << endl;
		MB = Verify(SB, eB, nB);
		cout << "MB = " << MB.get_mpz_t() << endl << endl;
		if (mpz_cmp(M.get_mpz_t(), MB.get_mpz_t()) == 0)
		{
			cout << "Верификация пройдена успешно" << endl << endl;
		}
	}
	else
	{
		cout << "Введите подпись, которую вы хотите проверить" << endl;
		cout << "Сообщение :   ";
		string m;
		cin >> m;
		cout << "Подпись :   ";
		string s;
		cin >> s;
		mpz_class M2 = mpz_class(m, 16);
		SA = mpz_class(s, 16);
		cout << "Проверим подлинность подписи пользователя А : " << endl;
		MA = Verify(SA, eA, nA);
		cout << "MB = " << MA.get_mpz_t() << endl << endl;
		if (mpz_cmp(M2.get_mpz_t(), MA.get_mpz_t()) == 0)
		{
			cout << "Верификация пройдена успешно" << endl << endl;
		}
		cout << "----------------------------------------" << endl;
		cout << "Получим подпись пользователя В : " << endl;
		SB = Sign(M, dB, nB);
		cout << "SB = " << SB.get_mpz_t() << endl << endl;
		cout << "Проверьте подлинность подписи пользователя B с помощью сайта " << endl;
	}
	
	cout << "========================================" << endl;
	if (choise ==0)
	{
		number = randomNum(2 * (len - 2));
		num = arrToStr(number, 2 * (len - 2));
		M = mpz_class(num, 16);
		cout << "k = " << M.get_mpz_t() << endl << endl;
		cout << "========================================" << endl;
		SendKey(M, eB, dA, nB, nA);
		cout << "k1 = " << k1.get_mpz_t() << endl;
		cout << "S1 = " << S1.get_mpz_t() << endl << endl;
		cout << "----------------------------------------" << endl;
		ReceiveKey(dB, eA, nB, nA);
	}
	return 0;
}
