#include <string>
#include <stack>
#include "DecNumber.h"

int priority(char op);
bool isOperator(char op);
Number applyOp(Number a, Number b, char op);
DecNumber applyOp2(DecNumber a, DecNumber b, char op);
void control( std::string & a, std::stack<char> & ops);
void setDecNumber(DecNumber & val, std::string & str, unsigned int & i);
void setNumber(std::string & str, unsigned int & i, std::stack<DecNumber> & decValues, std::stack<Number> & intValues, bool decimal, bool negative);
void calculate(std::stack<char> & ops, std::stack<DecNumber> & decValues, std::stack<Number> & intValues, bool decimal);
DecNumber recNum(const std::string & str, unsigned int & i);
std::string evaluate(std::string str, bool decimal);
std::string eval(std::string str, bool decimal, int l);
