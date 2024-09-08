#include <QApplication>
#include "caesar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Caesar caesar;
    caesar.resize(1924, 1050);
    caesar.setStyleSheet("QMainWindow {"
                         "background-image: url(/home/endredi/Caesar/graphics/caesar_method_selector.png);"
                         "background-position: center;"
                         "background-repeat: no-repeat;"
                         "}");
    caesar.show();

    return a.exec();
}
