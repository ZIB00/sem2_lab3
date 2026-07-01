#include <gtest/gtest.h>
#include "include/BinaryHeap.hpp"
#include "include/MyComplex.hpp"
#include <string>
#include <limits>

// Конструкторы          

TEST(BinaryHeapTest, DefaultConstructor_IsEmpty) {
    BinaryHeap<int> heap;

    EXPECT_TRUE(heap.IsEmpty()) << "Ввод: BinaryHeap<int>()\n";
}

TEST(BinaryHeapTest, DefaultConstructor_SizeIsZero) {
    BinaryHeap<int> heap;

    EXPECT_EQ(heap.GetSize(), 0u) << "Ввод: BinaryHeap<int>()\n";
}

TEST(BinaryHeapTest, ConstructorFromConstArray_SizeMatchesInput) {
    int raw[] = {1, 3, 5, 4, 6, 13, 10};
    DynamicArray<int> arr(raw, 7);

    BinaryHeap<int> heap(std::less<int>(), arr);

    EXPECT_EQ(heap.GetSize(), 7u) << "Ввод: {1,3,5,4,6,13,10}\n";
}

TEST(BinaryHeapTest, ConstructorFromConstArray_RootIsMaxElement) {
    int raw[] = {1, 3, 5, 4, 6, 13, 10};
    DynamicArray<int> arr(raw, 7);

    BinaryHeap<int> heap(std::less<int>(), arr);

    EXPECT_EQ(heap.GetRoot(), 13) << "Ввод: {1,3,5,4,6,13,10}\n";
}

TEST(BinaryHeapTest, ConstructorFromEmptyArray_IsEmpty) {
    DynamicArray<int> arr(0);

    BinaryHeap<int> heap(std::less<int>(), arr);

    EXPECT_TRUE(heap.IsEmpty()) << "Ввод: пустой\n";
}

TEST(BinaryHeapTest, ConstructorFromRvalueArray_RootIsMaxElement) {
    int raw[] = {2, 7, 26, 25, 19, 1};
    DynamicArray<int> arr(raw, 6);

    BinaryHeap<int> heap(std::less<int>(), std::move(arr));

    EXPECT_EQ(heap.GetRoot(), 26) << "Ввод: {2,7,26,25,19,1} (rvalue)\n";
}

TEST(BinaryHeapTest, ConstructorFromRvalueArray_SourceIsEmpty) {
    int raw[] = {2, 7, 26, 25, 19, 1};
    DynamicArray<int> arr(raw, 6);

    BinaryHeap<int> heap(std::less<int>(), std::move(arr));

    EXPECT_EQ(arr.GetSize(), 0u)
        << "Ввод: {2,7,26,25,19,1} передан через std::move\n"
        << "Ожидалось: arr.GetSize() == 0 (данные перемещены)\n";
}

TEST(BinaryHeapTest, CopyConstructor_RootMatchesOriginal) {
    BinaryHeap<int> original;
    original.Push(10);
    original.Push(20);
    original.Push(15);

    BinaryHeap<int> copy(original);

    EXPECT_EQ(copy.GetRoot(), original.GetRoot()) << "Ввод: копия BinaryHeap{10,15,20}\n";
}

TEST(BinaryHeapTest, CopyConstructor_IsDeepCopy) {
    BinaryHeap<int> original;
    original.Push(10);
    BinaryHeap<int> copy(original);

    copy.Push(99);

    EXPECT_EQ(original.GetRoot(), 10)
        << "Ввод: Push(99) в копию BinaryHeap{10}\n"
        << "Ожидалось: оригинал не изменился, GetRoot() == 10\n";
}

TEST(BinaryHeapTest, MoveConstructor_DestinationHasData) {
    BinaryHeap<int> original;
    original.Push(42);
    original.Push(24);

    BinaryHeap<int> moved(std::move(original));

    EXPECT_EQ(moved.GetRoot(), 42) << "Ввод: std::move(BinaryHeap{42,24})\n";
}

TEST(BinaryHeapTest, MoveConstructor_SourceIsEmpty) {
    BinaryHeap<int> original;
    original.Push(42);

    BinaryHeap<int> moved(std::move(original));

    EXPECT_TRUE(original.IsEmpty())
        << "Ввод: std::move(BinaryHeap{42})\n"
        << "Ожидалось: источник IsEmpty() == true (данные перемещены)\n";
}

// Оператор присваивания 

TEST(BinaryHeapTest, Copy_RootMatchesSource) {
    BinaryHeap<int> original;
    original.Push(10);
    original.Push(20);
    BinaryHeap<int> assigned;

    assigned = original;

    EXPECT_EQ(assigned.GetRoot(), original.GetRoot()) << "Ввод: assigned = BinaryHeap{10,20}\n";
}

TEST(BinaryHeapTest, Move_DestinationHasData) {
    BinaryHeap<int> src;
    src.Push(10);
    src.Push(50);
    BinaryHeap<int> dest;

    dest = std::move(src);

    EXPECT_EQ(dest.GetRoot(), 50) << "Ввод: dest = std::move(BinaryHeap{10,50})\n";
}

TEST(BinaryHeapTest, Move_SourceIsEmpty) {
    BinaryHeap<int> src;
    src.Push(10);
    BinaryHeap<int> dest;

    dest = std::move(src);

    EXPECT_TRUE(src.IsEmpty())
        << "Ввод: dest = std::move(BinaryHeap{10})\n"
        << "Ожидалось: источник IsEmpty() == true (данные перемещены)\n";
}

