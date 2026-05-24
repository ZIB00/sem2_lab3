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

template<class T, class Compare>
template<class... Args>
void PriorityQueue<T, Compare>::Emplace(Args&&... args) {
    heap.Insert(T(std::forward<Args>(args)...));
}

template<class T, class Compare>
template<class R>
void PriorityQueue<T, Compare>::PushRange(R&& rg) {
    for (auto&& el : rg) {
        heap.Insert(std::forward<decltype(el)>(el));
    }
}