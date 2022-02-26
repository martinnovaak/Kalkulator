#include "DecNumber.h"
#include <iostream>
#include "kalkulator.h"



DecNumber::DecNumber()
{
	a = new Number();
	decDigits = 0;
}


DecNumber::DecNumber(const DecNumber& num) 
{
	this->a = new Number(*num.a);
	this->decDigits = num.decDigits;
}

DecNumber::DecNumber(DecNumber&& num) : a(num.a), decDigits(num.decDigits)
{
	num.a = nullptr;
}

DecNumber::DecNumber(std::string str) :decDigits(0)
{
    int length = (int)str.length();
    if(length != 0 && str[0] == '-')
    {
        if (length > N + 2)
        {
           throw NumberException(Kalkulator::tr("Number is too large."));
        }
        else if(length == N + 2) // string muze byt delky N+2 jenom tehdy
        //obsahuje-li desetinnou tecku a zaporne znamenko
        {
            if(str[0] != '-')
            {
                throw NumberException(Kalkulator::tr("Number is too large."));
            }
        }
    }

    int pom = -1;

    // vyhledani desetinne tecky
    int k = length - 1;
	for (; k >= 0; k--)
	{
		if (str[k] == '.' || str[k] == ',')
		{
            if (pom != -1) // obsahuje-li vice tecek resp. carek
                throw NumberException(Kalkulator::tr("Too many decimal points."));
			else
                pom = k;
		}
    }
    //v pom je ulozen index desetinne tecky resp. carky
    k = pom;
	if (k != -1)
	{
		this->decDigits = length - k - 1;
	}
	else
    {
		k = length;
        this->decDigits = 0;
        if (length == N + 1 || length == N+2)
            throw NumberException(Kalkulator::tr("Number is too large."));
    }
    if (this->decDigits) // jedna-li se o "double"
	{
		length--;
		if (k > N - M)
			throw("Number is too large in integer part.");
        else if(length - k > M)
			throw("Number is too large in decimal part.");

        str.erase(k, 1); // odstraneni tecky ze stringu
    }
    //doplneni nul do stringu, bude se volat konstruktor tridy Number
    //cislo musi byt delky k + M ( k - pocet celych cislic + M - pocet desetinnych cislic)
    while (k + M > length)
    {
        str = str + '0';
        length++;
    }
    this->a = new Number(str);
	setDigits();
}


//destruktor zavolá destruktor Numberu
DecNumber::~DecNumber()
{
	delete this->a;
}

DecNumber & DecNumber::operator=(const DecNumber& a)
{
	this->a->operator=(*a.a);
	this->decDigits = a.decDigits;
	return *this;
}

DecNumber & DecNumber::operator=(DecNumber &&a)
{
    delete this->a;
    this->a = a.a;
    this->decDigits = a.decDigits;
    a.a = nullptr;


	return *this;
}

//na scitani desetinnych cisel lze pouzit stejny algoritmus
//jako u scitani celych cisel, je potreba jen zkontrolovat zda
//nedoslo ke zmene poctu desetinnych cisel
void DecNumber::operator+=(DecNumber & number)
{
	(*this->a) += (*number.a);
	this->setDigits();
}

//obdobne jako u scitani
void DecNumber::operator-=(DecNumber & number)
{
	(*this->a) -= (*number.a);
	this->setDigits();
}


// cislo 0,04 x 0,002 == (4 x 2) / 10^(2+3)
void DecNumber::operator*=(DecNumber& number)
{
    // upravena funkce pro nasobeni u tridy Number
    // rozeznava jedna-li se o DecNumber a nebo o originalni tridu Number
    // u DecNumber je treba nasobek vydelit prave cislem
    // 10^(this->decDigits + number.decDigits)
    // tedy posunout vysledek o (this->decDigits + number.decDigits) mist do prava
    this->a->multiplication((*number.a), true);
	setDigits();
}


// je-li a<b pak by a/b v klasickem Number deleni vychazelo 0
// proto udelam a/b = (a x 10^M / b) / 10^M
// nelze proto ale delit cislo, ktere ma vice jak N - M cislic

// 1 / 3 by byla 0
//ale 100000 / 3 by bylo 33333
//po posunuti desetinnych mist 0.33333
void DecNumber::operator/=(DecNumber& number)
{
    if(this->a->absEq(*number.a))
    {
        *this = DecNumber("1.0");
        if (this->a->sign != number.a->sign)
            *this *= -1;
        return;
    }
    if (this->a->digits > N - M)
    {
        throw NumberException(Kalkulator::tr("Too large number for decimal division"));
    }
    int i;
	for (i = M; i < N; i++)
	{
		this->a->number[i - M] = this->a->number[i];
	}
	for (i = N - M; i < N; i++)
	{
		this->a->number[i] = 0;
	}
	this->a->digits += M;
	(*this->a) /= (*number.a);
	setDigits();
}