TEST(BinaryHeapTest, Move_Self_DataIntact) {
    BinaryHeap<int> heap;
    heap.Push(5);
    heap.Push(15);

    heap = std::move(heap);

    EXPECT_EQ(heap.GetRoot(), 15) << "Ввод: heap = std::move(heap) (самоприсваивание), BinaryHeap{5,15}\n";
}

TEST(BinaryHeapTest, Copy_Chained) {
    BinaryHeap<int> heap1;
    heap1.Push(5);
    BinaryHeap<int> heap2;
    BinaryHeap<int> heap3;

    heap3 = heap2 = heap1;

    EXPECT_EQ(heap3.GetRoot(), 5) << "Ввод: heap3 = heap2 = BinaryHeap{5}\n";
}

// Push      

TEST(BinaryHeapTest, Push_SingleElement_RootIsElement) {
    BinaryHeap<int> heap;

    heap.Push(5);

    EXPECT_EQ(heap.GetRoot(), 5) << "Ввод: Push(5) в пустую кучу\n";
}

TEST(BinaryHeapTest, Push_LargerElement_UpdatesRoot) {
    BinaryHeap<int> heap;
    heap.Push(5);

    heap.Push(8);

    EXPECT_EQ(heap.GetRoot(), 8) << "Ввод: Push(5), Push(8)\n";
}

TEST(BinaryHeapTest, Push_SmallerElement_RootUnchanged) {
    BinaryHeap<int> heap;
    heap.Push(5);

    heap.Push(3);

    EXPECT_EQ(heap.GetRoot(), 5) << "Ввод: Push(5), Push(3)\n";
}

TEST(BinaryHeapTest, Push_SizeIncrementsCorrectly) {
    BinaryHeap<int> heap;
    heap.Push(5);
    heap.Push(3);

    heap.Push(8);

    EXPECT_EQ(heap.GetSize(), 3u) << "Ввод: Push(5), Push(3), Push(8)\n";
}

TEST(BinaryHeapTest, Push_Duplicates_SizeIsCorrect) {
    BinaryHeap<int> heap;

    heap.Push(10);
    heap.Push(10);
    heap.Push(10);

    EXPECT_EQ(heap.GetSize(), 3u) << "Ввод: Push(10) трижды\n";
}

// Pop       

TEST(BinaryHeapTest, Pop_RemovesMaxElement_NewRootIsNextMax) {
    BinaryHeap<int> heap;
    heap.Push(5);
    heap.Push(10);
    heap.Push(3);

    heap.Pop();

    EXPECT_EQ(heap.GetRoot(), 5) << "Ввод: Pop() из BinaryHeap{3,5,10}\n";
}

TEST(BinaryHeapTest, Pop_DecrementsSizeByOne) {
    BinaryHeap<int> heap;
    heap.Push(10);
    heap.Push(20);
    heap.Push(30);

    heap.Pop();

    EXPECT_EQ(heap.GetSize(), 2u) << "Ввод: Pop() из BinaryHeap{10,20,30}\n";
}

TEST(BinaryHeapTest, Pop_AllElements_HeapIsEmpty) {
    BinaryHeap<int> heap;
    heap.Push(10);
    heap.Push(20);
    heap.Push(30);

    heap.Pop();
    heap.Pop();
    heap.Pop();

    EXPECT_TRUE(heap.IsEmpty()) << "Ввод: Pop() трижды из BinaryHeap{10,20,30}\n";
}

TEST(BinaryHeapTest, Pop_SingleElement_HeapIsEmpty) {
    BinaryHeap<int> heap;
    heap.Push(100);

    heap.Pop();

    EXPECT_TRUE(heap.IsEmpty()) << "Ввод: Pop() из BinaryHeap{100}\n";
}

TEST(BinaryHeapTest, Pop_ExtractsElementsInDescendingOrder) {
    BinaryHeap<int> heap;
    int elements[] = {5, 8, 3, 7, 1, 4};
    for (int e : elements) heap.Push(e);
    int expected[] = {8, 7, 5, 4, 3, 1};

    for (int exp : expected) {
        int actual = heap.GetRoot();
        heap.Pop();
        EXPECT_EQ(actual, exp) << "Ввод: BinaryHeap{1,3,4,5,7,8}, ожидаемый элемент: " << exp << "\n";
    }
}

// Кастомный компаратор  

TEST(BinaryHeapTest, GreaterComparator_RootIsMinimum) {
    BinaryHeap<int, std::greater<int>> min_heap;

    min_heap.Push(10);
    min_heap.Push(5);
    min_heap.Push(20);
    min_heap.Push(1);

    EXPECT_EQ(min_heap.GetRoot(), 1) << "Ввод: std::greater<int>, Push {10,5,20,1}\n";
}

TEST(BinaryHeapTest, GreaterComparator_PopRevealsNextMinimum) {
    BinaryHeap<int, std::greater<int>> min_heap;
    min_heap.Push(10);
    min_heap.Push(5);
    min_heap.Push(20);

    min_heap.Pop();

    EXPECT_EQ(min_heap.GetRoot(), 10) << "Ввод: Pop() из min-heap {5,10,20}\n";
}

TEST(BinaryHeapTest, CustomLambdaComparator_StringByLength) {
    auto comp = [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    };
    BinaryHeap<std::string, decltype(comp)> heap(comp);

    heap.Push("A");
    heap.Push("AAAA");
    heap.Push("AA");

    EXPECT_EQ(heap.GetRoot(), "AAAA") << "Ввод: компаратор по длине, Push {\"A\",\"AAAA\",\"AA\"}\n";
}