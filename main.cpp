#include <QApplication>
#include "caesar.h"
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInfo() << "Current working directory:" << QDir::currentPath();
    a.setStyleSheet("QWidget {background-image: url(./../caesar_background.png) }");
    Caesar w;
    w.resize(832, 478);
    w.show();
    return a.exec();
}
