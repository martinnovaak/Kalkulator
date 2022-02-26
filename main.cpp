#include "kalkulator.h"
#include <QIcon>
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/calculator.svg")); //<div>Icons made by <a href="https://www.flaticon.com/authors/pixel-perfect" title="Pixel perfect">Pixel perfect</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>
    //Překlad
    QTranslator translator;
    translator.load("Kalkulator_cs_CZ",":translations");
    a.installTranslator(&translator);
    //_______________________________________

    Kalkulator w;
    w.show();
    return a.exec();
}
