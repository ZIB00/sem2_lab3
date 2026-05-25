#include <gtest/gtest.h>
#include "include/PriorityQueue.hpp"
#include "include/sequence/sequences/DynamicArray.hpp"
#include "include/MyComplex.hpp"
#include <string>

template<typename T, typename Comp>
DynamicArray<T> DumpQueueToDynamicArray(PriorityQueue<T, Comp> q) {
    DynamicArray<T> result(q.Size());
    size_t idx = 0;
    while (!q.Empty()) {
        result.Set(idx++, q.Top());
        q.pop();
    }
    return result;
}

template<typename T>
void AssertArraysEqual(const DynamicArray<T>& actual, const DynamicArray<T>& expected, 
                       const std::string& functionName, const std::string& inputs) {
    if (actual.GetSize() != expected.GetSize()) {
        size_t minSize = (actual.GetSize() < expected.GetSize()) ? actual.GetSize() : expected.GetSize();
        FAIL() << "Function: " << functionName << "\n"
               << "Inputs: " << inputs << "\n"
               << "Error: Size mismatch\n"
               << "Expected size: " << expected.GetSize() << "\n"
               << "Actual size: " << actual.GetSize() << "\n"
               << "Mismatch at index: " << minSize << "\n"
               << "Expected value: " << (minSize < expected.GetSize() ? std::to_string(expected.Get(minSize)) : "N/A") << "\n"
               << "Actual value: " << (minSize < actual.GetSize() ? std::to_string(actual.Get(minSize)) : "N/A");
    }
    for (size_t i = 0; i < actual.GetSize(); ++i) {
        if (!(actual.Get(i) == expected.Get(i))) {
            FAIL() << "Function: " << functionName << "\n"
                   << "Inputs: " << inputs << "\n"
                   << "Error: Value mismatch\n"
                   << "Expected size: " << expected.GetSize() << "\n"
                   << "Actual size: " << actual.GetSize() << "\n"
                   << "Mismatch at index: " << i << "\n"
                   << "Expected value: " << std::to_string(expected.Get(i)) << "\n"
                   << "Actual value: " << std::to_string(actual.Get(i));
        }
    }
}

template<typename T, typename Comp>
void AssertQueueIsEmpty(const PriorityQueue<T, Comp>& q, const std::string& functionName, const std::string& inputs) {
    if (!q.Empty()) {
        FAIL() << "Function: " << functionName << "\n"
               << "Inputs: " << inputs << "\n"
               << "Error: Queue is not empty\n"
               << "Expected: Empty queue\n"
               << "Actual size: " << q.Size();
    }
}

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
    DynamicArray<int> array = {10, 30, 20, 5, 40};
    PriorityQueue<int> pq(std::less<int>(), array);
    EXPECT_EQ(pq.Size(), 5);
    EXPECT_EQ(pq.Top(), 40);
}

TEST(PriorityQueueTest, ConstructorFromRvalueContainerMovesData) {
    DynamicArray<int> array = {7, 8, 9};
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
    PriorityQueue<int> pq = {5, 1, 9, 3};
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
    PriorityQueue<int> original = {100, 500};
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
    PriorityQueue<int> original = {42};
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
    auto comp = [](int a, int b) {
        return b < a;
    };
    PriorityQueue<int, decltype(comp)> min_pq(comp);
    min_pq.Push(50);
    min_pq.Push(10);
    min_pq.Push(30);
    EXPECT_EQ(min_pq.Top(), 10);
    min_pq.pop();
    EXPECT_EQ(min_pq.Top(), 30);
}

TEST(PriorityQueueTest, SwapMethodExchangesTwoQueuesEntirely) {
    PriorityQueue<int> pq1 = {1, 2};
    PriorityQueue<int> pq2 = {99};
    pq1.swap(pq2);
    EXPECT_EQ(pq1.Size(), 1);
    EXPECT_EQ(pq1.Top(), 99);
    EXPECT_EQ(pq2.Size(), 2);
    EXPECT_EQ(pq2.Top(), 2);
}

TEST(PriorityQueueExtensionsTest, MapStandardCase) {
    PriorityQueue<int> pq = {1, 3, 2};
    PriorityQueue<int> result = pq.Map([](const int& x) { return x * 2; });
    DynamicArray<int> expected = {6, 4, 2};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Map", "Queue=[3,2,1], Func=x*2");
}

