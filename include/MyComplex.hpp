#pragma once

#include <iostream>

//https://en.cppreference.com/cpp/numeric/complex
template <typename T>
class MyComplex {
private:
    T re;
    T im;

public:
    MyComplex(T r = T(), T i = T()) : re(r), im(i) {}

    T real() const { return re; }
    T imag() const { return im; }

    void real(T r) { re = r; }
    void imag(T i) { im = i; }

    MyComplex& operator+=(const MyComplex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }

    MyComplex& operator-=(const MyComplex& other) {
        re -= other.re;
        im -= other.im;
        return *this;
    }

    MyComplex& operator*=(const MyComplex& other) {
        T new_re = (re * other.re) - (im * other.im);
        T new_im = (re * other.im) + (im * other.re);
        re = new_re;
        im = new_im;
        return *this;
    }

    MyComplex conj() const {
        return MyComplex(re, -im);
    }

    T abs() const {
        return (re * re + im * im) * (re * re + im * im);
    }
};

template <typename T>
MyComplex<T> operator+(MyComplex<T> lhs, const MyComplex<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
MyComplex<T> operator-(MyComplex<T> lhs, const MyComplex<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
MyComplex<T> operator*(MyComplex<T> lhs, const MyComplex<T>& rhs) {
    lhs *= rhs;
    return lhs;
}

template <typename T>
bool operator==(const MyComplex<T>& lhs, const MyComplex<T>& rhs) {
    return lhs.real() == rhs.real() && lhs.imag() == rhs.imag();
}

//NOT IN ORIGINAL SRD::COMPLEX, IT IS MINE
template <typename T>
bool operator<(const MyComplex<T>& lhs, const MyComplex<T>& rhs) {
    return lhs.abs() < rhs.abs();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const MyComplex<T>& c) {
    os << c.real() << (c.imag() >= 0 ? "+" : "") << c.imag() << "i";
    return os;
}