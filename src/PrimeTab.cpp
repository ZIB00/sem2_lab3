#include <QRegularExpressionValidator>
#include <qobject.h>
#include <QCoreApplication>

#include <cmath>

#include "PrimeTab.hpp"
#include "Sequence/ArraySequence.hpp"

namespace {
    bool isPrimeNumber(int num) {
        if (num <= 1) return false;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }
}

PrimeTab::PrimeTab(QTextEdit *log, QWidget *parent) : QWidget(parent), log(log) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout *topLayout = new QHBoxLayout();
    numInput = new QLineEdit(this);
    numInput->setText("120");
    numInput->setPlaceholderText(QString("Введите целое неотрицательное число до %1 не включительно").arg(MAX_NUMBER));
    numInput->setMaxLength(MAX_NUMBER_LEN);
    start = new QPushButton("Начать", this);

    QRegularExpression val("^[0-9]*$");
    QValidator *validator = new QRegularExpressionValidator(val, this);
    numInput->setValidator(validator);
    
    topLayout->addWidget(numInput, 1);
    topLayout->addWidget(start, 0);
    
    output = new QTextEdit(this);
    output->setReadOnly(true);
    
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(output);

    connect(start, &QPushButton::clicked, this, &PrimeTab::onPrime);
}

int PrimeTab::Parse()
{
    QString input = numInput->text();

    if (input.isEmpty()) {
        log->append("Поле ввода пустое");
        return -1;
    }

    bool ok;
    int val = input.toInt(&ok);
    if (!ok) {
        log->append("Ошибка: " + input + " слишком большое число");
        return -1;
    }

    log->append(QString("Получено число: %1").arg(val));
    return val;
}

bool PrimeTab::AllFactors(int num)
{
    if (num <= 0) return false;
    
    QList<int> smallFactors;
    QList<int> largeFactors;
    
    int n = std::sqrt(num); 
    
    for(int i = 1; i <= n; ++i) {
        if(num % i == 0) {
            smallFactors.append(i);
            if (i != num / i) {
                largeFactors.prepend(num / i);
            }
        }
    }
    
    QStringList result;
    for (int i : smallFactors) result << QString::number(i);
    for (int i : largeFactors) result << QString::number(i);
    
    output->append("Множители числа: ");
    output->append(result.join(", "));
    output->append(" ");
    
    return (smallFactors.size() + largeFactors.size() == 2);
}

void PrimeTab::IsPrime(int num)
{
    if( isPrimeNumber(num) ) {
        output->append("Число простое");
    } else {
        output->append("Число не простое");
    }
    output->append(" ");
}

void PrimeTab::PrimeFactors(int num)
{   
    if (num <= 0) return;
    
    QStringList smallFactors;
    QStringList largeFactors;
    
    int n = std::sqrt(num); 
    
    for(int i = 1; i <= n; ++i) {
        if(num % i == 0) {
            if( isPrimeNumber(i) ) smallFactors.append(QString::number(i));
            if (i != num / i) {
                if( isPrimeNumber(num / i) ) largeFactors.prepend(QString::number(num / i));
            }
        }
    }
    
    QStringList result;
    result << smallFactors;
    result << largeFactors;
    
    output->append("Простые множители числа: ");
    output->append(result.join(", "));
    output->append(" ");
}

void PrimeTab::AllPrimes(int num)
{
    if (num < 2) {
        output->append(QString("Нет простых чисел до %1").arg(num));
        return;
    }

    MutableArraySequence<bool> isPrime(num + 1);
    isPrime.Set(0, false);
    isPrime.Set(1, false);

    for (int i = 2; i <= num; ++i) {
        isPrime.Set(i, true);
    }

    for (int p = 2; p * p <= num; ++p) {
        if (isPrime.Get(p)) {
            for (int i = p * p; i <= num; i += p) {
                isPrime.Set(i, false);
            }
        }
    }

    QStringList result;
    for (int i = 2; i <= num; ++i) {
        if (isPrime.Get(i)) {
            result << QString::number(i);
        }
    }

    output->append(QString("Простые числа до %1: ").arg(num));
    output->append(result.join(", "));
    output->append(" ");
}

void PrimeTab::onPrime()
{
    start->setEnabled(false);
    QCoreApplication::processEvents(); 

    output->clear();
    int num = Parse();
    
    if( num <= 0 ) {
        return; 
    }

    AllFactors(num);
    IsPrime(num);
    PrimeFactors(num);
    AllPrimes(num); 

    start->setEnabled(true);
}