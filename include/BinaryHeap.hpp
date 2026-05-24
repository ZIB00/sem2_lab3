#pragma once

#include "sequence/sequences/DynamicArray.hpp"
#include <functional>
#include <utility>

template< class T, class Compare = std::less<T> >
class BinaryHeap 
{
    private:
        DynamicArray<T> data;
        Compare comp;

        size_t Parent(size_t i) const     { return (i - 1) / 2; }
        size_t LeftChild(size_t i) const  { return 2 * i + 1; }
        size_t RightChild(size_t i) const { return 2 * i + 2; }

        void SiftUp(size_t index);
        void SiftDown(size_t index);

    public:
        BinaryHeap() : data(0), comp(Compare()) {}
        explicit BinaryHeap( const Compare& compare ) : data(0), comp(compare) {}
        BinaryHeap( const Compare& compare, const DynamicArray<T>& array );
        BinaryHeap( const Compare& compare, DynamicArray<T>&& array) ;
        BinaryHeap( const BinaryHeap< T, Compare >& other ) : data(other.data), comp(other.comp) {}
        BinaryHeap( BinaryHeap< T, Compare >&& other );

        BinaryHeap< T, Compare >& operator=( const BinaryHeap< T, Compare >& other );
        BinaryHeap< T, Compare >& operator=( BinaryHeap< T, Compare >&& other );

        size_t GetSize() const   { return data.GetSize(); }
        bool IsEmpty() const     { return data.GetSize() == 0; }
        const T& GetRoot() const { return data.Get(0); }

        void Insert(const T& item);
        void Insert(T&& item);
        void ExtractRoot();
        void Swap(BinaryHeap< T, Compare >& other);
};

#include "../template/BinaryHeap.tpp"