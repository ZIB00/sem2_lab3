#pragma once

#include <string>
#include <functional>
#include <ostream>

struct Student {
    std::string name;
    double avgGrade;

    bool operator<(const Student& other) const {
        return this->avgGrade < other.avgGrade;
    }

    bool operator==(const Student& other) const {
        return this->avgGrade == other.avgGrade && this->name == other.name;
    }

    Student operator+(const Student& other) const {
        return Student{
            this->name + "&" + other.name,
            (this->avgGrade + other.avgGrade) / 2.0
        };
    }
};

inline std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.name << "(" << s.avgGrade << ")";
    return os;
}

struct Teacher {
    std::string name;
    int salary;

    bool operator<(const Teacher& other) const {
        return this->salary < other.salary;
    }

    bool operator==(const Teacher& other) const {
        return this->salary == other.salary && this->name == other.name;
    }

    Teacher operator+(const Teacher& other) const {
        return Teacher{
            this->name + "+" + other.name,
            this->salary + other.salary
        };
    }
};

inline std::ostream& operator<<(std::ostream& os, const Teacher& t) {
    os << t.name << "(" << t.salary << ")";
    return os;
}

template<class T, class T2>
struct Func {
    std::function<T(T2)> function;
    int priority;
    std::string name;

    bool operator<(const Func& other) const {
        return this->priority < other.priority;
    }
};