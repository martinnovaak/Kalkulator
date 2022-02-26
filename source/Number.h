#include "NumberException.h"

const int N = 300;

//_________________________
const int M = 100;
//_________________________

class Number
{
private:
	static int pocet; // pocet instanci
	char* number; // cislo
	bool sign; // znamenko true == +, false == -
	int digits; // pocet cislic cisla

	void setDigits();
	void setNumber(int);
public:
	Number();
	Number(int a);

	Number(const Number& kop);
	Number(Number&& zdroj);

	Number(const std::string& a);
	~Number();

	static int getPocet() { return pocet; };

	char& operator[](int a);

	int getDigits() { return this->digits; };
	bool getSign() { return this->sign; };

	Number& operator=(const Number& d);
	Number& operator=(Number&& d);

	void operator+=(Number& d);
	void operator-=(Number& d);
	void operator*=(Number& d);
	void operator/=(Number& d);

	void multiplication(Number& d, bool decimal);
	void division(Number& d, bool decimal);

	void operator*=(int a);
	void operator-=(int a);
	void operator+=(int a);
	void operator/=(int a);

	bool operator==(Number& d);
	bool operator!=(Number& d);
	bool operator>=(Number& d);
	bool isBigger(Number& a);

	void operator++(int);
	void operator--(int);
	void operator=(int);

	bool operator<=(int);
	bool operator>=(int);
	
	bool operator<(int);
	bool operator>(int);
	bool operator==(int);
	bool operator!=(int);

	void Print();
    bool absEq(Number & a);
private:
	friend Number operator+(Number& a, Number& b);
	friend Number operator-(Number& a, Number& b);
	friend Number operator*(Number& a, Number& b);
	friend Number operator/(Number& a, Number& b);
	friend Number operator%(Number& a, Number& b);
    //friend std::ostream& operator<<(std::ostream& os, Number& a);
    //friend std::ostream& operator<<(std::ostream& os, Number&& a);
	friend std::string num(Number& a);

	//_________________________________________
	friend Number operator* (Number& a, int b);

	friend class DecNumber;
};


Number operator+(Number& a, Number& b);
Number operator-(Number& a, Number& b);
Number operator*(Number& a, Number& b);
Number operator/(Number& a, Number& b);
Number operator%(Number& a, Number& b);

Number operator* (Number& a, int b);
/*std::ostream& operator<<(std::ostream& os, Number& a);
std::ostream& operator<<(std::ostream& os, Number&& a);*/
std::string num(Number& a);
