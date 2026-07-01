#pragma once

#include <string>
#include <functional>

struct Student {
    std::string name;
    double avgGrade;

    bool operator<(const Student& other) const {
        return this->avgGrade < other.avgGrade;
    }

    bool operator>(const Student& other) const {
        return this->avgGrade > other.avgGrade;
    }

    bool operator==(const Student& other) const {
        return this->avgGrade == other.avgGrade && this->name == other.name;
    }
};

struct Teacher {
    std::string name;
    int salary;

    bool operator<(const Teacher& other) const {
        return this->salary < other.salary;
    }

    bool operator>(const Teacher& other) const {
        return this->salary > other.salary;
    }

    bool operator==(const Teacher& other) const {
        return this->salary == other.salary && this->name == other.name;
    }
};

template<class T, class T2>
struct Func {
    std::string name;
    std::function<T(T2)> function;
    int priority;

    bool operator<(const Func& other) const {
        return this->priority < other.priority;
    }

    bool operator>(const Func& other) const {
        return this->priority > other.priority;
    }

    bool operator==(const Func& other) const {
        return this->priority == other.priority;
    }
};