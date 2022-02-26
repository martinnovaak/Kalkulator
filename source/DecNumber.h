#pragma once
#include "Number.h"

//lehce modifikovane trida pro pocitani celych cisel tak, aby se
//s ni dali pocitat i jako s desetinnymi
//prvnich N-M prvku pole a->number predstavuje cislice cele casti cisla
//a poslednich M prvku predstavuje cislice desetinne casti cisla

class DecNumber {
private:
	Number * a;
    int decDigits; // pocet desetinnych mist, je-li rovno nule jedna
                   // se o cele cislo, jinak o desetinne
public:
	DecNumber();
    DecNumber(const DecNumber& a);
	DecNumber(DecNumber&& b);
	DecNumber(std::string a);
	~DecNumber();

    DecNumber& operator=(const DecNumber& a);
    DecNumber& operator=(DecNumber&& a);

	void operator+=(DecNumber& d);
	void operator-=(DecNumber& d);
	void operator*=(DecNumber& d);
	void operator/=(DecNumber& d);

	void operator+=(int a);
	void operator-=(int a);
    void operator*=(int a);
    void operator/=(int a);

    void operator++(int);
    void operator--(int);

	bool operator>=(DecNumber& a);

	bool operator==(DecNumber& a);
	bool operator!=(DecNumber& a);

    bool operator>=(int a);
    bool operator<=(int a);
    bool operator==(int a);
    bool operator!=(int a);


    void round();

	void setDigits();
    int getDigits(){return decDigits;};

	void print();

    char & operator[](int a);
	friend DecNumber operator/(DecNumber& a, DecNumber& b);
	friend DecNumber exponential(DecNumber& a);
	friend DecNumber intPart(DecNumber& a);	
    friend std::string num2str(DecNumber& a);
    friend DecNumber operator%(DecNumber& a, DecNumber& b);
	friend std::string num2binary(DecNumber& a);
	friend DecNumber mocnina(DecNumber& a, DecNumber& b);
    friend DecNumber fakt(DecNumber & a);
};

DecNumber operator+(DecNumber& a, DecNumber& b);
DecNumber operator-(DecNumber& a, DecNumber& b);
DecNumber operator*(DecNumber& a, DecNumber& b);
DecNumber operator/(DecNumber& a, DecNumber& b);
DecNumber operator%(DecNumber& a, DecNumber& b); // treba dodelat

DecNumber euler();
DecNumber exponential(DecNumber& a);
DecNumber logarithm(DecNumber& a);
DecNumber log(DecNumber& a, DecNumber& base);
DecNumber intPart(DecNumber& a);
DecNumber ln2();
DecNumber babylon_sqrt(DecNumber& a);
DecNumber sinus(DecNumber& a);
DecNumber cosinus(DecNumber& a);

DecNumber mocnina(DecNumber& a, DecNumber& b);
DecNumber fakt(DecNumber & a);

std::string num2str(DecNumber& a);
std::string num2binary(DecNumber& a);

//predem predpocitane konstanty kvuli usetreni rychlosti
const DecNumber ln_2("0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875"); // retezec vypocteny pres fci ln2()
const DecNumber e("2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274"); // retezec vypocteny pres fci euler()
const DecNumber pi("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679");
