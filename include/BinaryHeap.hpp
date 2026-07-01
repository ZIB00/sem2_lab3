#pragma once

#include "sequence/Sequence/DynamicArray.hpp"
#include <functional>

template< class T, class Compare = std::less<T> >
class BinaryHeap 
{
    private:
        DynamicArray<T> array;
        Compare compare;

        size_t Parent(size_t i) const     { return (i - 1) / 2; }
        size_t LeftChild(size_t i) const  { return 2 * i + 1; }
        size_t RightChild(size_t i) const { return 2 * i + 2; }

        void Swap(size_t index1, size_t index2);

        void SiftUp(size_t index);
        void SiftDown(size_t index);

    public:
        BinaryHeap()                                               : array(0), compare(Compare()) {}
        BinaryHeap( const Compare& comp )                          : array(0), compare(comp) {}
        BinaryHeap( const Compare& comp, const DynamicArray<T>& array );
        BinaryHeap( const Compare& comp, DynamicArray<T>&& array);
        BinaryHeap( const BinaryHeap< T, Compare >& other )        : array(other.array), compare(other.compare) {}
        BinaryHeap( BinaryHeap< T, Compare >&& other );

        BinaryHeap< T, Compare >& operator=( const BinaryHeap< T, Compare >& other );
        BinaryHeap< T, Compare >& operator=( BinaryHeap< T, Compare >&& other );

        size_t   GetSize() const { return array.GetSize(); }
        bool     IsEmpty() const { return array.GetSize() == 0; }
        const T& GetRoot() const { return array.Get(0); }

        void Push(const T& item);
        void Push(T&& item);
        void Pop();
};

template< class T, class Compare >
void BinaryHeap<T, Compare>::Swap(size_t index1, size_t index2)
{   
    T temp = array.Get(index1);
    array.Set(index1, array.Get(index2));
    array.Set(index2, temp);
}

template<class T, class Compare>
void BinaryHeap<T, Compare>::SiftUp(size_t index)
{
    while (index > 0) {
        size_t parent = Parent(index);
        
        if (compare(array.Get(parent), array.Get(index))) {
            Swap(parent, index);
            
            index = parent;
        } else {
            break; 
        }
    }
}

template<class T, class Compare>
void BinaryHeap<T, Compare>::SiftDown(size_t index)
{
    size_t length = array.GetSize();
    
    while (LeftChild(index) < length) {
        size_t lChild = LeftChild(index);
        size_t rChild = RightChild(index);
        size_t better = index;

        if (compare(array.Get(better), array.Get(lChild))) {
            better = lChild;
        }
        
        if (rChild < length && compare(array.Get(better), array.Get(rChild))) {
            better = rChild;
        }

        if (better == index) {
            break;
        }

        Swap(index, better);
        index = better;
    }
}

template<class T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap(const Compare& comp, const DynamicArray<T>& seq)
    : array(), compare(comp)
{
    size_t size = seq.GetSize();
    for( size_t i = 0; i < size; ++i ) {
        Push(seq.Get(i));
    }
}

template<class T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap(const Compare& comp, DynamicArray<T>&& seq)
    : array(), compare(comp)
{
    DynamicArray<T> tempSeq = std::move(seq);
    seq.Resize(0);

    size_t size = tempSeq.GetSize();
    for( size_t i = 0; i < size; ++i ) {
        Push(tempSeq.Get(i));
    }
}

template<class T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap(BinaryHeap<T, Compare>&& other) 
    : array(std::move(other.array)), compare(std::move(other.compare)) 
{
    other.array.Resize(0);
}

template<class T, class Compare>
BinaryHeap< T, Compare >& BinaryHeap< T, Compare >::operator=(const BinaryHeap< T, Compare >& other)
{
    if( this != &other ) {
        this->compare = other.compare;
        this->array.Resize(0);
        size_t size = other.array.GetSize();
        for( size_t i = 0; i < size; ++i ) {
            Push(other.array.Get(i));
        }
    }
    return *this;
}

template<class T, class Compare>
BinaryHeap<T, Compare>& BinaryHeap<T, Compare>::operator=(BinaryHeap<T, Compare>&& other)
{
    if (this != &other) {
        this->array = std::move(other.array);
        this->compare = std::move(other.compare);

        other.array.Resize(0);
    }
    return *this;
}

template<class T, class Compare>
void BinaryHeap< T, Compare >::Push(const T& item) 
{
    size_t size = array.GetSize();
    array.Resize(size + 1);
    array.Set(size, item);
    SiftUp(size);
}

template<class T, class Compare>
void BinaryHeap< T, Compare >::Push(T&& item) 
{
    size_t size = array.GetSize();
    array.Resize(size + 1);
    array.Set(size, std::move(item));
    SiftUp(size);
}

template<class T, class Compare>
void BinaryHeap< T, Compare >::Pop() 
{
    size_t size = array.GetSize();
    if (size == 0) return;

    if (size > 1) {
        array.Set(0, array.Get(size - 1));
    }
    array.Resize(size - 1);

    if (array.GetSize() > 0) {
        SiftDown(0);
    }
}