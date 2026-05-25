#pragma once

#include <string>
#include <functional>
#include <ostream>

struct Student {
    std::string name;
    double avg_grade;

    bool operator<(const Student& other) const {
        return this->avg_grade < other.avg_grade;
    }

    // Добавляем оператор равенства для IsSubsequence
    bool operator==(const Student& other) const {
        return this->avg_grade == other.avg_grade && this->name == other.name;
    }

    Student operator+(const Student& other) const {
        return Student{
            this->name + "&" + other.name,
            (this->avg_grade + other.avg_grade) / 2.0
        };
    }
};

inline std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.name << "(" << s.avg_grade << ")";
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