#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <string>
#include <cctype>

#include "QueueTestWidget.hpp"
#include "other/Exceptions.hpp"

static bool hasDigits(const std::string& s) {
    for (char c : s) if (::isdigit(c)) return true;
    return false;
}

static int intParser(const QString& str) {
    std::string s = str.trimmed().toStdString();
    if (s.empty()) throw InvalidArgument("Строка пуста!");
    size_t pos = 0;
    int val = std::stoi(s, &pos);
    if (pos != s.length()) throw InvalidArgument("Лишние символы!");
    return val;
}

static MyComplex<double> complexParser(const QString& str) {
    QStringList parts = str.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) throw InvalidArgument("Нужно два числа!");
    size_t pos_re = 0, pos_im = 0;
    double re = std::stod(parts[0].toStdString(), &pos_re);
    double im = std::stod(parts[1].toStdString(), &pos_im);
    return MyComplex<double>(re, im);
}

static Student studentParser(const QString& str) {
    QStringList parts = str.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) throw InvalidArgument("Формат: [Имя] [Оценка]");
    std::string name = parts[0].toStdString();
    if (hasDigits(name)) throw InvalidArgument("В имени цифры!");
    double grade = std::stod(parts[1].toStdString());
    if (grade < 0.0 || grade > 5.0) throw OutOfRange("Оценка 0-5!");
    return Student{name, grade};
}

static Teacher teacherParser(const QString& str) {
    QStringList parts = str.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 2) throw InvalidArgument("Формат: [Имя] [Зарплата]");
    std::string name = parts[0].toStdString();
    if (hasDigits(name)) throw InvalidArgument("В имени цифры!");
    int salary = std::stoi(parts[1].toStdString());
    if (salary < 0) throw OutOfRange("Зарплата < 0!");
    return Teacher{name, salary};
}

class Tab1 : public QWidget {
public:
    Tab1(QWidget* parent = nullptr) : QWidget(parent) {
        auto* mainLayout = new QVBoxLayout(this);
        auto* tabWidget = new QTabWidget(this);

        tabWidget->addTab(new QueueTestWidget<int>(intParser, this), "Integer (int)");
        tabWidget->addTab(new QueueTestWidget<MyComplex<double>>(complexParser, this), "Complex");
        tabWidget->addTab(new QueueTestWidget<Student>(studentParser, this), "Students");
        tabWidget->addTab(new QueueTestWidget<Teacher>(teacherParser, this), "Teachers");

        mainLayout->addWidget(tabWidget);
    }
};