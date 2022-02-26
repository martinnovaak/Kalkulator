#include "kalkulator.h"
#include "exp_solver.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QListWidget>
#include <QStatusBar>
#include <QSpacerItem>
#include <QSettings>

Kalkulator::Kalkulator(QWidget *parent)
    : QWidget(parent)
{
    decimal = 1;
    this->setWindowTitle(tr("Calculator"));
    setupForm();
    loadSettings();
    this->statBar->showMessage(tr("Calculator with up to 200 integer digits and 100 decimal digits"));
}


void Kalkulator::setupForm()
{
    statBar = new QStatusBar;
    QVBoxLayout * layout = new QVBoxLayout;

    QVBoxLayout * layoutText = new QVBoxLayout;
    lbl = new QLabel(tr("Calculator"));
    ledit = new QLineEdit;
    layoutText->addWidget(lbl);
    layoutText->addWidget(ledit);
    layout->addLayout(layoutText);

    QVBoxLayout * layoutBtns = new QVBoxLayout;

    QHBoxLayout * row1 = new QHBoxLayout;
    pi = new QPushButton("π");
    pi->setStatusTip(tr("The constant π (pi)."));
    sin = new QPushButton("&sin");
    sin->setStatusTip(tr("Sine"));
    cos = new QPushButton("&cos");
    cos->setStatusTip(tr("Cosine"));
    leftBracket = new QPushButton("(");
    leftBracket->setStatusTip(tr("("));
    rightBracket = new QPushButton(")");
    rightBracket->setStatusTip(tr(")"));
    row1->addWidget(pi);
    row1->addWidget(sin);
    row1->addWidget(cos);
    row1->addWidget(leftBracket);
    row1->addWidget(rightBracket);
    layoutBtns->addLayout(row1);


    QHBoxLayout * row2 = new QHBoxLayout;
    e = new QPushButton("&e");
    e->setStatusTip(tr("The Euler's constant e."));
    ln = new QPushButton("&ln");
    ln->setStatusTip(tr("Natural logarithm"));
    log = new QPushButton("log");
    log->setStatusTip(tr("Logarithm"));
    exp = new QPushButton("^");
    exp->setStatusTip(tr("Exponentiation"));
    div = new QPushButton("/");
    div->setStatusTip(tr("Division"));
    row2->addWidget(e);
    row2->addWidget(ln);
    row2->addWidget(log);
    row2->addWidget(exp);
    row2->addWidget(div);
    layoutBtns->addLayout(row2);

    QHBoxLayout * row3 = new QHBoxLayout;
    storno = new QPushButton("A&C");
    storno->setStatusTip(tr("All clear"));
    num7 = new QPushButton("7");
    num8 = new QPushButton("8");
    num9 = new QPushButton("9");
    mult = new QPushButton("x");
    mult->setStatusTip(tr("multiplication"));
    row3->addWidget(storno);
    row3->addWidget(num7);
    row3->addWidget(num8);
    row3->addWidget(num9);
    row3->addWidget(mult);
    layoutBtns->addLayout(row3);

    QHBoxLayout * row4 = new QHBoxLayout;
    backSpace = new QPushButton;
    backSpace->setIcon(QIcon(":/icons/backspace.svg"));
    backSpace->setStatusTip(tr("BackSpace"));
    num4 = new QPushButton("4");
    num5 = new QPushButton("5");
    num6 = new QPushButton("6");
    minus = new QPushButton("-");
    minus->setStatusTip(tr("Minus"));
    row4->addWidget(backSpace);
    row4->addWidget(num4);
    row4->addWidget(num5);
    row4->addWidget(num6);
    row4->addWidget(minus);
    layoutBtns->addLayout(row4);

    QHBoxLayout * row5 = new QHBoxLayout;
    mod = new QPushButton("%");
    mod->setStatusTip(tr("Modulo"));
    num1 = new QPushButton("1");
    num2 = new QPushButton("2");
    num3 = new QPushButton("3");
    plus = new QPushButton("+");
    plus->setStatusTip(tr("Plus"));
    row5->addWidget(mod);
    row5->addWidget(num1);
    row5->addWidget(num2);
    row5->addWidget(num3);
    row5->addWidget(plus);
    layoutBtns->addLayout(row5);

    QHBoxLayout * row6 = new QHBoxLayout;
    fak = new QPushButton("!");
    fak->setStatusTip(tr("Factorial"));
    sqrt = new QPushButton("√");
    sqrt->setStatusTip(tr("Square root"));
    num0 = new QPushButton("0");
    decPoint = new QPushButton(tr("."));
    decPoint->setStatusTip(tr("Decimal point"));
    equals = new QPushButton("=");
    equals->setStatusTip(tr("Calculate"));
    row6->addWidget(fak);
    row6->addWidget(sqrt);
    row6->addWidget(num0);
    row6->addWidget(decPoint);
    row6->addWidget(equals);
    layoutBtns->addLayout(row6);
    layout->addLayout(layoutBtns);

    QHBoxLayout * settings = new QHBoxLayout;
    deci = new QRadioButton(tr("decimal"));
    deci->setChecked(true);
    inte = new QRadioButton(tr("integral"));
    numOfDec = new QLabel(tr("Number of decimal digits"));
    spinBox = new QSpinBox;
    spinBox->setRange(1,M);
    spinBox->setValue(M);
    setButton = new QPushButton(tr("Set settings"));
    settings->addWidget(deci);
    settings->addWidget(inte);
    settings->addWidget(numOfDec);
    settings->addWidget(spinBox);
    settings->addWidget(setButton);
    layout->addLayout(settings);

    listwidget = new QListWidget();
    hideButton = new QPushButton(tr("Hide history"));
    itemremoveButton = new QPushButton(tr("Remove item"));
    clearButton = new QPushButton(tr("Clear history"));
    //QSpacerItem * spacer = new QSpacerItem(200, hideButton->height(),QSizePolicy::Minimum, QSizePolicy::Expanding);

    layout->addWidget(listwidget);
    QHBoxLayout * listviewbtns = new QHBoxLayout();
    listviewbtns->addWidget(hideButton);
    listviewbtns->addWidget(itemremoveButton);
    //listviewbtns->addItem(spacer);
    listviewbtns->addWidget(clearButton);
    layout->addLayout(listviewbtns);

    layout->addWidget(statBar);
    this->setLayout(layout);


    connect(num0, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num1, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num2, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num3, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num4, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num5, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num6, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num7, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num8, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(num9, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(decPoint, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(backSpace, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(storno, SIGNAL(clicked()), this, SLOT(pushed()));

    connect(plus, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(minus, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(mult, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(div, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(mod, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(fak, SIGNAL(clicked()), this, SLOT(pushed()));

    connect(e, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(pi, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(ln, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(log, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(exp, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(leftBracket, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(rightBracket, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(sin, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(cos, SIGNAL(clicked()), this, SLOT(pushed()));
    connect(sqrt, SIGNAL(clicked()), this, SLOT(pushed()));


    connect(equals, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(setButton, SIGNAL(clicked()), this, SLOT(setSettings()));

    connect(clearButton, &QPushButton::clicked, this->listwidget, &QListWidget::clear);
    connect(itemremoveButton, &QPushButton::clicked, this, [this]{delete this->listwidget->takeItem(listwidget->currentRow());});
    connect(hideButton, &QPushButton::clicked, this, &Kalkulator::hideshow);

    this->setFixedSize(0,0);
}

void Kalkulator::setSettings()
{
    decimal = deci->isChecked();
    num = spinBox->value();
    QSettings settings("FNSPE CTU", "Kalkulator");
    settings.setValue("decimal", decimal);
    settings.setValue("numOfDec", num);
    setEn();
}

void Kalkulator::hideshow()
{
    if (this->listwidget->isVisible())
    {
        this->listwidget->setVisible(false);
        this->hideButton->setText(tr("Show history"));
    }
    else
    {
        this->listwidget->setVisible(true);
        this->hideButton->setText(tr("Hide history"));
    }
        this->setFixedSize(0,0);
}

void Kalkulator::setEn()
{
    this->pi->setEnabled(decimal);
    this->e->setEnabled(decimal);
    this->ln->setEnabled(decimal);
    this->log->setEnabled(decimal);
    this->exp->setEnabled(decimal);
    this->sqrt->setEnabled(decimal);
    this->decPoint->setEnabled(decimal);
    this->exp->setEnabled(decimal);
    this->sin->setEnabled(decimal);
    this->cos->setEnabled(decimal);
    this->spinBox->setEnabled(decimal);
}

void Kalkulator::loadSettings()
{
    QSettings settings("FNSPE CTU", "Kalkulator");
    decimal = settings.value("decimal", decimal).toBool();
    num = settings.value("numOfDec", num).toInt();

    spinBox->setValue(num);
    if(!decimal)
        inte->setChecked(true);
    setEn();
}

void Kalkulator::pushed()
{
   QObject * obj = QObject::sender();
   if (obj == num0)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"0"));
   else if(obj == num1)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"1"));
   else if(obj == num2)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"2"));
   else if(obj == num3)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"3"));
   else if(obj == num4)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"4"));
   else if(obj == num5)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"5"));
   else if(obj == num6)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"6"));
   else if(obj == num7)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"7"));
   else if(obj == num8)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"8"));
   else if(obj == num9)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"9"));
   else if(obj == decPoint)
    this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(), tr(".")));
   else if(obj == storno)
   {
       this->ledit->setText("");
       this->lbl->setText(tr("Calculator"));
       this->statBar->showMessage(tr("Calculator"));
   }
   else if (obj == plus)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"+"));
   else if (obj == minus)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"-"));
   else if (obj == mult)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"x"));
   else if (obj == mod)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"%"));
   else if (obj == div)
        this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"/"));
   else if (obj == fak)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"!"));
   else if (obj == e)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"e"));
   else if (obj == pi)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"pi"));
   else if (obj == ln)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"ln("));
   else if (obj == log)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"log10("));
   else if (obj == sin)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"sin(" ));
   else if (obj == cos)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"cos(" ));
   else if (obj == exp)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"^" ));
   else if (obj == sqrt)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"sqrt("));
   else if (obj == leftBracket)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),"(") );
   else if (obj == rightBracket)
       this->ledit->setText(this->ledit->text().insert(this->ledit->cursorPosition(),")"));
   else // backspace
   {
       QString a = this->ledit->text();
       if (a.count() < 1)
           return;
       if(ledit->cursorPosition() == 0 )
           return;
       a.remove(ledit->cursorPosition()-1,1); // vymaze char z indexu pred kurzorem
       this->ledit->setText(a);
   }
}

void Kalkulator::calculate()
{
    try {
        lbl->setText(ledit->text());
        std::string l = eval(ledit->text().toStdString(), this->decimal, num);
        QString qstr =  QString::fromStdString(l);
        listwidget->addItem(ledit->text() + " = " + qstr);
        ledit->setText(qstr);
        this->statBar->showMessage(tr("Calculator with up to 200 integer digits and 100 decimal digits"));
    }  catch (NumberException & e) {
       QMessageBox::warning(this, tr("Error"),tr(e.what()));
    }

}

Kalkulator::~Kalkulator()
{
}