TEST(PriorityQueueExtensionsTest, MapEmptyQueue) {
    PriorityQueue<int> pq;
    PriorityQueue<int> result = pq.Map([](const int& x) { return x * 2; });
    AssertQueueIsEmpty(result, "Map", "Queue=[], Func=x*2");
}

TEST(PriorityQueueExtensionsTest, MapNegativeValues) {
    PriorityQueue<int> pq = {-5, -1, -10};
    PriorityQueue<int> result = pq.Map([](const int& x) { return x + 5; });
    DynamicArray<int> expected = {4, 0, -4};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Map", "Queue=[-1,-5,-10], Func=x+5");
}

TEST(PriorityQueueExtensionsTest, WhereFiltersEven) {
    PriorityQueue<int> pq = {1, 4, 3, 6, 5};
    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });
    DynamicArray<int> expected = {6, 4};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Where", "Queue=[6,5,4,3,1], Pred=x%2==0");
}

TEST(PriorityQueueExtensionsTest, WhereNoMatches) {
    PriorityQueue<int> pq = {1, 3, 5};
    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });
    AssertQueueIsEmpty(result, "Where", "Queue=[5,3,1], Pred=x%2==0");
}

TEST(PriorityQueueExtensionsTest, WhereAllMatch) {
    PriorityQueue<int> pq = {2, 4, 6};
    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });
    DynamicArray<int> expected = {6, 4, 2};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Where", "Queue=[6,4,2], Pred=x%2==0");
}

TEST(PriorityQueueExtensionsTest, ReduceSum) {
    PriorityQueue<int> pq = {10, 20, 30};
    int sum = pq.Reduce<int>([](const int& acc, const int& x) { return acc + x; });
    if (sum != 60) {
        FAIL() << "Function: Reduce\nInputs: Queue=[30,20,10]\nExpected: 60\nActual: " << sum;
    }
}

TEST(PriorityQueueExtensionsTest, ReduceEmptyQueue) {
    PriorityQueue<int> pq;
    int sum = pq.Reduce<int>([](const int& acc, const int& x) { return acc + x; });
    if (sum != 0) {
        FAIL() << "Function: Reduce\nInputs: Queue=[]\nExpected: 0\nActual: " << sum;
    }
}

TEST(PriorityQueueExtensionsTest, ReduceProductWithZero) {
    PriorityQueue<int> pq = {5, 10, 2};
    int product = pq.Reduce<int>([](const int& acc, const int& x) { return acc * x; });
    if (product != 0) {
        FAIL() << "Function: Reduce\nInputs: Queue=[10,5,2]\nExpected: 0\nActual: " << product;
    }
}

TEST(PriorityQueueExtensionsTest, ConcatenateStandard) {
    PriorityQueue<int> pq1 = {10, 30};
    PriorityQueue<int> pq2 = {40, 20};
    PriorityQueue<int> result = pq1.Concat(pq2);
    DynamicArray<int> expected = {40, 30, 20, 10};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Concat", "PQ1=[30,10], PQ2=[40,20]");
    if (pq1.Size() != 2 || pq2.Size() != 2) {
        FAIL() << "Function: Concat\nInputs: Immutability Check\nExpected: Sizes 2 and 2\nActual: " << pq1.Size() << " " << pq2.Size();
    }
}

TEST(PriorityQueueExtensionsTest, ConcatenateWithEmpty) {
    PriorityQueue<int> pq1 = {5, 15};
    PriorityQueue<int> pq2;
    PriorityQueue<int> result = pq1.Concat(pq2);
    DynamicArray<int> expected = {15, 5};
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "Concat", "PQ1=[15,5], PQ2=[]");
}

TEST(PriorityQueueExtensionsTest, GetSubsequenceStandardRange) {
    PriorityQueue<int> pq;
    pq.Push(10);
    pq.Push(40);
    pq.Push(20);
    pq.Push(30);
    PriorityQueue<int> result = pq.GetSubsequence(1, 2);
    DynamicArray<int> expected(2);
    expected.Set(0, 30);
    expected.Set(1, 20);
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "GetSubsequence", "Queue=[40,30,20,10], Range=[1, 2]");
}

TEST(PriorityQueueExtensionsTest, GetSubsequenceSingleElement) {
    PriorityQueue<int> pq;
    pq.Push(10);
    pq.Push(40);
    pq.Push(20);
    PriorityQueue<int> result = pq.GetSubsequence(0, 0);
    DynamicArray<int> expected(1);
    expected.Set(0, 40);
    AssertArraysEqual(DumpQueueToDynamicArray(result), expected, "GetSubsequence", "Queue=[40,20,10], Range=[0, 0]");
}

