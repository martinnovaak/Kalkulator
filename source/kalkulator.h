#ifndef KALKULATOR_H
#define KALKULATOR_H

#include <QWidget>

class QLabel;
class QPushButton;
class QLineEdit;
class QStatusBar;
class QRadioButton;
class QSpinBox;
class QListWidget;

class Kalkulator : public QWidget
{
    Q_OBJECT
    QLabel * lbl;
    QLineEdit * ledit;
    QPushButton * storno;
    QPushButton * backSpace;
    //cisla
    QPushButton * num1;
    QPushButton * num2;
    QPushButton * num3;
    QPushButton * num4;
    QPushButton * num5;
    QPushButton * num6;
    QPushButton * num7;
    QPushButton * num8;
    QPushButton * num9;
    QPushButton * num0;


    QPushButton * decPoint;
    //operace
    QPushButton * div;
    QPushButton * mult;
    QPushButton * minus;
    QPushButton * plus;

    QPushButton * equals;

    //konstanty
    QPushButton * pi;
    QPushButton * e;

    // gonio
    QPushButton * sin;
    QPushButton * cos;

    //logaritmy
    QPushButton * log;
    QPushButton * ln;

    //závorky
    QPushButton * leftBracket;
    QPushButton * rightBracket;

    QPushButton * exp; // mocnina
    QPushButton * sqrt; // druha odmocnina

    QPushButton * mod; // modulo
    QPushButton * fak; // faktorial

    QRadioButton * deci;
    QRadioButton * inte;

    QLabel * numOfDec;
    QSpinBox * spinBox;

    QPushButton * setButton;

    QStatusBar * statBar;

    QListWidget * listwidget;
    QPushButton * hideButton;
    QPushButton * itemremoveButton;
    QPushButton * clearButton;

    void setupForm();
    void loadSettings();
    void setEn();

    bool decimal;
    unsigned int num;
public:
    Kalkulator(QWidget *parent = nullptr);
    ~Kalkulator();
private slots:
    void pushed();
    void calculate();
    void setSettings();
    void hideshow();
};
#endif // KALKULATOR_H
