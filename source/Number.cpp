#include "Number.h"
#include <iostream>
#include "kalkulator.h"
using namespace std;

int Number::pocet;

// Konstruktory

Number::Number()
{
	pocet++;
	this->sign = true;
	this->number = new char[N];
	this->digits = 0;
	for (int i = 0; i < N; i++)
	{
		this->number[i] = 0;
	}
}

Number::Number(int a)
{
	pocet++;
	this->number = new char[N];
	setNumber(a);
}

Number::Number(const string& a)
{
	pocet++;
	this->number = new char[N];
	int i = a.length();
	if (a[0] == '-')
	{
		this->sign = false;
	}
	else
	{
		this->sign = true;
	}
	this->digits = i;
	i--;
	int j = N - 1;
	while (i > -1 && j > -1)
	{
		if (a[i] < 58 && a[i] > 47) // cislice jsou v ASCII kodu 48 - 57, potrebuju hodnoty
		{
			this->number[j] = a[i] - 48;
			j--;
		}
		i--;
	}
	while (j > -1)
	{
		this->number[j] = 0;
		j--;
	}

	setDigits();
}

//kopirovaci konstruktor -- vytvari kopii existujici instance
Number::Number(const Number& a) : sign(a.sign), digits(a.digits)
{
	pocet++;
	this->number = new char[N];
	for (int i = 0; i < N; i++)
	{
		this->number[i] = a.number[i];
	}
}

//stehovaci konstruktor  -- vytvari kopii nepojmenovane instance
Number::Number(Number&& a) : number(a.number), sign(a.sign), digits(a.digits)
{
    //pocet++;
	a.number = nullptr;
}


//destruktor
Number::~Number()
{
	pocet--;
	delete[] number;
}


// vypis
void Number::Print()
{
	if (!this->sign)
		cout << "-";
	for (int i = N - this->digits; i < N; i++)
	{
		cout << +this->number[i];
	}
	cout << endl;
}

//setNumber
void Number::setNumber(int a)
{
	if (a < 0)
	{
		this->sign = false;
		a = -a;
	}
	else
	{
		this->sign = true;
	}

	int i = N - 1;
	while (a != 0)
	{
		this->number[i] = a % 10;
		a = a / 10;
		i--;
	}
	this->digits = (N - 1) - i;

	while (i > -1)
	{
		this->number[i] = 0;
		i--;
	}
}


char& Number::operator[](int a)
{
	if (a > N || a < 0)
	{
        throw NumberException(Kalkulator::tr("Out of bounds.")); // mimo meze
	}
	else
	{
		return number[a];
	}
}

//kopirovaci prirazovaci operator
Number& Number::operator=(const Number& a)
{
    this->sign = a.sign;
	this->digits = a.digits;
	for (int i = 0; i < N; i++)
	{
		this->number[i] = a.number[i];
	}
	return *this;
}

//stehovaci prirazovaci operator
Number& Number::operator=(Number&& a)
{
	delete[] number;
	this->number = a.number;
	this->digits = a.digits;
	this->sign = a.sign;
	a.number = nullptr;
	return *this;
}


//scitani
void Number::operator+=(Number& a)
{
	if (this->sign != a.sign) // maji-li jine znamenko
	{
		if (a.sign) // (-a) + b == - (a - b) 
		{
			this->sign = true; 
			operator-=(a);
			this->sign = !this->sign; // zde byla chyba: this->sign = false;

		}
		else // a + (-b) == a - b
		{
			a.sign = true;
			operator-=(a);
			a.sign = false;
		}
	}
	else  // maji-li stejne znamenko
	{
		int k;
		if (digits >= a.digits)
		{
			k = digits;
		}
		else
		{
			k = a.digits;
			this->digits = k;
		}

		bool pod = false;
		if (k == N)
		{
			pod = true;
		}

		//lehce upraveny algoritmus z ucebnice ZALGU 7.2.2 - Scitani celych k-mistnych cisel
		int p = 0, b;
		for (int i = N - 1; i > N - 1 - k; i--)
		{
			if (pod && i == -1 && p == 1) // preteceni
			{
                throw NumberException(Kalkulator::tr("An overflow occurs!"));
			}


			b = number[i] + a.number[i] + p;
			if (b < 10)
			{
				this->number[i] = b;
				p = 0;
			}
			else
			{
				this->number[i] = b - 10;
				p = 1;
				if (i == N - k)
				{
					k++;
					this->digits++;
				}
			}
		}
	}
}

