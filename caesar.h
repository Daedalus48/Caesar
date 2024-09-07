#ifndef CAESAR_H
#define CAESAR_H

#include <QMainWindow>
#include <cstring>

QT_BEGIN_NAMESPACE
namespace Ui { class Caesar; }
QT_END_NAMESPACE

class Caesar : public QMainWindow
{
    Q_OBJECT

public:
    Caesar(QWidget *parent = nullptr);
    ~Caesar();

    enum Language
    {
        British = 0,
        Norwegian = 1
    };

protected:
    void    caesarEncrypt(bool decrypt = false);
    uint    incrementVal(uint val, const uint min_val, const uint max_val, int increment);
    wchar_t incrementChar(wchar_t letter, int increment);
    uint    wchar2uint(wchar_t ch);
    wchar_t uint2wchar(uint val);

    std::string m_text_file_name;
    std::string m_text_file_location = "/home/endredi/Caesar/";
    Language    m_language = British;
    int         m_char_increment = 1;
    uint        m_a_val = 'a';
    uint        m_z_val = 'z';
    uint        m_A_val = 'A';
    uint        m_Z_val = 'Z';

private slots:
    void on_pushButton_Encrypt_clicked();
    void on_pushButton_Decrypt_clicked();
    void on_pushButton_Exit_clicked();

private:
    Ui::Caesar *ui;
};
#endif // CAESAR_H
