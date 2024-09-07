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
    m_language = Norwegian;         // TODO: get from input
    m_char_increment = 2;         // TODO: get from input

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

    // Set the locale to read UTF-8 encoded input and write UTF-8 encoded output
    // without this the code can't handle characters larger than 1 byte
    fin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    fout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

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
        max_lc += 3;        // additional 3 letters: æ, ø, å
        max_capital +=3;    // additional 3 letters: Æ, Ø, Å
        break;
    case British: // Fallthrough
    default:
        break;
    }

    // convert letter to integer
    int letter_val = wchar2uint(letter);

    // lower case letters
    if ((min_lc <= letter_val) && (letter_val <= max_lc))
        return uint2wchar(incrementVal(letter_val, min_lc, max_lc, increment));

    // capital letters
    if ((min_capital <= letter_val) && (letter_val <= max_capital))
        return uint2wchar(incrementVal(letter_val, min_capital, max_capital, increment));

    // return chars that are not letter of the alfabet unchanged
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

uint Caesar::wchar2uint(wchar_t ch)
{
    switch (m_language) {
    case Norwegian:
        // adjust the int value of norwegian specific letters, so that all the letters of the alfabet have consecutive values
        if (ch == L'æ')
            return m_z_val + 1;
        if (ch == L'ø')
            return m_z_val + 2;
        if (ch == L'å')
            return m_z_val + 3;
        if (ch == L'Æ')
            return m_Z_val + 1;
        if (ch == L'Ø')
            return m_Z_val + 2;
        if (ch == L'Å')
            return m_Z_val + 3;
        break;
    case British: // Fallthrough
    default:
        break;
    }

    // translate regularly
    return static_cast<uint>(ch);
}

wchar_t Caesar::uint2wchar(uint val)
{
    switch (m_language) {
    case Norwegian:
        // adjust the int value of norwegian specific letters, so that all the letters of the alfabet have consecutive values
        if (val == m_z_val + 1)
            return L'æ';
        if (val == m_z_val + 2)
            return L'ø';
        if (val == m_z_val + 3)
            return L'å';
        if (val == m_Z_val + 1)
            return L'Æ';
        if (val == m_Z_val + 2)
            return L'Ø';
        if (val == m_Z_val + 3)
            return L'Å';
        break;
    case British: // Fallthrough
    default:
        break;
    }

    // translate regularly
    return static_cast<wchar_t>(val);
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