void DecNumber::operator+=(int a)
{
	std::string str = std::to_string(a);
	for (int i = 0; i < M; i++)
		a = a + '0';
	DecNumber s(str);
	*this += s;
}

void DecNumber::operator-=(int a)
{
	std::string str = std::to_string(a);
	for (int i = 0; i < M; i++)
		a = a + '0';
	DecNumber s(str);
	*this -= s;
}

void DecNumber::operator*=(int a)
{
	if (a == -1)
	{
		this->a->sign = !this->a->sign;
		return;
	}
	std::string str = std::to_string(a);
	/*for (int i = 0; i < M; i++)
		a = a + '0';*/
	DecNumber s(str);
	*this *= s;
	setDigits();
}

void DecNumber::operator/=(int a)
{
	std::string str = std::to_string(a);
	for (int i = 0; i < M; i++)
		a = a + '0';
	DecNumber s(str);
	*this /= s;
    setDigits();
}

void DecNumber::operator++(int)
{
	std::string a = "1.";
	for (int i = 0; i < M; i++)
		a = a + '0';
	DecNumber s(a);
	*this += s;
}

void DecNumber::operator--(int)
{
	std::string a = "-1";
	for (int i = 0; i < M; i++)
		a = a + '0';
	DecNumber s(a);
	*this += s;
}


bool DecNumber::operator>=(DecNumber& a)
{
	return this->a->operator>=(*a.a);
}


bool DecNumber::operator==(DecNumber& a)
{
	return this->a->operator==(*a.a);
}

bool DecNumber::operator==(int a)
{
    DecNumber b(std::to_string(a));
    return *this == b;
}

bool DecNumber::operator>=(int a)
{
    DecNumber b(std::to_string(a));
    return this->operator>=(b);
}

bool DecNumber::operator<=(int a)
{
    if(*this == a)
        return true;
    return !(*this >= a);
}

bool DecNumber::operator!=(int a)
{
    return !this->operator==(a);
}

bool DecNumber::operator!=(DecNumber& a)
{
	return this->a->operator!=(*a.a);
}

//funkce, ktera zjisti pocet desetinnych mist
//prochazi pole od konce a hleda index prvniho nenuloveho cisla
void DecNumber::setDigits()
{
	int i = N - 1;
	while (i >= N - M)
	{
		if (this->a->number[i] != 0)
		{
			this->decDigits = i + M + 1 - N;
			return;
		}
		i--;
	}
	this->decDigits = 0;
}

void DecNumber::print()
{
	if (!a->sign)
		std::cout << "-";
	if (a->digits - M > 0)
	{
		for (int i = N - this->a->getDigits(); i < N - M; i++)
		{
			std::cout << +a->number[i];
		}
	}
	else
	{
		std::cout << "0";
	}
	if (this->decDigits)
	{
		std::cout << ".";
		for (int i = N-M; i < N-M + this->decDigits; i++)
		{
			std::cout << +a->number[i];
		}
	}
}

DecNumber operator+(DecNumber& a, DecNumber& b)
{
    DecNumber s(a);
    s += b;
    return s;
}

DecNumber operator-(DecNumber& a, DecNumber& b)
{
    DecNumber s(a);
    s -= b;
    return s;
}

DecNumber operator*(DecNumber& a, DecNumber& b)
{
    DecNumber s(a);
    s *= b;
    return s;
}

DecNumber operator/(DecNumber& a, DecNumber& b)
{
	DecNumber s(a);
	s /= b;
	return s;
}

//prevedeni celociselneho DecNumber na binarni cislo
std::string num2binary(DecNumber& a)
{
	std::string str = "";
	DecNumber nula("0.0");
	DecNumber dva("2.0");
	DecNumber pom(a);
	
	while (pom.a->getDigits() != M)
	{
		if (pom[N - M - 1] % 2)
		{
			str += '1';
		}
		else
			str += '0';
		pom /= dva;
	}
    return str; // binarni cislo je ulozeno ve stringu naopak - tzn 4 == 001 misto  100
}



