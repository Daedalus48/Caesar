#include "caesar.h"
#include "./ui_caesar.h"
#include <QDebug>

Caesar::Caesar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Caesar)
{
    ui->setupUi(this);
}

Caesar::~Caesar()
{
    delete ui;
}


void Caesar::on_pushButton_Encrypt_clicked()
{
    qInfo() << "Encrypt button pushed";
}


void Caesar::on_pushButton_Decrypt_clicked()
{
    qInfo() << "Decrypt button pushed";
}


void Caesar::on_pushButton_Exit_clicked()
{
    qInfo() << "Exit button pushed";
    QApplication::quit();
}

