#include "QueueControllerImpl.hpp"
#include "other/Exceptions.hpp"

QString ValueToString(const int& value) { return QString::number(value); }
QString ValueToString(const double& value) { return QString::number(value); }
QString ValueToString(const MyComplex<int>& value) { return QString::number(value.real()) + " + i" + QString::number(value.imag()); }
QString ValueToString(const QString& value) { return value; }
QString ValueToString(const FuncType& value) { 
    return "Имя: " + QString::fromStdString(value.name) + " Приоритет: " + QString::number(value.priority); 
}
QString ValueToString(const Student& value) {
    return QString::fromStdString(value.name) + ", балл: " + QString::number(value.avgGrade);
}
QString ValueToString(const Teacher& value) {
    return QString::fromStdString(value.name) + ", зарплата: " + QString::number(value.salary);
}


int CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<int>) {
    return input2.toInt();
}
double CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<double>) {
    QString fixed = input2;
    fixed.replace(",", ".");
    return fixed.toDouble();
}
MyComplex<int> CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<MyComplex<int>>) {
    return MyComplex<int>(input1.toDouble(), input2.toDouble());
}
QString CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<QString>) {
    return input1;
}
FuncType CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<FuncType>) {
    FuncType result;
    bool ok = true;
    result.priority = input2.toInt(&ok);
    if(!ok) throw InvalidArgument("Неккоректный ввод");

    if (secondCombo == "x + 69") {
        result.function = [](int x) { return x + 69; };
        result.name = "x + 69";
    } else {
        result.function = [](int x) { return 96; };
        result.name = "x = 96";
    }
    return result;
}
Student CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<Student>) {
    QString fixed = input2;
    fixed.replace(",", ".");
    Student s;
    s.name = input1.toStdString();
    s.avgGrade = fixed.toDouble();
    return s;
}
Teacher CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<Teacher>) {
    QString fixed = input2;
    fixed.replace(",", ".");
    Teacher t;
    t.name = input1.toStdString();
    t.salary = static_cast<int>(fixed.toDouble());
    return t;
}

std::function<int(const int&)> MapFunction(const QString& name, TypeTag<int>) {
    if (name == "x + 228") return [](const int& x) { return x + 228; };
    return [](const int& x) { return x * 2; };
}
std::function<double(const double&)> MapFunction(const QString& name, TypeTag<double>) {
    if (name == "x + 0.5") return [](const double& x) { return x + 0.5; };
    return [](const double& x) { return x * 1.5; };
}
std::function<MyComplex<int>(const MyComplex<int>&)> MapFunction(const QString& name, TypeTag<MyComplex<int>>) {
    if (name == "z + 228") return [](const MyComplex<int>& x) { return x + MyComplex<int>(228, 0); };
    return [](const MyComplex<int>& x) { return x * MyComplex<int>(2, 0); };
}
std::function<QString(const QString&)> MapFunction(const QString& name, TypeTag<QString>) {
    if (name == "x + zachet") return [](const QString& x) { return x + "zachet"; };
    return [](const QString& x) { return QString("Такого не бывает"); };
}
std::function<FuncType(const FuncType&)> MapFunction(const QString& name, TypeTag<FuncType>) {
    if (name == "zachet") return [](const FuncType& f) { return FuncType{f.name, f.function, f.priority + 1}; };
    return [](const FuncType& f) { return FuncType{f.name, f.function, f.priority - 1}; };
}
std::function<Student(const Student&)> MapFunction(const QString& name, TypeTag<Student>) {
    if (name == "Поставить зачёт") return [](const Student& s) { return Student{s.name, 4.0}; };
    return [](const Student& s) { return Student{s.name, 5.0}; };
}
std::function<Teacher(const Teacher&)> MapFunction(const QString& name, TypeTag<Teacher>) {
    if (name == "Премировать") return [](const Teacher& t) { return Teacher{t.name, t.salary + 10000}; };
    return [](const Teacher& t) { return Teacher{t.name, t.salary * 2}; };
}


