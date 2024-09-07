#ifndef CAESAR_H
#define CAESAR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Caesar; }
QT_END_NAMESPACE

class Caesar : public QMainWindow
{
    Q_OBJECT

public:
    Caesar(QWidget *parent = nullptr);
    ~Caesar();

private slots:
    void on_pushButton_Encrypt_clicked();
    void on_pushButton_Decrypt_clicked();
    void on_pushButton_Exit_clicked();

private:
    Ui::Caesar *ui;
};
#endif // CAESAR_H
