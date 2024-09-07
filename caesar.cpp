#include "caesar.h"
#include "./ui_caesar.h"
#include <cmath>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

using std::isfinite;
using std::wifstream;
using std::wofstream;
using std::ios;
using std::codecvt_utf8;

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

void Caesar::caesarEncrypt(bool decrypt)
{
    // DEBUG
    m_text_file_name = "some_text"; // TODO: get from input

    /// open input file
    std::string input_file = m_text_file_location + m_text_file_name + ".txt";

    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    wifstream fin;
    fin.open(input_file);

    if (fin.fail() || !fin.is_open())
    {
        qWarning() << "Error: opening input file failed";
        return;
    }


    /// set output file name and sign of increment value depending on encryption/decryption
    int increment = m_char_increment;
    std::string output_file = m_text_file_location + m_text_file_name;
    if (decrypt)
    {
        output_file += "_decrypted.txt";
        increment = -increment;
    }
    else
        output_file += "_encrypted.txt";


    /// open output file
    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    wofstream fout;
    fout.open(output_file);

    if (fout.fail() || !fout.is_open())
    {
        qWarning() << "Error: opening output file failed";
        return;
    }


    /// read file, and write encrypted/decrypted text to output
    wchar_t letter;
    uint letter_val;
    fin.get(letter);
    while (!fin.eof())
    {
        //letter_val = letter;
        //qInfo() << "letter =" << letter << ", val =" << letter_val;


        fout << incrementChar(letter, increment);
        fin.get(letter);
    }

    // close files
    fin.close();
    fout.close();

    if (decrypt)
        qInfo() << "The decrypted text was written to" << QString::fromStdString(output_file);
    else
        qInfo() << "The encrypted text was written to" << QString::fromStdString(output_file);
}

/// Caesar encryption
/// using wchar_t instead of char, because 'æ', 'ø', 'å' are larger than 1 byte
wchar_t Caesar::incrementChar(wchar_t letter, int increment)
{
    uint min_lc = m_a_val;
    uint max_lc = m_z_val;
    uint min_capital = m_A_val;
    uint max_capital = m_Z_val;
    switch (m_language) {
    case Norwegian:
        qInfo() << "norwegian not implemented yet";
        max_lc += 3;
        max_capital +=3;
        return '!';
        break;
    case British: // Fallthrough
    default:
        // default british
        break;
    }

    // convert letter to integer
    int letter_val = static_cast<uint>(letter);

    // lower case letters
    if ((min_lc <= letter_val) && (letter_val <= max_lc))
    {
        return static_cast<wint_t>(incrementVal(letter_val, min_lc, max_lc, increment));
    }

    // capital letters
    if ((min_capital <= letter_val) && (letter_val <= max_capital))
    {
        return static_cast<wint_t>(incrementVal(letter_val, min_capital, max_capital, increment));
    }

    // left unchanged
    //qInfo() << "left unchanged =" << letter;


    return letter;
}

/// x_incremented = (x - min + increment) % (max - min + 1) + min
uint Caesar::incrementVal(uint val, const uint min_val, const uint max_val, int increment)
{
    if (!std::isfinite(val) || !std::isfinite(min_val) || !std::isfinite(max_val) || !std::isfinite(increment))
    {
        qWarning() << "invalid input";
        return 33; // "!" in ASCII
    }

    if (max_val < min_val)
    {
        qWarning() << "min_val is greater than max_val";
        return 33; // "!" in ASCII
    }

    int n_values = max_val - min_val + 1;
    int val_incremented_offset = val - min_val + increment;

    // make sure the value is positive, otherwise the modulo operator will return a negative value
    while (val_incremented_offset < 0)
        val_incremented_offset += n_values;

    return val_incremented_offset % n_values + min_val;
}

void Caesar::on_pushButton_Encrypt_clicked()
{
    caesarEncrypt();
}


void Caesar::on_pushButton_Decrypt_clicked()
{
    caesarEncrypt(true);
}


void Caesar::on_pushButton_Exit_clicked()
{
    QApplication::quit();
}