// e^(a+b) = e^a * e^b
DecNumber exponential(DecNumber & a)
{
    DecNumber pom("1.0");
    if (a == 0)
    {
        return pom;
    }
    else if(a == 1)
    {
        return DecNumber(e);
    }
    DecNumber b = intPart(a);
	std::string str = num2binary(b);

	DecNumber num = a - b;
	b = num;
	DecNumber n("1.0");


	DecNumber suma("1.0");
	int i = 2;
    while (pom != 0)
	{
		pom = num / n;
		suma += pom;
		num *= b;
		n *= i;
		i++;
	}

	DecNumber ex = e;
	DecNumber jedna("1.0");
    if (a <= 0)
	{
		ex = jedna / ex;
	}
	
	// Vypocet mocniny podle skript Základy algoritmizace str. 237 kap. 8.2 - Vypocet mocniny
    for (int j = 0; j < (int)str.length(); j++)
	{
		if (str[j] == '1')
			suma *= ex;
		ex *= ex;
	}
	return suma;
}

DecNumber logarithm(DecNumber& a)
{
    if (a <= 0)
    {
        throw NumberException(Kalkulator::tr("Logarithm error"));
    }
	DecNumber pom(a);
	DecNumber l("0.666666");
	DecNumber r("1.333332");
	int i = 0;
    //lna = ln(b*2^i) = lnb + i*ln2 ; kde b e (l,r) -> kde taylorova rada konverguje pomerne rychle a ln2 je referencni hodnota
    while (!(pom >= l && r >= pom))
	{
        if(l >= pom)
        {
            pom *= 2;
            i--;
        }
        else
        {
            pom /= 2;
            i++;
        }
    }
	DecNumber n("1.0");
    DecNumber suma("0.0");
    if (pom != 1) //ln(1) == 1
    {
        pom -= n;
        DecNumber moc = pom;
        DecNumber p("0.0");
        int k = 1;
        do
        {
            p = pom / n;
            if (k % 2 == 0)
                p *= -1;
            suma += p;
            pom *= moc;

            n += 1;
            k++;
            } while (p != 0);
        }
	if (i == 0)
		return suma;
    DecNumber lg(ln_2);
	lg *= i;
	suma += lg;
	return suma;
}


// log_b (a) = ln(a) / ln(b)
DecNumber log(DecNumber& a, DecNumber& base)
{
    if (base == 1)
    {
        throw NumberException(Kalkulator::tr("Logarithm with a base of 1."));
    }
    DecNumber lna = logarithm(a);
	DecNumber lnb = logarithm(base);
	return lna / lnb;
}

//prevede DecNumber na celociselne DecNumber (vynuluje vsechny desetinne mista)
DecNumber intPart(DecNumber& a)
{
	DecNumber b(a);
	for (int i = N - M; i < N; i++)
	{
		b[i] = 0;
	}
    b.decDigits = 0;
	return b;
}



char & DecNumber::operator[](int b)
{
    if (b >= 0 && b < N)
    {
        return this->a->number[b];
    }
    throw NumberException(Kalkulator::tr("Error, out of bounds"));
}


// a % b == a * (a/b)*b -- kde je vyuzito celociselne deleni
DecNumber operator%(DecNumber& a, DecNumber& b)
{
    if (a.decDigits || b.decDigits)
        throw("Expression must be integral type");

    // celociselne vydeleni cisla a cislem b
    DecNumber c = a / b;
    c = intPart(c);

    c *= b;
    c = a - c;
    return c;
}

//fce ktera prevede DecNumber na string
std::string num2str(DecNumber& num)
{
    if(num.decDigits == 0 && num.a->getDigits() == 0)
        return "0";
    std::string str = "";
    if(num <= 0)
        str += '-';
    char l;
    if (num.a->getDigits() > M)
    {
        for(int i = N - num.a->getDigits(); i < N - M ; i++)
        {
            l = num[i] + '0';
            str = str + l;
        }
    }
    else
        str = str + "0"; // |num| < 1
    if(num.decDigits)
    {
        str = str + '.';
        for (int i = N - M; i < N - M + num.decDigits; i++)
        {
            l = num[i] + '0';
            str = str + l;
        }
    }
    return str;
}

//dava cislo s presnosti na 97 desetinnych mist
DecNumber ln2()
{
	// puvodni fce pouzita pro vypocet cisla ln2
	// kvuli usetreni rychlosti uz predpocitana
    /*DecNumber suma("0.0");
	DecNumber jedna("1.0");
	DecNumber dva("2.0");
	DecNumber n("1.0");
	DecNumber pom("1.0");
	for (int i = 1; i < 350; i++)
	{
		pom = dva * n;
		pom = jedna / pom;
		suma += pom;

		dva *= 2;
		n += jedna;
	}
    return suma;*/

    return DecNumber(ln_2); // pro vetsi rychlost je konstanta ln2 predem predpocitana
}