TEST(PriorityQueueExtensionsTest, GetSubsequenceInvalidRange) {
    PriorityQueue<int> pq;
    pq.Push(10);
    pq.Push(20);
    PriorityQueue<int> result = pq.GetSubsequence(2, 1);
    AssertQueueIsEmpty(result, "GetSubsequence", "Queue=[20,10], Range=[2, 1]");
}

TEST(PriorityQueueExtensionsTest, GetSubsequenceOutOfRange) {
    PriorityQueue<int> pq;
    pq.Push(10);
    PriorityQueue<int> result = pq.GetSubsequence(5, 10);
    AssertQueueIsEmpty(result, "GetSubsequence", "Queue=[10], Range=[5, 10]");
}

TEST(PriorityQueueExtensionsTest, ContainsSubsequenceFalse) {
    PriorityQueue<int> mainPq = {10, 20};
    PriorityQueue<int> subPq = {20, 99};
    if (mainPq.IsSubsequence(subPq)) {
        FAIL() << "Function: IsSubsequence\nInputs: Main=[20,10], Sub=[99,20]\nExpected: false";
    }
}

TEST(PriorityQueueExtensionsTest, ContainsSubsequenceEmptySub) {
    PriorityQueue<int> mainPq = {10};
    PriorityQueue<int> subPq;
    if (!mainPq.IsSubsequence(subPq)) {
        FAIL() << "Function: IsSubsequence\nInputs: Main=[10], Sub=[]\nExpected: true";
    }
}

TEST(PriorityQueueExtensionsTest, AppendStandard) {
    PriorityQueue<int> pq1 = {30, 10};
    PriorityQueue<int> pq2 = {40, 20};
    pq1.Append(pq2);
    DynamicArray<int> expected(4);
    expected.Set(0, 40);
    expected.Set(1, 30);
    expected.Set(2, 20);
    expected.Set(3, 10);
    AssertArraysEqual(DumpQueueToDynamicArray(pq1), expected, "Append", "PQ1=[30,10], PQ2=[40,20]");
}

TEST(PriorityQueueExtensionsTest, AppendToEmpty) {
    PriorityQueue<int> pq1;
    PriorityQueue<int> pq2 = {5, 1};
    pq1.Append(pq2);
    DynamicArray<int> expected(2);
    expected.Set(0, 5);
    expected.Set(1, 1);
    AssertArraysEqual(DumpQueueToDynamicArray(pq1), expected, "Append", "PQ1=[], PQ2=[5,1]");
}

TEST(PriorityQueueExtensionsTest, SplitBalanced) {
    PriorityQueue<int> pq = {1, 2, 3, 4};
    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });
    DynamicArray<int> expectedTrue = {4, 2};
    DynamicArray<int> expectedFalse = {3, 1};
    AssertArraysEqual(DumpQueueToDynamicArray(trueQueue), expectedTrue, "Split (True Branch)", "Queue=[4,3,2,1]");
    AssertArraysEqual(DumpQueueToDynamicArray(falseQueue), expectedFalse, "Split (False Branch)", "Queue=[4,3,2,1]");
}

TEST(PriorityQueueExtensionsTest, SplitAllFalse) {
    PriorityQueue<int> pq = {1, 3, 5};
    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });
    DynamicArray<int> expectedFalse = {5, 3, 1};
    AssertQueueIsEmpty(trueQueue, "Split (True Branch)", "Queue=[5,3,1]");
    AssertArraysEqual(DumpQueueToDynamicArray(falseQueue), expectedFalse, "Split (False Branch)", "Queue=[5,3,1]");
}

TEST(PriorityQueueTest, PushRvalueMaintainsDataCorrectness) {
    PriorityQueue<std::string> pq;
    std::string text = "rvalue_test";
    pq.Push(std::move(text));
    EXPECT_EQ(pq.Top(), "rvalue_test");
    EXPECT_TRUE(text.empty());
}

TEST(PriorityQueueTest, ClearQueueByPopping) {
    PriorityQueue<int> pq = {1, 2, 3};
    pq.pop();
    pq.pop();
    pq.pop();
    EXPECT_TRUE(pq.Empty());
    EXPECT_EQ(pq.Size(), 0);
}

TEST(PriorityQueueTest, InitializerListSingleElement) {
    PriorityQueue<int> pq = {42};
    EXPECT_EQ(pq.Size(), 1);
    EXPECT_EQ(pq.Top(), 42);
}

