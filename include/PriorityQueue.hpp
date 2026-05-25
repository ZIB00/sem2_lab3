#pragma once

#include <initializer_list>
#include "BinaryHeap.hpp"
// #include <sequences>

//https://en.cppreference.com/cpp/container/priority_queue
template<
    class T,
    //class Container = MutableArraySequence<T>,
    class Compare = std::less<T>
> 
class PriorityQueue {
    protected:
        BinaryHeap<T, Compare> heap;

    public:
        PriorityQueue() : heap() {}
        explicit PriorityQueue( const Compare& compare ) : heap(compare) {}
        //explicit PriorityQueue( const Compare& compare = Compare(), const Container& cont = Container() );
        PriorityQueue( const Compare& compare, const DynamicArray<T>& cont ) : heap(compare, cont) {}
        PriorityQueue(const Compare& compare, DynamicArray<T>&& cont) : heap(compare, std::move(cont)) {}
        PriorityQueue(const PriorityQueue& other) : heap(other.heap) {}
        PriorityQueue(PriorityQueue&& other) : heap(std::move(other.heap)) {}
        PriorityQueue(std::initializer_list<T> initList, const Compare& compare = Compare()); //My Own
        ~PriorityQueue() = default; //Note, that if the elements are pointers, the pointed-to objects are not destroyed. 

        PriorityQueue& operator=(const PriorityQueue& other);
        PriorityQueue& operator=(PriorityQueue&& other);

        //Element access
        const T& Top() const { return heap.GetRoot(); }

        //Capacity
        bool Empty() const { return heap.IsEmpty(); }
        size_t Size() const { return heap.GetSize(); }

        //Modifiers
        void Push(const T& value) { heap.Insert(value); }
        void Push(T&& value)      { heap.Insert(std::move(value)); }

        template<class... Args>
        void Emplace(Args&&... args);

        template<class R>
        void PushRange(R&& rg);

        void pop() { heap.ExtractRoot(); }
        
        void swap(PriorityQueue& other) { heap.Swap(other.heap); }

        //Задания
        PriorityQueue<int, std::less<int>> Map(std::function<int(const T&)> func) const;
        PriorityQueue<T, Compare> Where(std::function<bool(const T&)> predicate) const;

        template<class T2>
        T2 Reduce(std::function<T2(const T2&, const T&)> func) const;

        PriorityQueue<T, Compare> Concat(const PriorityQueue& other) const;
        PriorityQueue<T, Compare> GetSubsequence(size_t startIndex, size_t endIndex) const;
        bool IsSubsequence(const PriorityQueue& subQueue) const;
        void Append(const PriorityQueue& other);
        std::pair<PriorityQueue<T, Compare>, PriorityQueue<T, Compare>> Split(std::function<bool(const T&)> predicate) const;
};

#include "../template/PriorityQueue.tpp"