#include <iostream>
#include <stack>
#include <string>
#include "exp_solver.h"
#include "kalkulator.h"
using namespace std;

bool opControl = true;

//vrati prioritu operátoru
int priority(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/' || op ==  'x' || op == '%') // deleni a nasobeni ma vyssi prioritu jak scitani a odcitani
        return 2;
    else if (op == '^') // mocnina má nejvyssi prioritu
        return 3;

    throw NumberException(Kalkulator::tr("Incorrect operator: %").arg(op));
}

//povolene binarni operatory
bool isOperator(char op)
{
    if (op == '+' || op == '-' || op == '*' || op == 'x' || op == '/' || op == '%' || op == '^')
        return true;
    return false;
}

Number applyOp(Number a, Number b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case 'x': return a * b;
    case '/': return a / b;
    case '%': return a % b;
    default: throw NumberException(Kalkulator::tr("Invalid operation."));
    }
}

DecNumber applyOp2(DecNumber a, DecNumber b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case 'x': return a * b;
    case '/': return a / b;
    case '%': return a % b;
    case '^': return mocnina(a,b);
    default: throw NumberException(Kalkulator::tr("Invalid operation."));
    }
}

//funkce, ktera upravi retezec do podoby ktery umi vyresit
void control( string & a, stack<char> & ops)
{
    unsigned int count1 = 0; // pocet (
    unsigned int count2 = 0; // pocet )
    for (unsigned int i = 0; i < a.length() ; i++)
    {
        if (a[i] == '+') // odstrani dva plusy za sebou
        {
            if (a[i+1] == '+') // "++" == "+"
            {
                a.erase(i, 1);
                i--;
            }
        }
        else if(a[i] == '(')
            count1++;
        else if (a[i] == ')')
            count2++;
        else if (a[i] == ' ') // odstrani mezery
        {
            a.erase(i,1);
            i--;
        }
    }

    // v retezci musi byt stejny pocet levych a pravych zavorek
    if (count1 > count2) // je-li pocet levych zavorek vetsi doplni na konec retezce prave zavorky
        for(unsigned int i = 0; i < count1 - count2; i++)
            a = a + ')';
    else if (count2 > count1) // je-li pocet pravych zavorek vetsi doplni do zasobniku na operace leve zavorky
        for (unsigned int i = 0; i < count2 - count1 ; i++)
            ops.push('(');

    if(a.length() == 0)
        a = a + '0';
}

//najde cely podretezec cisla, zavola jeho konstruktor
void setNumber(string & str, unsigned int & i, stack<DecNumber> & decValues, stack<Number> & intValues, bool decimal, bool negative)
{
    string num = "";
        if (decimal)
        {
            while (i < str.length() && (isdigit(str[i]) || str[i] == '.' || str[i] == ','))
            {
                num = num + str[i];
                i++;
            }
            DecNumber val(num);
            if (negative)
                val *= -1;
            decValues.push(val);
        }
        else
        {
            while (i < str.length() && isdigit(str[i]))
            {
                num = num + str[i];
                i++;
            }
            Number val(num);
            if (negative)
                val *= -1;
            intValues.push(val);
        }
        i--;
}

void calculate(stack<char> & ops, stack<DecNumber> & decValues, stack<Number> & intValues, bool decimal)
{
    if (decimal)
    {
        DecNumber val2 = decValues.top();
        decValues.pop();

        DecNumber val1 = decValues.top();
        decValues.pop();

        char op = ops.top();
        ops.pop();

        decValues.push(applyOp2(val1, val2, op));
    }
    else
    {
        Number val2 = intValues.top();
        intValues.pop();

        Number val1 = intValues.top();
        intValues.pop();

        char op = ops.top();
        ops.pop();

        intValues.push(applyOp(val1, val2, op));
    }
}

DecNumber recNum(const string & str, unsigned int & i, bool & op)
{
    string s = "";
    while(i < str.length() && str[i] != ')')
    {
        s = s + str[i];
        i++;
    }
    if(i == str.length() && str[i-1] != ')')
        throw NumberException(Kalkulator::tr("Missing )"));
    DecNumber val(evaluate(s,true));
    op = false;
    return val;
}