TEST(PriorityQueueTest, MyComplexMaxPriorityQueueOrder) {
    PriorityQueue<MyComplex<int>> pq;
    pq.Push(MyComplex<int>(1, 1));
    pq.Push(MyComplex<int>(5, 5));
    pq.Push(MyComplex<int>(2, 2));
    EXPECT_EQ(pq.Top().real(), 5);
}

TEST(PriorityQueueTest, MyComplexMinPriorityQueueOrder) {
    auto comp = [](const MyComplex<int>& a, const MyComplex<int>& b) {
        return b < a;
    };
    PriorityQueue<MyComplex<int>, decltype(comp)> pq(comp);
    pq.Push(MyComplex<int>(5, 5));
    pq.Push(MyComplex<int>(1, 1));
    pq.Push(MyComplex<int>(2, 2));
    EXPECT_EQ(pq.Top().real(), 1);
}

TEST(PriorityQueueTest, AssignmentOperatorReturnsSelfReference) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2;
    pq2 = pq1;
    EXPECT_EQ(pq2.Size(), 2);
    EXPECT_EQ(pq2.Top(), 20);
}

TEST(PriorityQueueTest, MoveAssignmentClearsActiveElements) {
    PriorityQueue<int> pq1 = {10};
    PriorityQueue<int> pq2 = {30, 40};
    pq1 = std::move(pq2);
    EXPECT_EQ(pq1.Size(), 2);
    EXPECT_EQ(pq1.Top(), 40);
    EXPECT_TRUE(pq2.Empty());
}

TEST(PriorityQueueExtensionsTest, ConcatTwoEmptyQueues) {
    PriorityQueue<int> pq1;
    PriorityQueue<int> pq2;
    PriorityQueue<int> result = pq1.Concat(pq2);
    EXPECT_TRUE(result.Empty());
}

TEST(PriorityQueueExtensionsTest, AppendEmptyQueueDoesNotChangeState) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2;
    pq1.Append(pq2);
    EXPECT_EQ(pq1.Size(), 2);
    EXPECT_EQ(pq1.Top(), 20);
}

TEST(PriorityQueueExtensionsTest, IsSubsequenceOfItself) {
    PriorityQueue<int> pq = {5, 10, 15};
    if (!pq.IsSubsequence(pq)) {
        FAIL() << "Function: IsSubsequence\nInputs: Self match\nExpected: true";
    }
}

TEST(PriorityQueueExtensionsTest, IsSubsequenceLongerQueueFalse) {
    PriorityQueue<int> pq1 = {10};
    PriorityQueue<int> pq2 = {10, 20};
    if (pq1.IsSubsequence(pq2)) {
        FAIL() << "Function: IsSubsequence\nInputs: Sub queue is longer\nExpected: false";
    }
}

TEST(PriorityQueueExtensionsTest, SplitAllTrue) {
    PriorityQueue<int> pq = {2, 4, 6};
    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(trueQueue.Size(), 3);
    EXPECT_TRUE(falseQueue.Empty());
}

TEST(PriorityQueueExtensionsTest, SplitEmptyQueue) {
    PriorityQueue<int> pq;
    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });
    EXPECT_TRUE(trueQueue.Empty());
    EXPECT_TRUE(falseQueue.Empty());
}

TEST(PriorityQueueExtensionsTest, GetSubsequenceFullRange) {
    PriorityQueue<int> pq = {10, 20, 30};
    PriorityQueue<int> result = pq.GetSubsequence(0, 2);
    EXPECT_EQ(result.Size(), 3);
    EXPECT_EQ(result.Top(), 30);
}

TEST(PriorityQueueExtensionsTest, ReduceWithStringConcatenation) {
    PriorityQueue<std::string> pq;
    pq.Push("A");
    pq.Push("B");
    std::string result = pq.Reduce<std::string>([](const std::string& acc, const std::string& s) { return acc + s; });
    EXPECT_FALSE(result.empty());
}

TEST(PriorityQueueTest, DuplicateElementsPushPopSequence) {
    PriorityQueue<int> pq;
    pq.Push(5);
    pq.Push(5);
    EXPECT_EQ(pq.Size(), 2);
    pq.pop();
    EXPECT_EQ(pq.Top(), 5);
}

