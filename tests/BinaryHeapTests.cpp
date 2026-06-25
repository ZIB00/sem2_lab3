#include <gtest/gtest.h>
#include "include/BinaryHeap.hpp"
#include "include/MyComplex.hpp"
#include <string>
#include <limits>

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
    EXPECT_EQ(heap.GetRoot(), 8);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 8);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 7);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 5);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 5);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 4);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 3);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 3);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 1);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 0);
    heap.ExtractRoot();
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

TEST(BinaryHeapTest, InsertDuplicateValuesMaintainsStructure) {
    BinaryHeap<int> heap;
    heap.Insert(10);
    heap.Insert(10);
    heap.Insert(10);
    EXPECT_EQ(heap.GetSize(), 3);
    EXPECT_EQ(heap.GetRoot(), 10);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 10);
}

TEST(BinaryHeapTest, SequenceOfInsertsAndExtracts) {
    BinaryHeap<int> heap;
    heap.Insert(10);
    heap.Insert(20);
    EXPECT_EQ(heap.GetRoot(), 20);
    heap.ExtractRoot();
    heap.Insert(5);
    heap.Insert(30);
    EXPECT_EQ(heap.GetRoot(), 30);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 10);
}

TEST(BinaryHeapTest, ComplexTypeMaxHeapification) {
    BinaryHeap<MyComplex<double>> heap;
    heap.Insert(MyComplex<double>(1.0, 1.0));
    heap.Insert(MyComplex<double>(3.0, 3.0));
    heap.Insert(MyComplex<double>(2.0, 2.0));
    EXPECT_EQ(heap.GetRoot().real(), 3.0);
    EXPECT_EQ(heap.GetRoot().imag(), 3.0);
}

TEST(BinaryHeapTest, ComplexTypeMinHeapificationWithGreater) {
    auto comp = [](const MyComplex<double>& a, const MyComplex<double>& b) {
        return b < a;
    };
    BinaryHeap<MyComplex<double>, decltype(comp)> heap(comp);
    heap.Insert(MyComplex<double>(3.0, 3.0));
    heap.Insert(MyComplex<double>(1.0, 1.0));
    heap.Insert(MyComplex<double>(2.0, 2.0));
    EXPECT_EQ(heap.GetRoot().real(), 1.0);
    EXPECT_EQ(heap.GetRoot().imag(), 1.0);
}

TEST(BinaryHeapTest, SelfAssignmentDoesNotCorruptData) {
    BinaryHeap<int> heap;
    heap.Insert(5);
    heap.Insert(15);
    heap = *&heap;
    EXPECT_EQ(heap.GetSize(), 2);
    EXPECT_EQ(heap.GetRoot(), 15);
}

TEST(BinaryHeapTest, MoveSelfAssignmentDoesNotCorruptData) {
    BinaryHeap<int> heap;
    heap.Insert(5);
    heap.Insert(15);
    heap = std::move(heap);
    EXPECT_EQ(heap.GetSize(), 2);
    EXPECT_EQ(heap.GetRoot(), 15);
}

TEST(BinaryHeapTest, LargeNumberOfElementsAscending) {
    BinaryHeap<int> heap;
    for (int i = 0; i < 1000; ++i) {
        heap.Insert(i);
    }
    EXPECT_EQ(heap.GetSize(), 1000);
    EXPECT_EQ(heap.GetRoot(), 999);
}

TEST(BinaryHeapTest, LargeNumberOfElementsDescending) {
    BinaryHeap<int> heap;
    for (int i = 1000; i > 0; --i) {
        heap.Insert(i);
    }
    EXPECT_EQ(heap.GetSize(), 1000);
    EXPECT_EQ(heap.GetRoot(), 1000);
}

TEST(BinaryHeapTest, ExtractUntilEmptyVerifiesSizeDecrement) {
    BinaryHeap<int> heap;
    heap.Insert(10);
    heap.Insert(20);
    heap.Insert(30);
    EXPECT_EQ(heap.GetSize(), 3);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetSize(), 2);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetSize(), 1);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetSize(), 0);
    EXPECT_TRUE(heap.IsEmpty());
}

TEST(BinaryHeapTest, CustomComparatorStringLength) {
    auto comp = [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    };
    BinaryHeap<std::string, decltype(comp)> heap(comp);
    heap.Insert("A");
    heap.Insert("AAAA");
    heap.Insert("AA");
    EXPECT_EQ(heap.GetRoot(), "AAAA");
}

TEST(BinaryHeapTest, MoveConstructorClearsSourceHeapMetrics) {
    BinaryHeap<int> src;
    src.Insert(10);
    src.Insert(20);
    BinaryHeap<int> dest(std::move(src));
    EXPECT_EQ(src.GetSize(), 0);
    EXPECT_TRUE(src.IsEmpty());
    EXPECT_EQ(dest.GetSize(), 2);
}

TEST(BinaryHeapTest, SiftUpStressPerformanceEquivalent) {
    BinaryHeap<int> heap;
    for (int i = 0; i < 500; ++i) {
        heap.Insert(5);
    }
    heap.Insert(10);
    EXPECT_EQ(heap.GetRoot(), 10);
}

TEST(BinaryHeapTest, SiftDownStressPerformanceEquivalent) {
    BinaryHeap<int> heap;
    heap.Insert(100);
    for (int i = 0; i < 500; ++i) {
        heap.Insert(10);
    }
    EXPECT_EQ(heap.GetRoot(), 100);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 10);
}