void Number::operator-=(Number& a)
{
	if (this->sign != a.sign) //maji-li ruzna znamenka
	{
		if (a.sign) // -a - (+b) == - ( a + b)
		{
			this->sign = true;
			operator+=(a);
			this->sign = false;
		}
		else // a - (-b) == a + b
		{
			a.sign = true;
			operator+=(a);
			a.sign = false;
		}
	}
	else // maji-li stejne znamenko
	{
		// lehce upraveny algoritmus 7.2.3 pro odecitani celych cisel
		int k, p = 0, b;
		if (this->isBigger(a))
		{
			k = this->digits;

			for (int i = N - 1; i > N - 1 - k; i--)
			{
				b = this->number[i] - a.number[i] - p;
				if (b < 0)
				{
					this->number[i] = 10 + b;
					p = 1;
				}
				else
				{
					this->number[i] = b;
					p = 0;
				}
			}
		}
		else //  a-b = -(b-a)
		{
			k = a.digits;
			this->sign = !this->sign;

			for (int i = N - 1; i > N - 1 - k; i--)
			{
				b = a.number[i] - this->number[i] - p;
				if (b < 0)
				{
					this->number[i] = 10 + b;
					p = 1;
				}
				else
				{
					this->number[i] = b;
					p = 0;
				}
			}
		}
		setDigits();
	}
}

void Number::operator*=(Number& a)
{
	multiplication(a, false);
}

void Number::multiplication(Number& a, bool decimal)
{
	//algoritmus ZALG: 7.2.5
	// u x v = u_n u_(n-1) ... u_1 x v_m v_(m-1) ... v_1 = ( 10^n x u_n x v) + (10 ^(n-1) x u_(n-1) x v) + ... = (10^n x u_n x 10^m x u_m) + (10^n x u_n x 10^(m-1) x u_(m-1))+....
	// 725 x 456 = (700 x 456) + (20 x 456) + (5 x 456) = [(700 x 400) + (20 x 400) + (5 x 400)] + [(700 x 50) + (20 x 50) + (5 x 50)] + [(700 x 6) + (20 x 6) + (5 x 6)]

	if (decimal)
	{
		if (this->digits + a.digits - 2 * M > N - M)
		{
            throw NumberException(Kalkulator::tr("An overflow occurs!"));
		}

	}
	else
	{
		if (a.digits + this->digits - 1 > N)
		{
            throw NumberException(Kalkulator::tr("An overflow occurs!"));
		}
	}
	//potreba upravit pro decimal == true


	int j = N - 1;
	int i, p, t;
	int l = this->digits + a.digits;
	int* w = new int[l];
	for (int k = 0; k < l; k++)
	{
		w[k] = 0;
	}
	while (j > N - 1 - a.digits)
	{
		if (a.number[j] == 0)
		{
			w[j - N + a.digits] = 0;
			j--;
			continue;
		}
		i = N - 1;
		p = 0;
		while (i > N - 1 - this->digits)
		{
			t = this->number[i] * a.number[j] + w[i + j + l - (2 * N - 1)] + p;
			w[-(2 * N - 1) + i + j + l] = t % 10;
			p = t / 10;
			if ((-(2 * N - 1) + i + j + l) == 0 && p != 0) // jsme-li na nultem indexu a zbytek je nenulovy, dochazi k preteceni
			{
                throw NumberException(Kalkulator::tr("An overflow occurs!"));
			}
			i--;
		}
		w[j - N + a.digits] = p;
		j--;
	}
	j = 0;
    if (decimal)
	{
		for (int k = N - l + M; k < N; k++)
		{
			this->number[k] = w[j];
			j++;
		}
		for (int k = 0; k < N - l + M; k++)
		{
			this->number[k] = 0;
		}
	}
	else
	{
		for (int k = N - l; k < N; k++)
		{
			this->number[k] = w[j];
			j++;
		}
	}
	delete[] w;
	setDigits();
	if (a.sign == false)
	{
		this->sign = !this->sign;
	}
}

