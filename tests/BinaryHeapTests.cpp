#include <gtest/gtest.h>
#include "include/BinaryHeap.hpp"
#include <string>

TEST(BinaryHeapTest, DefaultConstructorInitializesEmptyHeap) {
    BinaryHeap<int> heap;
    EXPECT_TRUE(heap.IsEmpty());
    EXPECT_EQ(heap.GetSize(), 0);
}


TEST(BinaryHeapTest, InsertMaintainsMaxHeapProperty) {
    BinaryHeap<int> heap;
    
    heap.Insert(5);
    EXPECT_EQ(heap.GetRoot(), 5);
    EXPECT_EQ(heap.GetSize(), 1);

    heap.Insert(3);
    EXPECT_EQ(heap.GetRoot(), 5);

    heap.Insert(8);
    EXPECT_EQ(heap.GetRoot(), 8); 
    EXPECT_EQ(heap.GetSize(), 3);
}

TEST(BinaryHeapTest, ExtractRootRemovesElementsInCorrectOrder) {
    BinaryHeap<int> heap;
    
    int elements[] = {5, 8, 3, 7, 1, 4, 8, 0, 3, 5};
    size_t count = sizeof(elements) / sizeof(elements[0]);
    
    for (size_t i = 0; i < count; ++i) {
        heap.Insert(elements[i]);
    }
    
    EXPECT_EQ(heap.GetSize(), 10);

    EXPECT_EQ(heap.GetRoot(), 8); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 8); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 7); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 5); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 5); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 4); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 3); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 3); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 1); heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 0); heap.ExtractRoot();

    EXPECT_TRUE(heap.IsEmpty());
}

TEST(BinaryHeapTest, InsertRvalueMovesResource) {
    BinaryHeap<std::string> heap;
    std::string str = "heavy_string_resource_to_move";
    
    heap.Insert(std::move(str));
    
    EXPECT_EQ(heap.GetRoot(), "heavy_string_resource_to_move");
    EXPECT_TRUE(str.empty()); 
}


TEST(BinaryHeapTest, ConstructorFromConstArrayReferenceBuildsCorrectHeap) {
    int raw_data[] = {1, 3, 5, 4, 6, 13, 10};
    DynamicArray<int> arr(raw_data, 7);

    BinaryHeap<int> heap(std::less<int>(), arr);

    EXPECT_EQ(heap.GetSize(), 7);
    EXPECT_EQ(heap.GetRoot(), 13); 
}

TEST(BinaryHeapTest, ConstructorFromRvalueArrayMovesArrayAndBuildsHeap) {
    int raw_data[] = {2, 7, 26, 25, 19, 1};
    DynamicArray<int> arr(raw_data, 6);

    BinaryHeap<int> heap(std::less<int>(), std::move(arr));

    EXPECT_EQ(heap.GetSize(), 6);
    EXPECT_EQ(heap.GetRoot(), 26);
    EXPECT_EQ(arr.GetSize(), 0); 
}

TEST(BinaryHeapTest, CopyConstructorAndAssignmentOperator) {
    BinaryHeap<int> original;
    original.Insert(10);
    original.Insert(20);
    original.Insert(15);

    BinaryHeap<int> copy(original);
    EXPECT_EQ(copy.GetSize(), original.GetSize());
    EXPECT_EQ(copy.GetRoot(), original.GetRoot());

    BinaryHeap<int> assigned;
    assigned = original;
    EXPECT_EQ(assigned.GetSize(), original.GetSize());
    EXPECT_EQ(assigned.GetRoot(), original.GetRoot());

    copy.ExtractRoot();
    EXPECT_NE(copy.GetRoot(), original.GetRoot());
}

TEST(BinaryHeapTest, MoveConstructorAndAssignmentOperator) {
    BinaryHeap<int> original;
    original.Insert(42);
    original.Insert(24);

    BinaryHeap<int> moved(std::move(original));
    EXPECT_EQ(moved.GetSize(), 2);
    EXPECT_EQ(moved.GetRoot(), 42);
    EXPECT_TRUE(original.IsEmpty()); 
    
    BinaryHeap<int> moved_assigned;
    moved_assigned = std::move(moved);
    EXPECT_EQ(moved_assigned.GetSize(), 2);
    EXPECT_EQ(moved_assigned.GetRoot(), 42);
    EXPECT_TRUE(moved.IsEmpty());
}

TEST(BinaryHeapTest, GreaterComparatorCreatesMinHeap) {
    BinaryHeap<int, std::greater<int>> min_heap;

    min_heap.Insert(10);
    min_heap.Insert(5);
    min_heap.Insert(20);
    min_heap.Insert(1);

    EXPECT_EQ(min_heap.GetRoot(), 1);
    min_heap.ExtractRoot();
    EXPECT_EQ(min_heap.GetRoot(), 5);
}

TEST(BinaryHeapTest, SwapMethodExchangesHeapsEntirely) {
    BinaryHeap<int> heap1;
    heap1.Insert(10);
    heap1.Insert(20);

    BinaryHeap<int> heap2;
    heap2.Insert(100);

    heap1.Swap(heap2);

    EXPECT_EQ(heap1.GetSize(), 1);
    EXPECT_EQ(heap1.GetRoot(), 100);

    EXPECT_EQ(heap2.GetSize(), 2);
    EXPECT_EQ(heap2.GetRoot(), 20);
}