std::function<bool(const int&)> WherePredicate(const QString& name, TypeTag<int>) {
    if (name == "x > 5") return [](const int& x) { return x > 5; };
    return [](const int& x) { return x % 2 == 0; };
}
std::function<bool(const double&)> WherePredicate(const QString& name, TypeTag<double>) {
    if (name == "x > 5") return [](const double& x) { return x > 5; };
    return [](const double& x) { return x < 0; };
}
std::function<bool(const MyComplex<int>&)> WherePredicate(const QString& name, TypeTag<MyComplex<int>>) {
    if (name == "z > 5") return [](const MyComplex<int>& x) { return x.abs() > 5; };
    return [](const MyComplex<int>& x) { return static_cast<long long>(x.abs()) % 2 == 0; };
}
std::function<bool(const QString&)> WherePredicate(const QString& name, TypeTag<QString>) {
    if (name == "len > 5") return [](const QString& x) { return x.length() > 5; };
    return [](const QString& x) { return x.length() < 5; };
}
std::function<bool(const FuncType&)> WherePredicate(const QString& name, TypeTag<FuncType>) {
    if (name == "zachet") return [](const FuncType& f) { return f.priority > 0; };
    return [](const FuncType& f) { return f.priority <= 0; };
}
std::function<bool(const Student&)> WherePredicate(const QString& name, TypeTag<Student>) {
    if (name == "Больше 3") return [](const Student& s) { return s.avgGrade > 3; };
    return [](const Student& s) { return s.avgGrade > 4; };
}
std::function<bool(const Teacher&)> WherePredicate(const QString& name, TypeTag<Teacher>) {
    if (name == "Больше 100 тыс.") return [](const Teacher& t) { return t.salary > 100000; };
    return [](const Teacher& t) { return t.salary > 200000; };
}


std::function<int(const int&, const int&)> ReduceFunction(const QString& name, TypeTag<int>) {
    if (name == "Сумма") return [](const int& acc, const int& x) { return acc + x; };
    return [](const int& acc, const int& x) { return x > acc ? x : acc; };
}
std::function<double(const double&, const double&)> ReduceFunction(const QString& name, TypeTag<double>) {
    if (name == "Сумма") return [](const double& acc, const double& x) { return acc + x; };
    return [](const double& acc, const double& x) { return (acc == 0 || x < acc) ? x : acc; };
}
std::function<MyComplex<int>(const MyComplex<int>&, const MyComplex<int>&)> ReduceFunction(const QString& name, TypeTag<MyComplex<int>>) {
    if (name == "Сумма") return [](const MyComplex<int>& acc, const MyComplex<int>& x) { return acc + x; };
    return [](const MyComplex<int>& acc, const MyComplex<int>& x) { return x.abs() > acc.abs() ? x : acc; };
}
std::function<QString(const QString&, const QString&)> ReduceFunction(const QString& name, TypeTag<QString>) {
    if (name == "Объединение") return [](const QString& acc, const QString& x) { return acc + x; };
    return [](const QString& acc, const QString& x) { return (acc.isEmpty() || x.length() < acc.length()) ? x : acc; };
}
std::function<FuncType(const FuncType&, const FuncType&)> ReduceFunction(const QString& name, TypeTag<FuncType>) {
    return [](const FuncType& acc, const FuncType& x) { return x.priority > acc.priority ? x : acc; };
}
std::function<Student(const Student&, const Student&)> ReduceFunction(const QString& name, TypeTag<Student>) {
    if (name == "Лучший балл") return [](const Student& acc, const Student& x) { return x.avgGrade > acc.avgGrade ? x : acc; };
    return [](const Student& acc, const Student& x) { return (acc.avgGrade == 0 || x.avgGrade < acc.avgGrade) ? x : acc; };
}
std::function<Teacher(const Teacher&, const Teacher&)> ReduceFunction(const QString& name, TypeTag<Teacher>) {
    if (name == "Наибольшая") return [](const Teacher& acc, const Teacher& x) { return x.salary > acc.salary ? x : acc; };
    return [](const Teacher& acc, const Teacher& x) { return Teacher{"Все", acc.salary + x.salary}; };
}