#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

constexpr int MAX_NUMBER = 999999;
constexpr int MAX_NUMBER_LEN = 6;

class PrimeTab : public QWidget {
    Q_OBJECT

private:
    QTextEdit *log;
    QLineEdit *numInput;
    QPushButton *start;

    QTextEdit *output;

public:
    PrimeTab(QTextEdit *log, QWidget *parent = nullptr);

    int Parse();

    bool AllFactors(int num);
    void IsPrime(int num);
    void PrimeFactors(int num);
    void AllPrimes(int num);
    void onPrime();
};