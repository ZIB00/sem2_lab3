#pragma once

#include <initializer_list>
#include <string>
#include "BinaryHeap.hpp"
#include "sequence/other/Exceptions.hpp"

//https://en.cppreference.com/cpp/container/priority_queue
template<
    class T,
    class Compare = std::less<T>
> 
class PriorityQueue {
    protected:
        BinaryHeap<T, Compare> heap;

    public:
        PriorityQueue() : heap() {}
        PriorityQueue( const Compare& compare )                             : heap(compare) {}
        PriorityQueue( const Compare& compare, const DynamicArray<T>& seq ) : heap(compare, seq) {}
        PriorityQueue( const Compare& compare, DynamicArray<T>&& seq )      : heap(compare, std::move(seq)) {}
        PriorityQueue( const PriorityQueue& other )                         : heap(other.heap) {}
        PriorityQueue( PriorityQueue&& other )                              : heap(std::move(other.heap)) {}
        PriorityQueue( std::initializer_list<T> initList, const Compare& compare = Compare() ); //My Own
        ~PriorityQueue() = default; 

        PriorityQueue& operator=(const PriorityQueue& other);
        PriorityQueue& operator=(PriorityQueue&& other);

        //Element access
        const T& Top() const { return heap.GetRoot(); }

        //Capacity
        bool   Empty() const { return heap.IsEmpty(); }
        size_t Size()  const { return heap.GetSize(); }

        //Modifiers
        void Push(const T& value) { heap.Push(value); }
        void Push(T&& value)      { heap.Push(std::move(value)); }

        void pop() { heap.Pop(); }

        //Задания
        template< class T2 >
        PriorityQueue<T2, std::less<T2>> Map(std::function<T2(const T&)> func) const;
        PriorityQueue<T, Compare> Where(std::function<bool(const T&)> predicate) const;

        template<class T2>
        T2 Reduce(std::function<T2(const T2&, const T&)> func) const;

        PriorityQueue<T, Compare> Concat(const PriorityQueue& other) const;
        PriorityQueue<T, Compare> GetSubsequence(size_t startIndex, size_t endIndex) const;
        bool IsSubsequence(const PriorityQueue& subQueue) const;
        void Append(const PriorityQueue& other);
        std::pair<PriorityQueue<T, Compare>, PriorityQueue<T, Compare>> Split(std::function<bool(const T&)> predicate) const;
};

template< class T, class Compare >
PriorityQueue< T, Compare >::PriorityQueue( std::initializer_list<T> init_list, const Compare& compare )
    : heap(compare)
{
    for (const T& item : init_list) {
        this->Push(item);
    }
}

template<class T, class Compare>
PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(const PriorityQueue<T, Compare>& other) {
    if (this != &other) {
        this->heap = other.heap;
    }
    return *this;
}

template<class T, class Compare>
PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(PriorityQueue<T, Compare>&& other) {
    if (this != &other) {
        this->heap = std::move(other.heap);
    }
    return *this;
}

template <typename T, typename Compare>
template<class T2>
PriorityQueue<T2, std::less<T2>> PriorityQueue<T, Compare>::Map(std::function<T2(const T&)> func) const {
    PriorityQueue<T2, std::less<T2>> result;
    PriorityQueue<T, Compare> temp = *this;
    
    while (!temp.Empty()) {
        result.Push(func(temp.Top()));
        temp.pop();
    }
    return result;
}

template<class T, class Compare>
PriorityQueue<T, Compare> PriorityQueue<T, Compare>::Where(std::function<bool(const T&)> predicate) const {
    PriorityQueue<T, Compare> result;
    PriorityQueue<T, Compare> temp(*this);
    while (!temp.Empty()) {
        if (predicate(temp.Top())) {
            result.Push(temp.Top());
        }
        temp.pop();
    }
    return result;
}

template<class T, class Compare>
template<class T2>
T2 PriorityQueue<T, Compare>::Reduce(std::function<T2(const T2&, const T&)> func) const {
    T2 acc{};
    PriorityQueue<T, Compare> temp(*this);
    while (!temp.Empty()) {
        acc = func(acc, temp.Top());
        temp.pop();
    }
    return acc;
}

template<class T, class Compare>
PriorityQueue<T, Compare> PriorityQueue<T, Compare>::Concat(const PriorityQueue& other) const {
    PriorityQueue<T, Compare> result(*this);
    PriorityQueue<T, Compare> temp(other);
    while (!temp.Empty()) {
        result.Push(temp.Top());
        temp.pop();
    }
    return result;
}

template<class T, class Compare>
PriorityQueue<T, Compare> PriorityQueue<T, Compare>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    PriorityQueue<T, Compare> result;
    
    if (startIndex > endIndex) throw InvalidArgument( std::to_string(startIndex) + " больше, чем " + std::to_string(endIndex) );
    if (startIndex >= heap.GetSize()) throw InvalidArgument( std::to_string(startIndex) + " больше, чем " + std::to_string(heap.GetSize()) );
    

    PriorityQueue<T, Compare> temp(*this);
    size_t currentIdx = 0;

    while (!temp.Empty() && currentIdx <= endIndex) {
        if (currentIdx >= startIndex) {
            result.Push(temp.Top());
        }
        temp.pop();
        currentIdx++;
    }

    return result;
}

template<class T, class Compare>
bool PriorityQueue<T, Compare>::IsSubsequence(const PriorityQueue& subQueue) const {
    if (Size() < subQueue.Size()) return false;

    PriorityQueue<T, Compare> subTemp(subQueue);
    PriorityQueue<T, Compare> mainTemp(*this);

    while (!subTemp.Empty() && !mainTemp.Empty()) {
        if (mainTemp.Top() == subTemp.Top()) {
            subTemp.pop(); 
        }
        mainTemp.pop(); 
    }

    return subTemp.Empty();
}

template<class T, class Compare>
void PriorityQueue<T, Compare>::Append(const PriorityQueue& other) {
    PriorityQueue<T, Compare> temp(other);
    while (!temp.Empty()) {
        Push(temp.Top());
        temp.pop();
    }
}

template<class T, class Compare>
std::pair<PriorityQueue<T, Compare>, PriorityQueue<T, Compare>> 
PriorityQueue<T, Compare>::Split(std::function<bool(const T&)> predicate) const {
    PriorityQueue<T, Compare> trueQueue;
    PriorityQueue<T, Compare> falseQueue;
    PriorityQueue<T, Compare> temp(*this);
    while (!temp.Empty()) {
        if (predicate(temp.Top())) {
            trueQueue.Push(temp.Top());
        } else {
            falseQueue.Push(temp.Top());
        }
        temp.pop();
    }
    return std::make_pair(trueQueue, falseQueue);
}