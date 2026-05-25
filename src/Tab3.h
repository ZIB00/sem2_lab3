#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QIntValidator>
#include <cmath>
#include <limits> 

#include "../include/sequence/sequences/ArraySequence.hpp"

constexpr int MAX_NUM = 999999;

class Tab3 : public QWidget
{
    Q_OBJECT

    QLineEdit* inp;
    QTextEdit* log;

    bool IsPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

public:
    explicit Tab3(QWidget* parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout* lay = new QVBoxLayout(this);
        QString line = QString("Введите целое число для анализа (макс. %1):").arg(MAX_NUM);
        lay->addWidget(new QLabel(line));

        inp = new QLineEdit;
        inp->setText("120");
        
        QIntValidator* validator = new QIntValidator(1, MAX_NUM, this);
        inp->setValidator(validator);
        
        lay->addWidget(inp);

        QPushButton* b = new QPushButton("Вычислить");
        lay->addWidget(b);

        log = new QTextEdit; 
        log->setReadOnly(true);
        lay->addWidget(log);

        connect(b, &QPushButton::clicked, this, [this] {
            log->clear();
            
            QString textInput = inp->text().trimmed();
            
            if (textInput.isEmpty()) {
                log->append("Ошибка: Поле ввода пустое.");
                return;
            }

            bool ok;
            qlonglong checkNum = textInput.toLongLong(&ok);
            
            if (!ok || checkNum > std::numeric_limits<int>::max()) {
                log->append("<font color='red'><b>Ошибка переполнения:</b> Введенное число слишком велико для обработки.</font>");
                log->append(QString("Максимально допустимое значение: %1").arg(MAX_NUM));
                return;
            }

            int num = static_cast<int>(checkNum);
            
            if (num <= 0) {
                log->append("Ошибка: Введите корректное положительное число.");
                return;
            }

            log->append(QString("<b>Анализ числа: %1</b>\n").arg(num));

            log->append("<b>A-3.1. Все множители (делители):</b>");
            MutableArraySequence<int> divisors;
            for (int i = 1; i <= num; ++i) {
                if (num % i == 0) {
                    divisors.Append(i);
                }
            }
            
            QString divStr = "";
            for (size_t i = 0; i < divisors.GetLength(); ++i) {
                divStr += QString("%1 ").arg(divisors[i]);
            }
            log->append(divStr + "\n");

            log->append("<b>A-3.2. Проверка на простоту:</b>");
            if (IsPrime(num)) {
                log->append(QString("Число %1 — простое.\n").arg(num));
            } else {
                log->append(QString("Число %1 — составное.\n").arg(num));
            }

            log->append("<b>A-3.3. Разложение на простые множители:</b>");
            MutableArraySequence<int> primeFactors;
            int temp = num;
            
            while (temp % 2 == 0) {
                primeFactors.Append(2);
                temp /= 2;
            }
            for (int i = 3; i * i <= temp; i += 2) {
                while (temp % i == 0) {
                    primeFactors.Append(i);
                    temp /= i;
                }
            }
            if (temp > 2) {
                primeFactors.Append(temp);
            }

            if (num == 1) {
                log->append("У числа 1 нет простых множителей.\n");
            } else {
                QString pfStr = "";
                for (size_t i = 0; i < primeFactors.GetLength(); ++i) {
                    pfStr += QString("%1 ").arg(primeFactors[i]);
                }
                log->append(pfStr + "\n");
            }

            log->append(QString("<b>A-3.4. Решето Эратосфена (все простые до %1):</b>").arg(num));
            if (num < 2) {
                log->append("Нет простых чисел в этом диапазоне.");
            } else {
                bool* isPrimeFlag = nullptr;
                try {
                    isPrimeFlag = new bool[num + 1];
                } catch (const std::bad_alloc&) {
                    log->append("<font color='orange'>Не удалось выполнить Решето Эратосфена: недостаточно оперативной памяти для создания таблицы флагов.</font>");
                    return;
                }

                for (int i = 0; i <= num; ++i) isPrimeFlag[i] = true;

                for (int p = 2; p * p <= num; p++) {
                    if (isPrimeFlag[p]) {
                        for (int i = p * p; i <= num; i += p)
                            isPrimeFlag[i] = false;
                    }
                }

                MutableArraySequence<int> primes;
                for (int p = 2; p <= num; p++) {
                    if (isPrimeFlag[p]) {
                        primes.Append(p);
                    }
                }

                delete[] isPrimeFlag;

                QString sieveStr = "";
                for (size_t i = 0; i < primes.GetLength(); ++i) {
                    sieveStr += QString("%1 ").arg(primes[i]);
                }
                log->append(sieveStr);
            }
        });
    }
};