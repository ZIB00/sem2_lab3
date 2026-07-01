#pragma once

#include "sequence/Sequence/LinkedList.hpp"
#include <initializer_list>

template< class T >
class Stack 
{
    private:
        LinkedList<T> list;

    public:
        Stack()                            : list() {}
        Stack( const LinkedList<T>& list ) : list(list) {}
        Stack( LinkedList<T>&& list)       : list(std::move(list)) {}
        Stack( const Stack<T>& other )     : list(other.list) {}
        Stack( Stack<T>&& other );
        Stack(std::initializer_list<T> items) : list(items) {}

        Stack<T>& operator=( const Stack<T>& other );
        Stack<T>& operator=( Stack<T>&& other );

        size_t   GetSize() const { return list.GetLength(); }
        bool     IsEmpty() const { return list.GetLength() == 0; }
        const T& GetRoot() const { return list.Get(0); }

        void Push(const T& item) { list.Prepend(item); }
        void Push(T&& item) { list.Prepend(std::move(item)); }
        void Pop() { list.PopBack(); }
};

template<class T>
Stack<T>::Stack(Stack<T>&& other) 
    : list(std::move(other.list))
{
    other.list.Clear();
}

template<class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
    if( this != &other ) {
        this->list = other.list;
    }
    return *this;
}

template<class T>
Stack<T>& Stack<T>::operator=(Stack<T>&& other)
{
    if (this != &other) {
        this->list = std::move(other.list);
        other.list.Clear();
    }
    return *this;
}