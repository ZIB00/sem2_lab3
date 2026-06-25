#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QIntValidator>
#include <QEvent>
#include <cmath>

#include "../include/sequence/sequences/ArraySequence.hpp"
#include "other/Exceptions.hpp"

constexpr int MAX_NUM = 999999;

class Tab3 : public QWidget {
    QLineEdit* inp;
    QTextEdit* log;
    QPushButton* b;

    bool IsPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i <= sqrt(n); i++) if (n % i == 0) return false;
        return true;
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (obj == b && event->type() == QEvent::MouseButtonRelease) {
            log->clear();
            QString textInput = inp->text().trimmed();
            if (textInput.isEmpty()) { log->append("Ошибка: Поле пустое."); return true; }

            bool ok;
            qlonglong checkNum = textInput.toLongLong(&ok);
            if (!ok || checkNum > MAX_NUM) { log->append("Ошибка переполнения."); return true; }

            int num = static_cast<int>(checkNum);
            if (num <= 0) { log->append("Ошибка: Введите положительное число."); return true; }

            log->append(QString("Анализ числа: %1").arg(num));
            
            MutableArraySequence<int> divisors;
            for (int i = 1; i <= num; ++i) if (num % i == 0) divisors.Append(i);
            
            QString divStr = "Множители: ";
            for (size_t i = 0; i < divisors.GetLength(); ++i) divStr += QString::number(divisors[i]) + " ";
            log->append(divStr);
            log->append(IsPrime(num) ? "Простое" : "Составное");

            MutableArraySequence<int> primeFactors;
            int temp = num;
            while (temp % 2 == 0) { primeFactors.Append(2); temp /= 2; }
            for (int i = 3; i * i <= temp; i += 2) {
                while (temp % i == 0) { primeFactors.Append(i); temp /= i; }
            }
            if (temp > 2) primeFactors.Append(temp);

            QString pfStr = "Простые множители: ";
            for (size_t i = 0; i < primeFactors.GetLength(); ++i) pfStr += QString::number(primeFactors[i]) + " ";
            log->append(pfStr);

            if (num >= 2) {
                bool* isPrimeFlag = new bool[num + 1];
                for (int i = 0; i <= num; ++i) isPrimeFlag[i] = true;
                for (int p = 2; p * p <= num; p++) {
                    if (isPrimeFlag[p]) {
                        for (int i = p * p; i <= num; i += p) isPrimeFlag[i] = false;
                    }
                }
                QString sieveStr = "Решето: ";
                for (int p = 2; p <= num; p++) if (isPrimeFlag[p]) sieveStr += QString::number(p) + " ";
                log->append(sieveStr);
                delete[] isPrimeFlag;
            }
            return true;
        }
        return QWidget::eventFilter(obj, event);
    }

public:
    Tab3(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* lay = new QVBoxLayout(this);
        lay->addWidget(new QLabel(QString("Введите число (макс. %1):").arg(MAX_NUM)));

        inp = new QLineEdit("120");
        inp->setValidator(new QIntValidator(1, MAX_NUM, this));
        lay->addWidget(inp);

        b = new QPushButton("Вычислить"); b->installEventFilter(this);
        lay->addWidget(b);

        log = new QTextEdit; log->setReadOnly(true);
        lay->addWidget(log);
    }
};