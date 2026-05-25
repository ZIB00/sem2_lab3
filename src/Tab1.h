#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <string>
#include <algorithm>

#include "QueueTestWidget.hpp"
#include "other/Exceptions.hpp"

class Tab1 : public QWidget
{
    Q_OBJECT

public:
    explicit Tab1(QWidget* parent = nullptr) : QWidget(parent)
    {
        auto* mainLayout = new QVBoxLayout(this);
        auto* tabWidget = new QTabWidget(this);

        // Лямбда для проверки наличия цифр в строке
        auto hasDigits = [](const std::string& s) {
            return std::any_of(s.begin(), s.end(), ::isdigit);
        };

        // 1. Вкладка для int
        auto intParser = [](const QString& str) -> int {
            std::string s = str.trimmed().toStdString();
            if (s.empty()) {
                throw InvalidArgument("Ошибка ввода: строка пуста!");
            }
            
            size_t pos = 0;
            try {
                int val = std::stoi(s, &pos);
                if (pos != s.length()) {
                    throw InvalidArgument("Ошибка ввода: обнаружены лишние символы после числа!");
                }
                return val;
            } catch (const InvalidArgument&) {
                throw InvalidArgument("Ошибка ввода: вместо числа введены буквы!");
            } catch (const OutOfRange&) {
                throw OutOfRange("Ошибка диапазона: число не помещается в тип int!");
            }
        };
        auto* intTab = new QueueTestWidget<int>(intParser, this);
        tabWidget->addTab(intTab, "Integer (int)");

        auto complexParser = [](const QString& str) -> MyComplex<double> {
            QStringList parts = str.split(' ', Qt::SkipEmptyParts);
            if (parts.size() != 2) {
                throw InvalidArgument("Ошибка ввода: введите ровно два числа через пробел (Re и Im)!");
            }

            std::string s_re = parts[0].toStdString();
            std::string s_im = parts[1].toStdString();
            
            size_t pos_re = 0, pos_im = 0;
            try {
                double re = std::stod(s_re, &pos_re);
                double im = std::stod(s_im, &pos_im);

                if (pos_re != s_re.length() || pos_im != s_im.length()) {
                    throw InvalidArgument("Ошибка ввода: некорректные символы в комплексном числе!");
                }
                return MyComplex<double>(re, im);
            } catch (const InvalidArgument&) {
                throw InvalidArgument("Ошибка ввода: комплексное число содержит недопустимые буквы!");
            } catch (const OutOfRange&) {
                throw OutOfRange("Ошибка диапазона: вещественное число слишком огромное!");
            }
        };
        QueueTestWidget<MyComplex<double>>* complexTab = new QueueTestWidget<MyComplex<double>>(complexParser, this);
        tabWidget->addTab(complexTab, "Complex (MyComplex)");

        // 3. Вкладка для Student
        auto studentParser = [hasDigits](const QString& str) -> Student {
            QStringList parts = str.split(' ', Qt::SkipEmptyParts);
            if (parts.size() != 2) {
                throw InvalidArgument("Ошибка ввода: формат должен быть [Имя] [Оценка] через пробел!");
            }

            std::string name = parts[0].toStdString();
            if (hasDigits(name)) {
                throw InvalidArgument("Ошибка валидации: в имени студента содержатся цифры!");
            }

            std::string s_grade = parts[1].toStdString();
            size_t pos = 0;
            try {
                double grade = std::stod(s_grade, &pos);
                if (pos != s_grade.length()) {
                    throw InvalidArgument("Ошибка ввода: некорректные символы в оценке студента!");
                }
                if (grade < 0.0 || grade > 5.0) {
                    throw OutOfRange("Ошибка диапазона: оценка студента должна быть строго от 0 до 5!");
                }
                return Student{name, grade};
            } catch (const std::invalid_argument&) {
                throw InvalidArgument("Ошибка ввода: оценка студента содержит буквы!");
            } catch (const std::out_of_range& e) {
                throw OutOfRange("Ошибка диапазона: значение оценки вызвало переполнение типа!");
            }
        };
        QueueTestWidget<Student>* studentTab = new QueueTestWidget<Student>(studentParser, this);
        tabWidget->addTab(studentTab, "Students");

        auto teacherParser = [hasDigits](const QString& str) -> Teacher {
            QStringList parts = str.split(' ', Qt::SkipEmptyParts);
            if (parts.size() != 2) {
                throw InvalidArgument("Ошибка ввода: формат должен быть [Имя] [Зарплата] через пробел!");
            }

            std::string name = parts[0].toStdString();
            if (hasDigits(name)) {
                throw InvalidArgument("Ошибка валидации: в имени преподавателя содержатся цифры!");
            }

            std::string s_salary = parts[1].toStdString();
            size_t pos = 0;
            try {
                int salary = std::stoi(s_salary, &pos);
                if (pos != s_salary.length()) {
                    throw InvalidArgument("Ошибка ввода: некорректные символы в зарплате!");
                }
                if (salary < 0) {
                    throw OutOfRange("Ошибка диапазона: зарплата преподавателя не может быть отрицательной!");
                }
                return Teacher{name, salary};
            } catch (const InvalidArgument&) {
                throw InvalidArgument("Ошибка ввода: зарплата преподавателя содержит буквы!");
            } catch (const OutOfRange&) {
                throw OutOfRange("Ошибка диапазона: значение зарплаты превышает лимиты типа int!");
            }
        };
        auto* teacherTab = new QueueTestWidget<Teacher>(teacherParser, this);
        tabWidget->addTab(teacherTab, "Teachers");

        mainLayout->addWidget(tabWidget);
    }
};