void Number::operator/=(Number& a)
{
	if (a.sign == false)
	{
		this->sign = !this->sign;
	}

	bool signum = this->sign;
	this->sign = true;

	if (a.digits == 0)//deleni nulou
	{
        throw NumberException(Kalkulator::tr("Dividing by zero!"));
	}
    else if (this->absEq(a)) // jsou-li stejne (lisi se max ve znamenku)
	{
		for (int i = 0; i < this->digits - 1; i++)
		{
			this->number[N - this->digits + i] = 0;
		}
		this->digits = 1;
		this->number[N - 1] = 1;
	}
    else if (a >= *this) // jedna se pouze o a > *this , rovnost je oresena v minule podmince
	{
		this->sign = true;
		for (int i = N - 1 - this->digits; i < N; i++)
		{
			this->number[i] = 0;
		}
		this->digits = 0;
		return;
    }

    int l = this->digits + 1 - a.digits;

	char* w = new char[l];
	for (int i = 0; i < l; i++)
	{
		w[i] = 0;
	}
	Number pom("0");
	for (int i = N - this->digits; i < N - this->digits + a.digits - 1;i++)
	{
		pom *= 10;
		pom.number[N-1] = this->number[i];
		pom.setDigits();
	}
	Number t("0");
	Number p1(0);
	Number p2(0);
	Number nula(0);
	for (int j = 0; j < l; j++)
	{
		pom *= 10;
		pom.number[N-1] = this->number[N - this->digits + a.digits + j - 1];
		pom.setDigits();
		t = pom;
		for (int i = 1; i <= 10; i++)
		{
			p1 = a * i;
			p2 = pom - p1;
			if (nula == p2)
			{
				w[j] = i;
				pom = p2;
				break;
			}
			else if (nula >= p2)
			{
				w[j] = i-1;
				pom = t;
				break;
			}
			t = p2;
		}
	}
	int k = N - 1;
	for (int i = l - 1; i >=0 ; i--)
	{
		this->number[k] = w[i];
		k--;
	}
	while (k >= 0)
	{
		this->number[k] = 0;
		k--;
	}
	setDigits();
    delete[] w;
	this->sign = signum;
	//division(a, false);
}

