#include <QApplication>
#include <QDir>
//#include "method_selector.h"
#include "caesar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget {background-image: url(/home/endredi/Caesar/graphics/caesar_background.png) }");
    Caesar caesar;
    caesar.resize(832, 478);
    caesar.show();

    return a.exec();
}
