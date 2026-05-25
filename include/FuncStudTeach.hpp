#pragma once

#include <string.h>
#include <functional>

struct Student {
    std::string name;
    double avg_grade;

    bool operator<(const Student& other) const {
        return this->avg_grade < other.avg_grade;
    }
};

struct Teacher {
    std::string name;
    int salary;

    bool operator<(const Teacher& other) const {
        return this->salary < other.salary;
    }
};

template<class T, class T2>
struct Func {
    std::function<T(T2)> function;
    int priority;
    std::string name;

    bool operator<(const Task& other) const {
        return this->priority < other.priority;
    }
};