//stara jiz nepouzivana metoda
//nekde se vyskytuje chyba
void Number::division(Number& a, bool decimal)
{
	if (a.sign == false)
	{
		this->sign = !this->sign;
	}

	if (a.digits == 0)//deleni nulou
	{
        throw NumberException(Kalkulator::tr("Dividing by zero!"));
	}
	else if (a == *this) // jsou-li stejne (lisi se max ve znamenku)
	{
		for (int i = 0; i < this->digits - 1; i++)
		{
			this->number[N - this->digits + i] = 0;
		}
		this->digits = 1;
		this->number[N - 1] = 1;
	}
	else if (a >= *this) // jedna se pouze o a > *this , rovnost je oresena v minule podmince
	{
		this->sign = true;
		for (int i = N - 1 - this->digits; i < N; i++)
		{
			this->number[i] = 0;
		}
		this->digits = 0;
	}
	else if (a.digits == 1) // deleni jednocifernym cislem 
	{
		int d = a.number[N - 1];
		for (int i = N - this->digits; i < N; i++)
		{
			if (i != N - 1)
				this->number[i + 1] += (this->number[i] % d) * 10;
			this->number[i] = this->number[i] / d;
		}
		if (this->number[N - this->digits] == 0)
			this->digits--;
	}
	else
	{
		//algoritmus 7.2.6
		//1,2) inicializace
		bool mensi;
		int m, n;
		n = a.digits;
		m = this->digits - n;
		int* q = new int[m + 1];
		int* u = new int[m + n + 1];
		int* v = new int[n];
		int* v_ = new int[n + 1];
		// normalizace, zajistuje to aby prvni cifra delitele byla vetsi nez [10/k]
		int d = 10 / (a.number[N - n] + 1);
		for (int k = 1; k <= m + n; k++)//vynasobeni cisla u cislem d
		{
			u[k] = this->number[N - 1 - this->digits + k] * d;
		}
		for (int k = 0; k < n; k++) //vynasobeni cisla v cislem d
		{
			v[k] = a.number[N - n + k] * d;
		}
		u[0] = 0;

		// uprava tak aby v kazdem prvku byla jen cislice
		for (int k = m + n; k > 0; k--)
		{
			u[k - 1] += u[k] / 10;
			u[k] = u[k] % 10;
		}

		for (int k = n - 1; k > 0; k--)
		{
			v[k - 1] += v[k] / 10;
			v[k] = v[k] % 10;
		}


		int q_;
		for (int j = 0; j <= m; j++)
		{
			// 3) vypocet odhadu q_
			if (u[j] == v[0])
			{
				q_ = 9;
			}
			else
			{
                if (v[0])
                    q_ = (u[j] * 10 + u[j + 1]) / v[0];
                else
                    throw NumberException(Kalkulator::tr("Dividing by zero!")); // k tomuto deleni by vubec nemelo dojit, ale kvuli prekladaci radsi odchytavam, hlasil warning
			}

			while ((v[1] * q_) > ((u[j] * 10 + u[j + 1] - q_ * v[0]) * 10 + u[j + 2])) // test ze: v_2*q_ > (u_j * 10 + u_(j+1) - q_*v1)*10 + u_(j+2)
			{
				q_--;
			}

			{//nasobeni a odecteni
				v_[0] = 0;//pomocny vektor do ktereho ulozim cislo v x q_
				for (int k = 0; k < n; k++)
				{
					v_[k + 1] = v[k] * q_;

				}

				for (int k = n; k > 0; k--)
				{
					v_[k - 1] += v_[k] / 10;
					v_[k] = v_[k] % 10;
				}


				mensi = false;
				for (int l = 0; l < n + 1; l++)// test je-li v_ > u
				{
					if (u[l + j] > v_[l])
					{
						break;
					}
					else if (u[l + j] < v_[l])
					{
						mensi = true;
						break;
					}
				}
				int p = 0, b;
				for (int k = n; k >= 0; k--)
				{
					if (mensi && k == 0)//je-li v_ > u pak k u pricteme 10^n (vypujcka)
						p--;
					b = u[k + j] - v_[k] - p;
					if (b < 0)
					{
						u[k + j] = 10 + b;
						p = 1;
					}
					else
					{
						u[k + j] = b;
						p = 0;
					}
				}
			}

			//krok 5
			q[j] = q_;

			//krok 6
			if (mensi)//doslo-li k vypujcce
			{
				q[j]--;
				for (int k = 1; k <= n; k++)
				{
					u[k + j] += v[k];
				}
				for (int k = n; k > 0; k--)
				{
					u[k - 1 + j] += u[k + j] / 10;
					u[k + j] = u[k + j] % 10;
				}
			}
		}
		//8) zruseni normalizace v poli q je ulozen chteny podil.
		if (decimal)
		{
			
		}
		else
		{
			for (int i = 0; i < n - 1; i++)
			{
				this->number[N - this->digits + i] = 0;
			}
			for (int i = 0; i <= m; i++)
			{
				cout << q[i];
				this->number[N - 1 - m + i] = q[i];
			}
			cout << endl;
		}
		//mazani dynamickych poli
		delete[] u;
		delete[] v;
		delete[] q;
		delete[] v_;
		setDigits();
	}
}

void Number::operator/=(int a)
{
	Number s(a);
	operator/=(s);
}