TEST(BinaryHeapTest, InsertNegativeValuesCorrectOrder) {
    BinaryHeap<int> heap;
    heap.Insert(-10);
    heap.Insert(-5);
    heap.Insert(-20);
    EXPECT_EQ(heap.GetRoot(), -5);
}

TEST(BinaryHeapTest, HeapWithFloatingPointNumbers) {
    BinaryHeap<double> heap;
    heap.Insert(3.14);
    heap.Insert(2.71);
    heap.Insert(1.41);
    EXPECT_EQ(heap.GetRoot(), 3.14);
}

TEST(BinaryHeapTest, DeepCopyIsIndependent) {
    BinaryHeap<int> heap1;
    heap1.Insert(10);
    BinaryHeap<int> heap2 = heap1;
    heap2.Insert(20);
    EXPECT_EQ(heap1.GetRoot(), 10);
    EXPECT_EQ(heap2.GetRoot(), 20);
}

TEST(BinaryHeapTest, ConstructorFromEmptyDynamicArray) {
    DynamicArray<int> arr(0);
    BinaryHeap<int> heap(std::less<int>(), arr);
    EXPECT_TRUE(heap.IsEmpty());
    EXPECT_EQ(heap.GetSize(), 0);
}

TEST(BinaryHeapTest, ConstructorFromSingleElementDynamicArray) {
    int raw[] = {42};
    DynamicArray<int> arr(raw, 1);
    BinaryHeap<int> heap(std::less<int>(), arr);
    EXPECT_EQ(heap.GetSize(), 1);
    EXPECT_EQ(heap.GetRoot(), 42);
}

TEST(BinaryHeapTest, ExtractRootOnSingleElementHeap) {
    BinaryHeap<int> heap;
    heap.Insert(100);
    heap.ExtractRoot();
    EXPECT_TRUE(heap.IsEmpty());
    EXPECT_EQ(heap.GetSize(), 0);
}

TEST(BinaryHeapTest, OperatorsReturnCorrectReferences) {
    BinaryHeap<int> heap1;
    BinaryHeap<int> heap2;
    BinaryHeap<int> heap3;
    heap1.Insert(5);
    heap3 = heap2 = heap1;
    EXPECT_EQ(heap3.GetRoot(), 5);
}

TEST(BinaryHeapTest, CharTemplateTypeInstantiation) {
    BinaryHeap<char> heap;
    heap.Insert('a');
    heap.Insert('z');
    heap.Insert('m');
    EXPECT_EQ(heap.GetRoot(), 'z');
}

TEST(BinaryHeapTest, AlternateInsertsAndExtractsMixed) {
    BinaryHeap<int> heap;
    heap.Insert(10);
    heap.Insert(5);
    EXPECT_EQ(heap.GetRoot(), 10);
    heap.ExtractRoot();
    heap.Insert(20);
    EXPECT_EQ(heap.GetRoot(), 20);
    heap.ExtractRoot();
    EXPECT_EQ(heap.GetRoot(), 5);
}

TEST(BinaryHeapTest, MoveAssignmentIntoActiveHeapOverwrites) {
    BinaryHeap<int> heap1;
    heap1.Insert(10);
    BinaryHeap<int> heap2;
    heap2.Insert(50);
    heap2.Insert(60);
    heap1 = std::move(heap2);
    EXPECT_EQ(heap1.GetSize(), 2);
    EXPECT_EQ(heap1.GetRoot(), 60);
}

TEST(BinaryHeapTest, MyComplexTypeAbsoluteValuePropertyOrder) {
    BinaryHeap<MyComplex<int>> heap;
    heap.Insert(MyComplex<int>(0, 5));
    heap.Insert(MyComplex<int>(2, 0));
    EXPECT_EQ(heap.GetRoot().imag(), 5);
}

TEST(BinaryHeapTest, MinHeapWithFloatingPointValues) {
    auto comp = [](double a, double b) {
        return b < a;
    };
    BinaryHeap<double, decltype(comp)> heap(comp);
    heap.Insert(0.001);
    heap.Insert(0.1);
    heap.Insert(0.01);
    EXPECT_EQ(heap.GetRoot(), 0.001);
}

TEST(BinaryHeapTest, InsertRvalueArrayElementsCorrectly) {
    BinaryHeap<std::string> heap;
    std::string s1 = "aaa";
    std::string s2 = "bbb";
    heap.Insert(std::move(s1));
    heap.Insert(std::move(s2));
    EXPECT_EQ(heap.GetSize(), 2);
}

TEST(BinaryHeapTest, ExtremeValuesIntMinMax) {
    BinaryHeap<int> heap;
    heap.Insert(std::numeric_limits<int>::min());
    heap.Insert(std::numeric_limits<int>::max());
    EXPECT_EQ(heap.GetRoot(), std::numeric_limits<int>::max());
}

TEST(BinaryHeapTest, ReverseSortedArrayInitialization) {
    int raw[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    DynamicArray<int> arr(raw, 10);
    BinaryHeap<int> heap(std::less<int>(), arr);
    EXPECT_EQ(heap.GetRoot(), 10);
    EXPECT_EQ(heap.GetSize(), 10);
}

TEST(BinaryHeapTest, SortedArrayInitialization) {
    int raw[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    DynamicArray<int> arr(raw, 10);
    BinaryHeap<int> heap(std::less<int>(), arr);
    EXPECT_EQ(heap.GetRoot(), 10);
}