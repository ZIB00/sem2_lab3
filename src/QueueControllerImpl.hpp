#pragma once

#include "QueueController.hpp"
#include "PriorityQueue.hpp"
#include "MyComplex.hpp"
#include "FuncStudTeach.hpp"
#include <QString>
#include <functional>

typedef Func<int, int> FuncType;

template<typename T>
struct TypeTag {};

QString ValueToString(const int& value);
QString ValueToString(const double& value);
QString ValueToString(const MyComplex<int>& value);
QString ValueToString(const QString& value);
QString ValueToString(const FuncType& value);
QString ValueToString(const Student& value);
QString ValueToString(const Teacher& value);

int CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<int>);
double CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<double>);
MyComplex<int> CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<MyComplex<int>>);
QString CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<QString>);
FuncType CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<FuncType>);
Student CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<Student>);
Teacher CreateValue(const QString& input1, const QString& input2, const QString& secondCombo, TypeTag<Teacher>);

std::function<int(const int&)> MapFunction(const QString& name, TypeTag<int>);
std::function<double(const double&)> MapFunction(const QString& name, TypeTag<double>);
std::function<MyComplex<int>(const MyComplex<int>&)> MapFunction(const QString& name, TypeTag<MyComplex<int>>);
std::function<QString(const QString&)> MapFunction(const QString& name, TypeTag<QString>);
std::function<FuncType(const FuncType&)> MapFunction(const QString& name, TypeTag<FuncType>);
std::function<Student(const Student&)> MapFunction(const QString& name, TypeTag<Student>);
std::function<Teacher(const Teacher&)> MapFunction(const QString& name, TypeTag<Teacher>);

std::function<bool(const int&)> WherePredicate(const QString& name, TypeTag<int>);
std::function<bool(const double&)> WherePredicate(const QString& name, TypeTag<double>);
std::function<bool(const MyComplex<int>&)> WherePredicate(const QString& name, TypeTag<MyComplex<int>>);
std::function<bool(const QString&)> WherePredicate(const QString& name, TypeTag<QString>);
std::function<bool(const FuncType&)> WherePredicate(const QString& name, TypeTag<FuncType>);
std::function<bool(const Student&)> WherePredicate(const QString& name, TypeTag<Student>);
std::function<bool(const Teacher&)> WherePredicate(const QString& name, TypeTag<Teacher>);

std::function<int(const int&, const int&)> ReduceFunction(const QString& name, TypeTag<int>);
std::function<double(const double&, const double&)> ReduceFunction(const QString& name, TypeTag<double>);
std::function<MyComplex<int>(const MyComplex<int>&, const MyComplex<int>&)> ReduceFunction(const QString& name, TypeTag<MyComplex<int>>);
std::function<QString(const QString&, const QString&)> ReduceFunction(const QString& name, TypeTag<QString>);
std::function<FuncType(const FuncType&, const FuncType&)> ReduceFunction(const QString& name, TypeTag<FuncType>);
std::function<Student(const Student&, const Student&)> ReduceFunction(const QString& name, TypeTag<Student>);
std::function<Teacher(const Teacher&, const Teacher&)> ReduceFunction(const QString& name, TypeTag<Teacher>);

template<typename T, typename Compare = std::less<T>>
class QueueController : public IQueueController {
private:
    PriorityQueue<T, Compare> queue;

public:
    QString Top() override {
        if (queue.Empty()) return "Очередь пуста";
        return "Top: " + ValueToString(queue.Top());
    }
    QString IsEmpty() override { return queue.Empty() ? "Да" : "Нет"; }
    QString Size() override { return QString::number(queue.Size()); }

    void Push(const QString& input1, const QString& input2, const QString& secondCombo) override {
        T value = CreateValue(input1, input2, secondCombo, TypeTag<T>{});
        queue.Push(value);
    }

    void Pop() override { if (!queue.Empty()) queue.pop(); }

    QString Map(const QString& funcName) override {
        std::function<T(const T&)> func = MapFunction(funcName, TypeTag<T>{});
        PriorityQueue<T, Compare> result = queue.template Map<T>(func);
        queue = result;
        return "Map выполнен. Размер очереди: " + QString::number(queue.Size());
    }

    QString Where(const QString& predicateName) override {
        std::function<bool(const T&)> predicate = WherePredicate(predicateName, TypeTag<T>{});
        PriorityQueue<T, Compare> result = queue.Where(predicate);
        return "Where выполнен. Найдено элементов: " + QString::number(result.Size());
    }

    QString Reduce(const QString& funcName) override {
        if (queue.Empty()) return "Очередь пуста";
        std::function<T(const T&, const T&)> func = ReduceFunction(funcName, TypeTag<T>{});
        T result = queue.template Reduce<T>(func);
        return "Результат Reduce: " + ValueToString(result);
    }

    QString GetSubsequence(const QString& startIndex, const QString& endIndex) override {
        try {
            size_t start = static_cast<size_t>(startIndex.toULongLong());
            size_t end = static_cast<size_t>(endIndex.toULongLong());
            PriorityQueue<T, Compare> result = queue.GetSubsequence(start, end);
            return "GetSubsequence выполнен. Размер результата: " + QString::number(result.Size());
        } catch (const std::exception& e) {
            return QString("Ошибка: ") + e.what();
        }
    }

    QString Split(const QString& predicateName) override {
        std::function<bool(const T&)> predicate = WherePredicate(predicateName, TypeTag<T>{});
        std::pair<PriorityQueue<T, Compare>, PriorityQueue<T, Compare>> result = queue.Split(predicate);
        return "Split выполнен. Первая часть: " + QString::number(result.first.Size()) + ", вторая часть: " + QString::number(result.second.Size());
    }

    QString ToString() override {
        if (queue.Empty()) return "Очередь пуста";

        PriorityQueue<T, Compare> tempQueue = queue;
        QString result = "";
        while (!tempQueue.Empty()) {
            result += ValueToString(tempQueue.Top()) + "\n";
            tempQueue.pop();
        }
        return result;
    }
};