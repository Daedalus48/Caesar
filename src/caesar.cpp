#include "caesar.h"
#include "./ui_caesar.h"
#include <cmath>
#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <random>

using std::isfinite;
using std::wifstream;
using std::wofstream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::codecvt_utf8;

Caesar::Caesar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Caesar)
{
    ui->setupUi(this);
    initialize();
}

Caesar::~Caesar()
{
    delete ui;
}

void Caesar::caesarEncrypt(bool decrypt)
{
    // read file name and path from GUI
    updateFileNameAndPath();

    /// open input file
    std::string input_file = m_text_file_location + m_text_file_name + ".txt";

    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    wifstream fin;
    fin.open(input_file);

    if (fin.fail() || !fin.is_open())
    {
        writeFileNotFoundToGui();
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
        fout << incrementChar(letter, increment);
        fin.get(letter);
    }

    // close files
    fin.close();
    fout.close();

    if (decrypt)
    {
        QString decrypt_msg = "The decrypted text was written to" + QString::fromStdString(output_file);
        qInfo() << decrypt_msg;

        // Output decrypted text to gui
        outputTextFileToGui(output_file);
        ui->output->append("\n\n" + decrypt_msg);
        setGuiScrollLabel(Decrypted);
    }
    else
    {
        QString encrypt_msg = "The encrypted text was written to" + QString::fromStdString(output_file);
        qInfo() << encrypt_msg;

        // Output encrypted text to gui
        outputTextFileToGui(output_file);
        ui->output->append("\n\n" + encrypt_msg);
        setGuiScrollLabel(Encrypted);
    }
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

void Caesar::brutusEncrypt(bool decrypt)
{
    // read file name and path from GUI
    updateFileNameAndPath();

    /// open input file
    std::string input_file = m_text_file_location + m_text_file_name + ".txt";

    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    wifstream fin;
    fin.open(input_file);

    if (fin.fail() || !fin.is_open())
    {
        writeFileNotFoundToGui();
        qWarning() << "Error: opening input file failed";
        return;
    }

    /// open encryption key
    std::string key_name = m_text_file_location + m_brutus_key_file_name + ".txt";

    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    ifstream fin_key;
    fin_key.open(key_name);

    if (fin_key.fail() || !fin_key.is_open())
    {
        writeFileNotFoundToGui();
        qWarning() << "Error: opening input file failed";
        return;
    }

    /// set output file name depending on encryption/decryption
    std::string output_file = m_text_file_location + m_text_file_name;
    if (decrypt)
    {
        output_file += "_decrypted.txt";
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
    int key;
    int increment;
    uint letter_val;
    fin.get(letter);
    while (!fin.eof())
    {
        fin_key >> key;
        if (fin_key.eof())
        {
            // if the end of the encryption key is reached go back to the beginning
            fin_key.clear();
            fin_key.seekg(0);
            fin_key >> key;
        }
        increment = static_cast<int>(key);
        increment = decrypt ? -increment : increment;
        fout << incrementChar(letter, increment);
        fin.get(letter);
    }

    // close files
    fin.close();
    fout.close();
    fin_key.close();

    if (decrypt)
        qInfo() << "The decrypted text was written to" << QString::fromStdString(output_file);
    else
        qInfo() << "The encrypted text was written to" << QString::fromStdString(output_file);
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

void Caesar::updateFileNameAndPath()
{
    m_text_file_name = ui->fileName->text().toStdString();
    m_text_file_location = ui->fileLocation->text().toStdString();
}

void Caesar::writeFileNotFoundToGui()
{
    // TODO: add text output lable
    //ui->title_caesar->setText("Warning");
    ui->output->setPlainText("Unable to open file. Ensure that the file name and path to file are correct.");
    setGuiScrollLabel(None);
}

void Caesar::setupCaesarWindow()
{
    this->setStyleSheet("QMainWindow {"
                        "background-image: url(/home/endredi/Caesar/graphics/caesar_background.png);"
                        "background-position: center;"
                        "background-repeat: no-repeat;"
                        "}");

    m_encryption_method = M_Caesar;
    ui->generate_key->hide();
    ui->label_increment->show();
    ui->increment->show();
    ui->label_min->show();
    ui->label_max->show();
}

void Caesar::setupBrutusWindow()
{
    this->setStyleSheet("QMainWindow {"
                        "background-image: url(/home/endredi/Caesar/graphics/brutus_background.png);"
                        "background-position: center;"
                        "background-repeat: no-repeat;"
                        "}");

    m_encryption_method = M_Brutus;
    ui->generate_key->show();
    ui->label_increment->hide();
    ui->increment->hide();
    ui->label_min->hide();
    ui->label_max->hide();
}

void Caesar::closeMethodSelectorWindow()
{
    // hide method selector buttons;
    ui->pushButton_Caesar->hide();
    ui->pushButton_Brutus->hide();

    // show stuff that are in both Caesar and Brutus
    ui->label_method->show();
    ui->label_file_location->show();
    ui->label_file_name->show();
    ui->fileLocation->show();
    ui->fileName->show();
    ui->pushButton_LoadFile->show();
    ui->pushButton_Write2File->show();
    ui->pushButton_Decrypt->show();
    ui->pushButton_Encrypt->show();
    ui->language->show();
    ui->label_language->show();
    ui->label_txt->show();
    ui->method->show();
    ui->output->show();
    setGuiScrollLabel(Original);
}

void Caesar::outputTextFileToGui(std::string input_file)
{
    /// open input file
    // using wifstream instead of ifstream because 'æ', 'ø', 'å' uses 2 bytes and don't fit in a char
    wifstream fin;
    fin.open(input_file);

    if (fin.fail() || !fin.is_open())
    {
        writeFileNotFoundToGui();
        return;
    }

    // Set the locale to read UTF-8 encoded input and write UTF-8 encoded output
    // without this the code can't handle characters larger than 1 byte
    fin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    /// read file, and write encrypted/decrypted text to output
    ui->output->setPlainText("");
    ui->output->moveCursor(QTextCursor::End);
    wchar_t c_text[MAX_INPUT];
    wchar_t letter;
    int size = 0;
    fin.get(letter);
    while (!fin.eof())
    {
        c_text[size] = letter;
        fin.get(letter);
        size++;

        if (size == MAX_INPUT)
        {
            ui->output->insertPlainText(QString::fromWCharArray(c_text, size));
            ui->output->moveCursor(QTextCursor::End);
            size = 0;
        }
    }
    ui->output->insertPlainText(QString::fromWCharArray(c_text, size));

    // close file
    fin.close();
}

void Caesar::setGuiScrollLabel(OutputText label)
{
    ui->label_original_text->hide();
    ui->label_encrypted_text->hide();
    ui->label_decrypted_text->hide();

    switch (label) {
    case Original:
        ui->label_original_text->show();
        return;
    case Encrypted:
        ui->label_encrypted_text->show();
        return;
    case Decrypted:
        ui->label_decrypted_text->show();
        return;
    case None:  // Fallthrough
    default:
        break;
    }
}

void Caesar::on_pushButton_Caesar_clicked()
{
    closeMethodSelectorWindow();
    setupCaesarWindow();
    ui->method->setCurrentIndex(M_Caesar);
}

void Caesar::on_pushButton_Brutus_clicked()
{
    closeMethodSelectorWindow();
    setupBrutusWindow();
    ui->method->setCurrentIndex(M_Brutus);
}

void Caesar::on_method_currentIndexChanged(int index)
{
    switch (index) {
    case M_Caesar:
        setupCaesarWindow();
        break;
    case M_Brutus:
        setupBrutusWindow();
        break;
    default:
        qWarning() << "Invalid encryptin method input";
        return;
    }
}

void Caesar::on_pushButton_Encrypt_clicked()
{
    switch (m_encryption_method) {
    case M_Brutus:
        brutusEncrypt();
        break;
    case M_Caesar:  // Fallthrough
    default:
        caesarEncrypt();
        break;
    }
}


void Caesar::on_pushButton_Decrypt_clicked()
{
    switch (m_encryption_method) {
    case M_Brutus:
        brutusEncrypt(true);
        break;
    case M_Caesar:  // Fallthrough
    default:
        caesarEncrypt(true);
        break;
    }
}

void Caesar::on_pushButton_LoadFile_clicked()
{
    // read file name and path from GUI
    updateFileNameAndPath();

    std::string input_file = m_text_file_location + m_text_file_name + ".txt";

    setGuiScrollLabel(Original);
    outputTextFileToGui(input_file);
}

void Caesar::on_pushButton_Write2File_clicked()
{
    // read file name and path from GUI
    updateFileNameAndPath();
    
    /// open output file
    std::string std_output_file = m_text_file_location + m_text_file_name + ".txt";
    QString q_output_file = QString::fromStdString(std_output_file);

    /// open file
    wofstream fout;
    fout.open(std_output_file);
    if (fout.fail() || !fout.is_open())
    {
        qWarning() << "Error: opening output file failed";
        return;
    }

    // Set the locale to read UTF-8 encoded input and write UTF-8 encoded output
    // without this the code can't handle characters larger than 1 byte
    fout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    /// read input from GUI and write it to a text file
    QString input = ui->output->toPlainText();
    wchar_t letter;
    for (QChar ch : input)
    {
        letter = ch.unicode();
        fout << letter;
    }

    fout.close();

    setGuiScrollLabel(Original);
}

void Caesar::on_language_activated(int index)
{
    switch (index) {
    case British:
        m_language = British;
        return;
    case Norwegian:
        m_language = Norwegian;
        return;
    default:
        qWarning() << "Invalid language input";
        return;
    }
}

void Caesar::on_increment_valueChanged(int new_increment)
{
    if (!std::isfinite(new_increment))
    {
        qWarning() << "Invalid Increment input";
        return;
    }

    m_char_increment = new_increment;
}

void Caesar::on_generate_key_clicked()
{
    /// open output file
    std::string brutus_key_file = m_text_file_location + m_brutus_key_file_name + ".txt";

    /// open file
    ofstream fout;
    fout.open(brutus_key_file);
    if (fout.fail() || !fout.is_open())
    {
        qWarning() << "Error: opening output file failed";
        return;
    }


    /// generate random increment values for each letter and write them to the encryption key
    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0,28); // Guaranteed unbiased
    for (int c = 0; c < 1000; c++)
    {
        fout << uni(rng);
        fout << "\n";
    }

    fout.close();
}

void Caesar::on_pushButton_Exit_clicked()
{
    QApplication::quit();
}

void Caesar::initialize()
{
    on_method_currentIndexChanged(ui->method->currentIndex());
    on_language_activated(ui->language->currentIndex());
    on_increment_valueChanged(ui->increment->value());
    on_pushButton_LoadFile_clicked();
    on_generate_key_clicked();

    // hide everything not part of method selector
    ui->label_method->hide();
    ui->label_file_location->hide();
    ui->label_file_name->hide();
    ui->fileLocation->hide();
    ui->fileName->hide();
    ui->pushButton_LoadFile->hide();
    ui->pushButton_Write2File->hide();
    ui->pushButton_Decrypt->hide();
    ui->pushButton_Encrypt->hide();
    ui->language->hide();
    ui->label_language->hide();
    ui->label_txt->hide();
    ui->label_increment->hide();
    ui->increment->hide();
    ui->method->hide();
    ui->output->hide();
    ui->generate_key->hide();
    ui->label_min->hide();
    ui->label_max->hide();
    setGuiScrollLabel(None);
}