void Number::operator*=(int a)
{
	if (a == -1)
	{
		this->sign = !this->sign;
		return;
	}
	/*Number s(a);
	operator*=(s);*/
	int t, p, o;
	if (a < 0)
	{
		this->sign = !this->sign;
		a = -a;
	}
	else if (a == 0)
	{
		this->sign = true;
		for (int i = N - this->digits; i < N; i++)
		{
			this->number[i] = 0;
		}
	}

	bool sus = false;
	o = a;
	for (int i = 1; i <= 10; i++)
	{
		if (o == 0)
		{
			o = i - 1;
			break;
		}
		o /= 10;
		if (i == 10)
			o = 10;
	}
	if (o + this->digits - 1 > N)
	{
        throw NumberException(Kalkulator::tr("An overflow occurs!"));
	}
	else if (o + this->digits - 1 == N)
	{
		sus = 1;
	}
	int* l = new int[o];

	for (int i = o - 1; i >= 0; i--)
	{
		l[i] = a % 10;
		a /= 10;
	}



	int* w = new int[this->digits + o];
	for (int i = 0; i < o + this->digits; i++)
		w[i] = 0;

	for (int k = o - 1; k >= 0; k--)
	{
		if (l[k] == 0)
		{
			w[k] = 0;
			continue;
		}
		p = 0;
		for (int i = N - 1; i > N - 1 - this->digits; i--)
		{
			t = this->number[i] * l[k] + w[i + k + this->digits - (N - 1)] + p;
			p = t / 10;
			w[i + k - (N - 1) + this->digits] = t % 10;
			if (sus && i == 0 && k == 0 && p != 0)
			{
                throw NumberException(Kalkulator::tr("An overflow occurs!"));
			}
		}
		w[k] = p;
	}
	int j = 0;
	for (int k = N - this->digits - o; k < N; k++)
	{
		this->number[k] = w[j];
		j++;
	}
	delete[] w;
	delete[] l;
	setDigits();
}

void Number::operator+=(int a)
{
	Number b(a);
	*this += b;
}

void Number::operator-=(int a)
{
	Number b(a);
	*this -= b;
}

//metoda, ktera podle prvni nalezene nenulove cislice urci pocet cislic cisla
void Number::setDigits()
{
	for (int i = 0; i < N; i++)
	{
		if (this->number[i] != 0)
		{
			this->digits = N - i;
			return;
		}
	}
	this->digits = 0;
}

//metoda na porovnani rovnosti cislic (neporovnava znamenka)
bool Number::absEq(Number & a)
{
    for (int i = 0; i < this->digits; i++)
    {
        if (this->number[N - this->digits + i] != a.number[N - this->digits + i])
        {
            return false;
        }
    }
    return true;
}

bool Number::operator==(Number& a)
{
	if (this->digits != a.digits)
	{
		return false;
	}
	else
	{
        return absEq(a);
	}
}

bool Number::operator!=(Number& a)
{
	return !(*this == a);
}

bool Number::operator>=(Number& a)
{
	if (this->sign == true && a.sign == false)
		return true;
	else if (this->sign == false && a.sign == true)
		return false;
	if (this->digits > a.digits)
	{
		return true;
	}
	else if (a.digits > this->digits)
	{
		return false;
	}
	else
	{
		for (int i = N - 1 - this->digits; i < N; i++)
		{
			if (this->number[i] != a.number[i])
			{
				if (this->number[i] > a.number[i])
					return true;
				else
					return false;
			}
		}
		return true;
	}
}

