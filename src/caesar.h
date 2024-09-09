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

    enum Method
    {
        M_Caesar = 0,
        M_Brutus = 1
    };

    enum Language
    {
        British   = 0,
        Norwegian = 1
    };

    enum OutputText
    {
        None      = 0,
        Original  = 1,
        Encrypted = 2,
        Decrypted = 3
    };

private:
    void    initialize();
    void    caesarEncrypt(bool decrypt = false);
    void    brutusEncrypt(bool decrypt = false);
    uint    incrementVal(uint val, const uint min_val, const uint max_val, int increment);
    wchar_t incrementChar(wchar_t letter, int increment);
    uint    wchar2uint(wchar_t ch);
    wchar_t uint2wchar(uint val);
    void    updateFileNameAndPath();
    void    writeFileNotFoundToGui();
    void    setupCaesarWindow();
    void    setupBrutusWindow();
    void    closeMethodSelectorWindow();
    void    outputTextFileToGui(std::string input_file);
    void    setGuiScrollLabel(OutputText label);
    void    outputEncryptedFileLocation(std::string output_file, bool decrypt);

    Method      m_encryption_method;
    std::string m_text_file_name;
    std::string m_text_file_location;
    Language    m_language;
    int         m_char_increment;
    std::string m_brutus_key_file_name = "brutus_key";
    uint        m_a_val = 'a';
    uint        m_z_val = 'z';
    uint        m_A_val = 'A';
    uint        m_Z_val = 'Z';

private slots:
    void on_pushButton_Caesar_clicked();
    void on_pushButton_Brutus_clicked();
    void on_comboBox_method_currentIndexChanged(int index);
    void on_pushButton_Encrypt_clicked();
    void on_pushButton_Decrypt_clicked();
    void on_pushButton_LoadFile_clicked();
    void on_pushButton_Write2File_clicked();
    void on_comboBox_language_activated(int index);
    void on_increment_valueChanged(int new_increment);
    void on_pushButton_GenerateKey_clicked();
    void on_pushButton_Exit_clicked();

private:
    Ui::Caesar *ui;
};
#endif // CAESAR_H