//dava cislo s presnosti na 98 desetinnych mist
DecNumber euler()
{
    /*DecNumber e("2.0");
	DecNumber fak("1.0");
	for (int i = 2; i <= 80; i++)
	{
		fak *= i;
		DecNumber jedna("1.0");
		jedna /= fak;
		e += jedna;
	}
    e.setDigits();*/

    return DecNumber(e); // pro vetsi rychlost je konstanta e uz predem predpocitana
}

//babylonska metoda
// x_n = (x_(n-1)+a/x_(n-1))
DecNumber babylon_sqrt(DecNumber & a)
{
    if(a == 0) // odmocnina z nuly
    {
        return DecNumber("0.0");
    }
    else if(a <= 0) // odmocnina ze zaporneho cisla
    {
        throw NumberException(Kalkulator::tr("Sqaure root of negative number."));
    }
	DecNumber pom("0.0");
	DecNumber x_0("0.0");
	
	DecNumber x_1("1.0");
	DecNumber nula("0.0");
	int i = 1;
    while(x_0 != x_1) // opakuj dokud nezkonverguje
	{
		x_0 = x_1;
		pom = a / x_0;
		pom += x_0;
		pom /= 2;
		x_1 = pom;
		i++;
	}
	return x_1;
}

// if(a > 0) a^b = e^(b * lna) 
DecNumber mocnina(DecNumber& a, DecNumber& b)
{
    if(a == 0)
    {
        if (b == 0)
            throw NumberException(Kalkulator::tr("Error 0^0"));
        return DecNumber("0.0");
    }
    if(b == 0)
        return DecNumber("1.0");
    if (b == 1)
        return a;
    DecNumber pom = intPart(b);
    if (!a.a->getSign() || b == pom)
	{
		if (b.decDigits != 0)
            throw NumberException(Kalkulator::tr("Root of negative number."));
		
		std::string str = num2binary(b);
		if (str.length() == 0)
            return DecNumber("1.0");
		else
            pom = a;
		DecNumber suma("1.0");
        for (int i = 0; i < (int)str.length(); i++)
		{
			if (str[i] == '1')
                suma *= pom;
            pom *= pom;
		}
        /*if (str[0] == 1 && !a.a->getSign())
            suma *= -1;*/
		return suma;
	}
	else
	{
        DecNumber lna = logarithm(a);
        DecNumber blna = b * lna;
        DecNumber res = exponential(blna);
        return res;
    }
}

DecNumber  sinus(DecNumber& a)
{
	DecNumber pom(a);
	DecNumber dvepi = pi;
	dvepi *= 2;
	DecNumber p = pi;
	while (pom >= p)
	{
		pom -= dvepi;
	}
	p *= -1;
	while (p >= pom)
	{
		pom += dvepi;
	}
	//nyni je pom z intervalu (-pi;pi), kde taylorova rada konverguje pomerne rychle
	p = pom;
	p *= p;
	DecNumber suma("0.0");
	DecNumber n("1.0");
	int i = 0;
	DecNumber mezires("1.0");
    while (mezires != 0)
	{
		mezires = pom / n;
		if (i % 2)
			mezires *= -1;
		suma += mezires;
		pom *= p;
		i++;
		n *= (2 * i + 1);
		n *= (2 * i);
	}
	return suma;
}


// cos(x) = sin(x+pi/2)
DecNumber cosinus(DecNumber& a)
{
	DecNumber pipul = pi;
	pipul /= 2;
	pipul += a;
	return sinus(pipul);
}

DecNumber fakt(DecNumber & a)
{
    if(a.decDigits)
        throw NumberException(Kalkulator::tr("Factorial of non-integer."));
    if(a.a->getDigits() == 0)
        return DecNumber("1.0");
    DecNumber pom(a);
    bool t = a.a->getSign();
    if(t == false)
        pom*=-1;
    DecNumber fac("1.0");
    while(pom != 1)
    {
        fac *= pom;
        pom -= 1;
    }
    if (t == false)
        fac *= -1;
    return fac;
}

//pri aproximacich muze vyjit cislo 0.9999999999999999999... nebo 1.00000000000000...2
void DecNumber::round()
{
    if(decDigits == M)
    {
        unsigned int i = N - M;
        while(this->a->number[i] == 0)
            i++;
        if (i > N - 5)
        {
            for (; i < N; i++)
            {
                this->a->number[i] = 0;
                this->decDigits = 0;
            }
            return;
        }

        unsigned int j = N - M;
        while(this->a->number[j] == 9)
            j++;
        if(j > N - 5)
        {
            j = N - M;
            for(;j < N;j++)
            {
                this->a->number[j] = 0;
                this->decDigits = 0;
            }
            *this+=1;
            return;
        }
    }
}