bool Number::isBigger(Number& a)
{
	if (this->digits > a.digits)
	{
		return true;
	}
	else if (a.digits > this->digits)
	{
		return false;
	}
	else
	{
		for (int i = N - 1 - this->digits; i < N; i++)
		{
			if (this->number[i] != a.number[i])
			{
				if (this->number[i] > a.number[i])
					return true;
				else
					return false;
			}
		}
		return true;
	}
}
//______________________________________________
void Number::operator++(int)
{
	if (this->sign == false)
	{
		this->sign = true;
		operator--(0);
		this->sign = false;
	}
	else
	{
		int i = N - 1;
		if (this->number[i] == 9)
		{
			while (this->number[i] == 9 && i >= 0)
			{
				this->number[i] = 0;
				i--;
			}
			if (i == -1)
			{
                throw NumberException(Kalkulator::tr("An overflow occurs!"));
			}
		}
		this->number[i] += 1;
		if (this->number[N - this->digits] == 0)
		{
			this->digits++;
		}
	}
}

void Number::operator--(int)
{
	if (this->sign == false)
	{
		this->sign = true;
		operator++(0);
		this->sign = false;
	}
	else
	{
		if (this->digits == 0)
		{
			this->number[N - 1] = 1;
			this->sign = false;
		}
		else
		{
			int i = N - 1;
			if (this->number[i] == 0)
			{
				while (this->number[i] == 0)
				{
					this->number[i] = 9;
				}
			}
			this->number[i] --;
			if (this->number[N - this->digits] == 0)
			{
				setDigits();
			}
		}
	}
}

void Number::operator=(int a)
{
	setNumber(a);
}

bool Number::operator<=(int a)
{
	if (this->digits > 10) // int ma max 10 cislic
	{
		if (this->sign)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (this->digits == 0)
	{
		if (a < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		long long j = 0;
		int i = 1;
		for (int k = 0; k < N - this->digits; k++)
		{
			j += this->number[N - 1 - k] * i;
			i *= 10;
		}
		if (j <= a)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Number::operator>=(int a)
{
	// a <= b ==  -a >= -b
	this->sign = !this->sign;
	if (operator<=(a))
	{
		this->sign = !this->sign;
		return true;
	}
	this->sign = !this->sign;
	return false;
}

bool Number::operator<(int a)
{
	if (operator==(a))
		return false;
	return operator<=(a);
}

bool Number::operator>(int a)
{
	if (operator==(a))
		return false;
	return operator>=(a);
}

bool Number::operator==(int a)
{
	if (this->digits > 10)
	{
		return false;
	}
	else if (this->digits == 0 && a == 0)
	{
		return true;
	}
	else if ((a < 0 && this->sign == true) || (a > 0 && this->sign == false))
	{
		return false;
	}

	for (int i = N - 1; i >= N - 10; i--)
	{
		if (this->number[i] != a % 10)
		{
			return false;
		}
		a /= 10;
	}
	return true;
}

bool Number::operator!=(int a)
{
	return !operator==(a);
}

//____________________________________________
//	Funkce

Number operator+(Number& a, Number& b)
{
	Number s(a);
	s += b;
	return s;
}

Number operator-(Number& a, Number& b)
{
	Number s(a);
	s -= b;
	return s;
}

Number operator*(Number& a, Number& b)
{
	Number s(a);
	s *= b;
	return s;
}

Number operator*(Number& a, int b)
{
	Number s(a);
	s *= b;
	return s;
}

Number operator/(Number& a, Number& b)
{
	Number s(a);
	s /= b;
	return s;
}

Number operator%(Number& a, Number& b)
{
	Number c = a / b;
	c *= b;
	c = a - c;
	return c;
}

/*std::ostream& operator<<(std::ostream& os, Number& a)
{
	if (a.sign == false)
	{
		os << "-";
	}
	for (int i = N - a.digits; i < N; i++)
	{
		os << +a.number[i];
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, Number&& a) // a je nepojmenovana instance
{
	if (a.sign == false)
	{
		os << "-";
	}
	for (int i = N - a.digits; i < N; i++)
	{
		os << +a.number[i];
	}
	return os;
}
*/

std::string num(Number& a)
{
	if (a.digits == 0)
		return "0";
	std::string number = "";
	char l;
	if (a.sign == false)
	{
		number = number + "-";
	}
	for (int i = N - a.digits; i < N; i++)
	{
		l = a[i] + '0';
		number = number + l;
	}

	return number;
}