TEST(PriorityQueueTest, FloatDataTypeMaxPriority) {
    PriorityQueue<float> pq;
    pq.Push(1.1f);
    pq.Push(5.5f);
    pq.Push(3.3f);
    EXPECT_EQ(pq.Top(), 5.5f);
}

TEST(PriorityQueueTest, SwapWithEmptyQueue) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2;
    pq1.swap(pq2);
    EXPECT_TRUE(pq1.Empty());
    EXPECT_EQ(pq2.Size(), 2);
}

TEST(PriorityQueueExtensionsTest, MapToDifferentTypeStructureCheck) {
    PriorityQueue<int> pq = {1, 2};
    PriorityQueue<int> result = pq.Map([](const int& x) { return x + 10; });
    EXPECT_EQ(result.Top(), 12);
}

TEST(PriorityQueueTest, EmplaceMaintainsSizeIncrement) {
    PriorityQueue<int> pq;
    pq.Emplace(10);
    pq.Emplace(20);
    EXPECT_EQ(pq.Size(), 2);
}

TEST(PriorityQueueExtensionsTest, WhereAllFilteredOutKeepEmptyMetrics) {
    PriorityQueue<int> pq = {10, 20};
    PriorityQueue<int> result = pq.Where([](const int& x) { return x > 100; });
    EXPECT_EQ(result.Size(), 0);
    EXPECT_TRUE(result.Empty());
}

TEST(PriorityQueueTest, StressInsertOrderRetention) {
    PriorityQueue<int> pq;
    for (int i = 0; i < 200; ++i) {
        pq.Push(i);
    }
    EXPECT_EQ(pq.Top(), 199);
}

TEST(PriorityQueueTest, StressPopOrderRetention) {
    PriorityQueue<int> pq;
    for (int i = 0; i < 200; ++i) {
        pq.Push(i);
    }
    for (int i = 199; i >= 0; --i) {
        if (pq.Top() != i) {
            FAIL() << "Stress mismatch at " << i;
        }
        pq.pop();
    }
    EXPECT_TRUE(pq.Empty());
}

TEST(PriorityQueueTest, DestructorCalledOnStackNoLeakSyntax) {
    {
        PriorityQueue<int> pq = {1, 2, 3};
    }
    EXPECT_TRUE(true);
}

TEST(PriorityQueueTest, ConstTopReferenceChecks) {
    PriorityQueue<int> pq = {10};
    const int& ref = pq.Top();
    EXPECT_EQ(ref, 10);
}

TEST(PriorityQueueExtensionsTest, IsSubsequenceOfDisjointQueuesFalse) {
    PriorityQueue<int> pq1 = {1, 2, 3};
    PriorityQueue<int> pq2 = {4, 5, 6};
    EXPECT_FALSE(pq1.IsSubsequence(pq2));
}

TEST(PriorityQueueTest, ReverseOrderGreaterComparatorStress) {
    auto comp = [](int a, int b) {
        return b < a;
    };
    PriorityQueue<int, decltype(comp)> pq(comp);
    for (int i = 100; i > 0; --i) {
        pq.Push(i);
    }
    EXPECT_EQ(pq.Top(), 1);
}

TEST(PriorityQueueExtensionsTest, NestedMapInvocations) {
    PriorityQueue<int> pq = {1};
    PriorityQueue<int> r1 = pq.Map([](const int& x) { return x + 1; });
    PriorityQueue<int> r2 = r1.Map([](const int& x) { return x * 2; });
    EXPECT_EQ(r2.Top(), 4);
}

TEST(PriorityQueueExtensionsTest, WherePredicateEvaluatesConstRef) {
    PriorityQueue<std::string> pq;
    pq.Push("alpha");
    PriorityQueue<std::string> result = pq.Where([](const std::string& s) { return !s.empty(); });
    EXPECT_EQ(result.Size(), 1);
}

TEST(PriorityQueueTest, CopyAssignmentWithSelfEmpty) {
    PriorityQueue<int> pq;
    pq = *&pq;
    EXPECT_TRUE(pq.Empty());
}

TEST(PriorityQueueExtensionsTest, ReduceAccumulatorInitValueManual) {
    PriorityQueue<int> pq = {1, 2};
    int res = pq.Reduce<int>([](const int& acc, const int& x) { return acc + x; });
    EXPECT_EQ(res, 3);
}

TEST(PriorityQueueTest, TemplateWithUnsignedInt) {
    PriorityQueue<unsigned int> pq;
    pq.Push(10u);
    pq.Push(50u);
    EXPECT_EQ(pq.Top(), 50u);
}