#include <gtest/gtest.h>
#include "include/PriorityQueue.hpp"
#include "include/sequence/sequences/DynamicArray.hpp"
#include <string>

struct TestPoint {
    int x, y;
    TestPoint() : x(0), y(0) {}
    TestPoint(int x, int y) : x(x), y(y) {}
    
    bool operator<(const TestPoint& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(const TestPoint& other) const {
        return x == other.x && y == other.y;
    }
};

TEST(PriorityQueueTest, DefaultConstructorInitializesEmptyQueue) {
    PriorityQueue<int> pq;
    EXPECT_TRUE(pq.Empty());
    EXPECT_EQ(pq.Size(), 0);
}

TEST(PriorityQueueTest, ConstructorFromConstContainerCopiesDataAndBuildsHeap) {
    int raw_data[] = {10, 30, 20, 5, 40};
    DynamicArray<int> array(raw_data, 5);

    PriorityQueue<int> pq(std::less<int>(), array);

    EXPECT_EQ(pq.Size(), 5);
    EXPECT_EQ(pq.Top(), 40); 
}

TEST(PriorityQueueTest, ConstructorFromRvalueContainerMovesData) {
    int raw_data[] = {7, 8, 9};
    DynamicArray<int> array(raw_data, 3);

    PriorityQueue<int> pq(std::less<int>(), std::move(array));

    EXPECT_EQ(pq.Size(), 3);
    EXPECT_EQ(pq.Top(), 9);
    EXPECT_EQ(array.GetSize(), 0); 
}


TEST(PriorityQueueTest, PushMaintainsPriorityOrder) {
    PriorityQueue<int> pq;

    pq.Push(10);
    EXPECT_EQ(pq.Top(), 10);

    pq.Push(20);
    EXPECT_EQ(pq.Top(), 20);

    pq.Push(15);
    EXPECT_EQ(pq.Top(), 20); 
    EXPECT_EQ(pq.Size(), 3);
}

TEST(PriorityQueueTest, PopRemovesHighestPriorityElement) {
    PriorityQueue<int> pq;
    
    int data[] = {5, 1, 9, 3};
    for (size_t i = 0; i < 4; ++i) {
        pq.Push(data[i]);
    }

    EXPECT_EQ(pq.Top(), 9);
    pq.pop();

    EXPECT_EQ(pq.Top(), 5);
    pq.pop();

    EXPECT_EQ(pq.Top(), 3);
    pq.pop();

    EXPECT_EQ(pq.Top(), 1);
    pq.pop();

    EXPECT_TRUE(pq.Empty());
}

TEST(PriorityQueueTest, EmplaceConstructsElementInPlace) {
    PriorityQueue<TestPoint> pq;

    pq.Emplace(1, 2);
    pq.Emplace(5, 10);
    pq.Emplace(3, 4);

    EXPECT_EQ(pq.Size(), 3);
    TestPoint expected_top(5, 10);
    EXPECT_EQ(pq.Top(), expected_top);
}

TEST(PriorityQueueTest, PushRangeInsertsMultipleElements) {
    PriorityQueue<int> pq;

    int range_elements[] = {14, 2, 88, 31, 0};
    pq.PushRange(range_elements);

    EXPECT_EQ(pq.Size(), 5);
    EXPECT_EQ(pq.Top(), 88);
}


TEST(PriorityQueueTest, CopyConstructorAndAssignmentOperator) {
    PriorityQueue<int> original;
    original.Push(100);
    original.Push(500);

    PriorityQueue<int> copy(original);
    EXPECT_EQ(copy.Size(), original.Size());
    EXPECT_EQ(copy.Top(), original.Top());

    PriorityQueue<int> assigned;
    assigned = original;
    EXPECT_EQ(assigned.Size(), original.Size());
    EXPECT_EQ(assigned.Top(), original.Top());

    copy.pop();
    EXPECT_NE(copy.Top(), original.Top());
}

TEST(PriorityQueueTest, MoveConstructorAndAssignmentOperator) {
    PriorityQueue<int> original;
    original.Push(42);

    PriorityQueue<int> moved(std::move(original));
    EXPECT_EQ(moved.Size(), 1);
    EXPECT_EQ(moved.Top(), 42);
    EXPECT_TRUE(original.Empty()); 

    PriorityQueue<int> moved_assigned;
    moved_assigned = std::move(moved);
    EXPECT_EQ(moved_assigned.Size(), 1);
    EXPECT_EQ(moved_assigned.Top(), 42);
    EXPECT_TRUE(moved.Empty());
}

TEST(PriorityQueueTest, GreaterComparatorChangesQueueToMinPriority) {
    PriorityQueue<int, std::greater<int>> min_pq;

    min_pq.Push(50);
    min_pq.Push(10);
    min_pq.Push(30);

    EXPECT_EQ(min_pq.Top(), 10);
    min_pq.pop();
    EXPECT_EQ(min_pq.Top(), 30);
}

TEST(PriorityQueueTest, SwapMethodExchangesTwoQueuesEntirely) {
    PriorityQueue<int> pq1;
    pq1.Push(1);
    pq1.Push(2);

    PriorityQueue<int> pq2;
    pq2.Push(99);

    pq1.swap(pq2);

    EXPECT_EQ(pq1.Size(), 1);
    EXPECT_EQ(pq1.Top(), 99);

    EXPECT_EQ(pq2.Size(), 2);
    EXPECT_EQ(pq2.Top(), 2);
}