// Zaklad algoritmu je: https://www.geeksforgeeks.org/expression-evaluation/
string evaluate(string str, bool decimal) {
    //inicializace
    unsigned int i;

    // zasobniky na cisla
    stack <DecNumber> decValues;
    stack <Number> intValues;

    // Zasobnik na operace
    stack <char> ops;

    //uprava retezce do zpracovatelneho tvaru
    control(str, ops);

    for (i = 0; i < str.size(); i++)
    {
        //1. krok - Vezmi dalsi polozku ve vstupnum retezci
        if (isdigit(str[i])) // 2. je-li to operand vloz ho do zasobniku s cisly
        {
             opControl = false; // po cislu smi nasledovat operace
             setNumber(str, i, decValues, intValues, decimal, false);
        }
        else if (str[i] == '(') //3. je-li to leva zavorka vloz ji do zasobniku s operatory
        {
            opControl = true; // po leve zavorce nesmi nasledovat operace
            ops.push(str[i]);
        }
        else if (str[i] == ')') // 4. je-li to prava zavorka vypocitej zavorku (retezec zpet az po levou zavorku)
        {
            if(opControl && ops.top() != '(')//jsou-li dva operatory po sobe vyjma prazdnych zavorek
            {
                throw NumberException(Kalkulator::tr("Two operators in a row.)."));
            }
            opControl = false; // po "(" mohou nasledovat dalsi operace
            while (!ops.empty() && ops.top() != '(')
            {
                calculate(ops, decValues, intValues, decimal);
            }

             ops.pop();
        }// 3. je-li to operator vloz ho do zasobniku s operatory
        else if (isOperator(str[i])) // jedna-li se o operator
        {
            if (opControl)//po operatoru nesmi nasledovat operator vyjma "-"
            {
                if (str[i] == '-')
                {
                    if (isdigit(str[i+1]))
                    {
                        setNumber(str, i, decValues, intValues, decimal, true);
                        opControl = false; // po cislu muye opet nasledovat operator
                    }
                    else
                        throw NumberException(Kalkulator::tr("Invalid expression."));
                }
                else
                {
                throw NumberException(Kalkulator::tr("Two operators in a row."));
                }
            }
            else
            {
            opControl = true;        
            //Ma-li predchozi operator >= prioritu nez aktualni
            //tak se predchozi vyraz vypocte (ma prednost pred aktualnim vyrazem)
            while (!ops.empty() && ops.top() != '(' && priority(ops.top()) >= priority(str[i]))
            {
                calculate(ops, decValues, intValues, decimal);
            }
            ops.push(str[i]);
            }
        }
        //konstanty
        else if(str[i] == 'e' && decimal && opControl)
        {
            DecNumber eul(e);
            decValues.push(e);
            opControl = false;
        }
        else if(str[i] == 'p' && str.length() >= i+1 && str[i+1] =='i' && decimal && opControl)
        {
            DecNumber p(pi);
            decValues.push(p);
            opControl = false;
            i++;
        }
        //unarni operatory:
        else if(str[i] == '!' && opControl == false && decimal)
        {
            DecNumber pom = decValues.top();
            decValues.pop();
            decValues.push(fakt(pom));
        }
        else if(str.length() > i + 5 && str[i] == 's' && str[i+1] == 'i' && str[i+2] == 'n' && str[i+3] == '(')
        {
            i+=4;
            DecNumber val = recNum(str, i, opControl);
            DecNumber si = sinus(val);
            decValues.push(si);
        }
        else if(str.length() > i + 5 && str[i] == 'c' && str[i+1] == 'o' && str[i+2] == 's' && str[i+3] == '(')
        {
            i+=4;
            DecNumber val = recNum(str, i, opControl);
            DecNumber co = cosinus(val);
            decValues.push(co);
        }
        else if(str.length() > i + 5 && str[i] == 's' && str[i+1] == 'q' && str[i+2] == 'r' && str[i+3] == 't' && str[i+4] == '(' && opControl && decimal)
        {
            i+=5;
            DecNumber val = recNum(str, i, opControl);
            DecNumber s = babylon_sqrt(val);
            decValues.push(s);
        }
        else if(str[i] == 'l' && decimal && opControl)
        {
            if(i+4 < str.length() && str[i+1] == 'n' && str[i+2] == '(' )
            {
                i += 3;
                DecNumber val = recNum(str, i, opControl);
                DecNumber lg = logarithm(val);
                decValues.push(lg);
            }
            else if(i + 5 < str.length() && str[i+1] == 'o' && str[i+2] == 'g')
            {
                bool dekadicky = true;
                i += 3;
                if(isdigit(str[i]))
                {
                    dekadicky = false;
                    string exp = "";
                    while(str[i] != '(')
                    {
                        exp = exp + str[i];
                        i++;
                    }
                    DecNumber zaklad(evaluate(exp, true));
                    decValues.push(zaklad);
                }
                if (str[i] == '(')
                {
                    i++;
                    DecNumber val = recNum(str, i, opControl);
                    if (!dekadicky)
                    {
                        DecNumber base = decValues.top();
                        decValues.pop();
                        DecNumber lg = log(val, base);
                        decValues.push(lg);
                    }
                    else
                    {
                        DecNumber base("10.0");
                        DecNumber lg = log(val, base);
                        decValues.push(lg);
                    }

                }
                else
                    throw NumberException(Kalkulator::tr("Error in logarithm."));
            }
        }
        else
        {
            throw NumberException(Kalkulator::tr("Invalid expression: %").arg(str[i]));
        }
    }

    //retezec uz byl probehnut
    //zbyva ho dopocitat


    while (!ops.empty()) {
        calculate(ops, decValues, intValues, decimal);
    }//cyklus muze probehnout maximalne 3x, protoze jsou jen tri druhy priority (napr 1 + 2 * 3^4)

    opControl = true;
    // v zasobniku zbyva posledni cislo a to to vysledne
    if (decimal)
    {
        DecNumber res = decValues.top();
        res.round();
        decValues.pop();
        return num2str(res);
    }
    else
    {
        return num(intValues.top());
    }
}

std::string eval(std::string str, bool decimal, int l)
{
    if (decimal)
    {
        DecNumber res(evaluate(str, decimal));
        int k = res.getDigits() - l;
        std::string sres = num2str(res);
        while(k > 0)
        {
            sres.pop_back();
            k--;
        }
        return sres;
    }
    else
        return evaluate(str, decimal);
}
