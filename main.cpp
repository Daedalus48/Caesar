#include <QApplication>
#include <QDir>
//#include "method_selector.h"
#include "caesar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget {background-image: url(/home/endredi/Caesar/caesar_background.png) }");
//    MethodSelector method;
//    method.resize(832, 478);
//    method.show();
    Caesar caesar;
    caesar.resize(832, 478);
    caesar.show();

    return a.exec();
}
