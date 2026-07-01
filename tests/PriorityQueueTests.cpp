#include <gtest/gtest.h>
#include "include/PriorityQueue.hpp"
#include "include/sequence/Sequence/DynamicArray.hpp"
#include <string>

template<typename T, typename Comp>
DynamicArray<T> DumpQueue(PriorityQueue<T, Comp> q) {
    DynamicArray<T> result(q.Size());
    size_t idx = 0;
    while (!q.Empty()) {
        result.Set(idx++, q.Top());
        q.pop();
    }
    return result;
}

// Конструкторы

TEST(PriorityQueueTest, DefaultConstructor_IsEmpty) {
     PriorityQueue<int> pq;

    EXPECT_TRUE(pq.Empty()) << "Ввод: PriorityQueue<int>()\n";
}

TEST(PriorityQueueTest, DefaultConstructor_SizeIsZero) {
     PriorityQueue<int> pq;

    EXPECT_EQ(pq.Size(), 0) << "Ввод: PriorityQueue<int>()\n";
}

TEST(PriorityQueueTest, InitializerListConstructor_TopIsMaxElement) {
     PriorityQueue<int> pq = {10, 30, 20, 5, 40};

    EXPECT_EQ(pq.Top(), 40) << "Ввод: {10, 30, 20, 5, 40}\n";
}

TEST(PriorityQueueTest, InitializerListConstructor_SizeMatchesInput) {
     PriorityQueue<int> pq = {10, 30, 20, 5, 40};

    EXPECT_EQ(pq.Size(), 5) << "Ввод: {10, 30, 20, 5, 40}\n";
}

TEST(PriorityQueueTest, ConstructorFromConstArray_TopIsMaxElement) {
    int raw[] = {10, 30, 20, 5, 40};
    DynamicArray<int> arr(raw, 5);

    PriorityQueue<int> pq(std::less<int>(), arr);

    EXPECT_EQ(pq.Top(), 40) << "Ввод: {10,30,20,5,40}\n";
}

TEST(PriorityQueueTest, ConstructorFromRvalueArray_TopIsMaxElement) {
    int raw[] = {7, 8, 9};
    DynamicArray<int> arr(raw, 3);

    PriorityQueue<int> pq(std::less<int>(), std::move(arr));

    EXPECT_EQ(pq.Top(), 9) << "Ввод: {7,8,9} (rvalue)\n";
}

TEST(PriorityQueueTest, ConstructorFromRvalueArray_SourceIsEmpty) {
    int raw[] = {7, 8, 9};
    DynamicArray<int> arr(raw, 3);

    PriorityQueue<int> pq(std::less<int>(), std::move(arr));

    EXPECT_EQ(arr.GetSize(), 0) << "Ввод: {7,8,9} передан через std::move\n";
}

TEST(PriorityQueueTest, CopyConstructor_TopMatchesOriginal) {
    PriorityQueue<int> original = {100, 500};

    PriorityQueue<int> copy(original);

    EXPECT_EQ(copy.Top(), original.Top()) << "Ввод: копия PriorityQueue{100, 500}\n";
}

TEST(PriorityQueueTest, CopyConstructor_IsDeepCopy) {
    PriorityQueue<int> original = {100, 500};
    PriorityQueue<int> copy(original);

    copy.pop();

    EXPECT_EQ(original.Top(), 500) << "Ввод: pop() в копии PriorityQueue{100,500}\n";
}

TEST(PriorityQueueTest, MoveConstructor_DestinationHasData) {
    PriorityQueue<int> original = {42};

    PriorityQueue<int> moved(std::move(original));

    EXPECT_EQ(moved.Top(), 42) << "Ввод: std::move(PriorityQueue{42})\n";
}

TEST(PriorityQueueTest, MoveConstructor_SourceIsEmpty) {
    PriorityQueue<int> original = {42};

    PriorityQueue<int> moved(std::move(original));

    EXPECT_TRUE(original.Empty()) << "Ввод: std::move(PriorityQueue{42})\n";
}

// Оператор присваивания   

TEST(PriorityQueueTest, CopyAssignment_TopMatchesSource) {
    PriorityQueue<int> original = {10, 20};
    PriorityQueue<int> assigned;

    assigned = original;

    EXPECT_EQ(assigned.Top(), 20) << "Ввод: assigned = PriorityQueue{10,20}\n";
}

TEST(PriorityQueueTest, MoveAssignment_DestinationHasData) {
    PriorityQueue<int> src = {30, 40};
    PriorityQueue<int> dest = {10};

    dest = std::move(src);

    EXPECT_EQ(dest.Top(), 40) << "Ввод: dest = std::move(PriorityQueue{30,40})\n";
}

TEST(PriorityQueueTest, MoveAssignment_SourceIsEmpty) {
    PriorityQueue<int> src = {30, 40};
    PriorityQueue<int> dest;

    dest = std::move(src);

    EXPECT_TRUE(src.Empty()) << "Ввод: dest = std::move(PriorityQueue{30,40})\n";
}

// Top         

TEST(PriorityQueueTest, Top_ReturnsConstRef_ValueIsCorrect) {
    PriorityQueue<int> pq = {10};

    const int& ref = pq.Top();

    EXPECT_EQ(ref, 10) << "Ввод: Top() из PriorityQueue{10}\n";
}

// Push        

TEST(PriorityQueueTest, Push_NewMax_UpdatesTop) {
    PriorityQueue<int> pq;
    pq.Push(10);

    pq.Push(20);

    EXPECT_EQ(pq.Top(), 20) << "Ввод: Push(10), Push(20)\n";
}

TEST(PriorityQueueTest, Push_SmallerValue_TopUnchanged) {
    PriorityQueue<int> pq;
    pq.Push(20);

    pq.Push(5);

    EXPECT_EQ(pq.Top(), 20) << "Ввод: Push(20), Push(5)\n";
}

TEST(PriorityQueueTest, Push_Rvalue_SourceIsEmpty) {
    PriorityQueue<std::string> pq;
    std::string text = "rvalue_test";

    pq.Push(std::move(text));

    EXPECT_TRUE(text.empty()) << "Ввод: Push(std::move(\"rvalue_test\"))\n";
}

TEST(PriorityQueueTest, Push_Rvalue_TopHasCorrectValue) {
    PriorityQueue<std::string> pq;
    std::string text = "rvalue_test";

    pq.Push(std::move(text));

    EXPECT_EQ(pq.Top(), "rvalue_test") << "Ввод: Push(std::move(\"rvalue_test\"))\n";
}

// pop         

TEST(PriorityQueueTest, Pop_RemovesTop_NextMaxBecomesTop) {
    PriorityQueue<int> pq = {5, 1, 9, 3};

    pq.pop();

    EXPECT_EQ(pq.Top(), 5) << "Ввод: pop() из PriorityQueue{1,3,5,9}\n";
}

TEST(PriorityQueueTest, Pop_AllElements_IsEmpty) {
    PriorityQueue<int> pq = {1, 2, 3};

    pq.pop();
    pq.pop();
    pq.pop();

    EXPECT_TRUE(pq.Empty()) << "Ввод: pop() трижды из PriorityQueue{1,2,3}\n";
}

// Кастомный компаратор    

TEST(PriorityQueueTest, GreaterComparator_TopIsMinElement) {
    auto comp = [](int a, int b) { return b < a; };
    PriorityQueue<int, decltype(comp)> min_pq(comp);

    min_pq.Push(50);
    min_pq.Push(10);
    min_pq.Push(30);

    EXPECT_EQ(min_pq.Top(), 10) << "Ввод: std::greater<int>, Push {50,10,30}\n";
}

TEST(PriorityQueueTest, GreaterComparator_PopRevealsNextMin) {
    auto comp = [](int a, int b) { return b < a; };
    PriorityQueue<int, decltype(comp)> min_pq(comp);
    min_pq.Push(50);
    min_pq.Push(10);
    min_pq.Push(30);

    min_pq.pop();

    EXPECT_EQ(min_pq.Top(), 30) << "Ввод: pop() из min-PriorityQueue{10,30,50}\n";
}

// Map         

TEST(PriorityQueueExtensionsTest, Map_TransformsValues_TopIsNewMax) {
    PriorityQueue<int> pq = {1, 3, 2};

    PriorityQueue<int> result = pq.Map<int>([](const int& x) { return x * 2; });

    EXPECT_EQ(result.Top(), 6) << "Ввод: Map(x*2) на PriorityQueue{1,2,3}\n";
}

TEST(PriorityQueueExtensionsTest, Map_PreservesSize) {
    PriorityQueue<int> pq = {1, 3, 2};

    PriorityQueue<int> result = pq.Map<int>([](const int& x) { return x * 2; });

    EXPECT_EQ(result.Size(), 3) << "Ввод: Map(x*2) на PriorityQueue{1,2,3}\n";
}

TEST(PriorityQueueExtensionsTest, Map_EmptyQueue_ResultIsEmpty) {
    PriorityQueue<int> pq;

    PriorityQueue<int> result = pq.Map<int>([](const int& x) { return x * 2; });

    EXPECT_TRUE(result.Empty()) << "Ввод: Map(x*2) на пустой очереди\n";
}

TEST(PriorityQueueExtensionsTest, Map_DoesNotModifyOriginal) {
    PriorityQueue<int> pq = {1, 3, 2};

    pq.Map<int>([](const int& x) { return x * 2; });

    EXPECT_EQ(pq.Top(), 3) << "Ввод: Map(x*2) на PriorityQueue{1,2,3}\n";
}

TEST(PriorityQueueExtensionsTest, Map_NestedCalls_ResultIsCorrect) {
    PriorityQueue<int> pq = {1};
    PriorityQueue<int> r1 = pq.Map<int>([](const int& x) { return x + 1; });

    PriorityQueue<int> r2 = r1.Map<int>([](const int& x) { return x * 2; });

    EXPECT_EQ(r2.Top(), 4) << "Ввод: Map(x+1) затем Map(x*2) на PriorityQueue{1}\n";
}

// Where       

TEST(PriorityQueueExtensionsTest, Where_FiltersCorrectly_TopIsMaxOfFiltered) {
    PriorityQueue<int> pq = {1, 4, 3, 6, 5};

    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(result.Top(), 6) << "Ввод: Where(x%2==0) на PriorityQueue{1,3,4,5,6}\n";
}

TEST(PriorityQueueExtensionsTest, Where_FiltersCorrectly_SizeIsCorrect) {
    PriorityQueue<int> pq = {1, 4, 3, 6, 5};

    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(result.Size(), 2) << "Ввод: Where(x%2==0) на PriorityQueue{1,3,4,5,6}\n";
}

TEST(PriorityQueueExtensionsTest, Where_NoMatches_ResultIsEmpty) {
    PriorityQueue<int> pq = {1, 3, 5};

    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });

    EXPECT_TRUE(result.Empty()) << "Ввод: Where(x%2==0) на PriorityQueue{1,3,5}\n";
}

TEST(PriorityQueueExtensionsTest, Where_AllMatch_SizeUnchanged) {
    PriorityQueue<int> pq = {2, 4, 6};

    PriorityQueue<int> result = pq.Where([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(result.Size(), 3) << "Ввод: Where(x%2==0) на PriorityQueue{2,4,6} (все чётные)\n";
}

// Reduce      

TEST(PriorityQueueExtensionsTest, Reduce_Sum_CorrectResult) {
    PriorityQueue<int> pq = {10, 20, 30};

    int sum = pq.Reduce<int>([](const int& acc, const int& x) { return acc + x; });

    EXPECT_EQ(sum, 60) << "Ввод: Reduce(acc+x) на PriorityQueue{10,20,30}\n";
}

TEST(PriorityQueueExtensionsTest, Reduce_EmptyQueue_ReturnsDefaultValue) {
    PriorityQueue<int> pq;

    int sum = pq.Reduce<int>([](const int& acc, const int& x) { return acc + x; });

    EXPECT_EQ(sum, 0) << "Ввод: Reduce(acc+x) на пустой очереди\n";
}

// Concat      

TEST(PriorityQueueExtensionsTest, Concat_TwoQueues_TopIsOverallMax) {
    PriorityQueue<int> pq1 = {10, 30};
    PriorityQueue<int> pq2 = {40, 20};

    PriorityQueue<int> result = pq1.Concat(pq2);

    EXPECT_EQ(result.Top(), 40) << "Ввод: Concat PriorityQueue{10,30} и PriorityQueue{20,40}\n";
}

TEST(PriorityQueueExtensionsTest, Concat_TwoQueues_SizeIsSumOfBoth) {
    PriorityQueue<int> pq1 = {10, 30};
    PriorityQueue<int> pq2 = {40, 20};

    PriorityQueue<int> result = pq1.Concat(pq2);

    EXPECT_EQ(result.Size(), 4) << "Ввод: Concat PriorityQueue{10,30} и PriorityQueue{20,40}\n";
}

TEST(PriorityQueueExtensionsTest, Concat_DoesNotModifyOriginals) {
    PriorityQueue<int> pq1 = {10, 30};
    PriorityQueue<int> pq2 = {40, 20};

    pq1.Concat(pq2);

    EXPECT_EQ(pq1.Size(), 2) << "Ввод: Concat не должен изменять исходные очереди\n";
}

TEST(PriorityQueueExtensionsTest, Concat_WithEmpty_SizeUnchanged) {
    PriorityQueue<int> pq1 = {5, 15};
    PriorityQueue<int> pq2;

    PriorityQueue<int> result = pq1.Concat(pq2);

    EXPECT_EQ(result.Size(), 2) << "Ввод: Concat PriorityQueue{5,15} и пустой\n";
}

TEST(PriorityQueueExtensionsTest, Concat_TwoEmpty_ResultIsEmpty) {
    PriorityQueue<int> pq1;
    PriorityQueue<int> pq2;

    PriorityQueue<int> result = pq1.Concat(pq2);

    EXPECT_TRUE(result.Empty()) << "Ввод: Concat двух пустых очередей\n";
}

// Append      

TEST(PriorityQueueExtensionsTest, Append_AddsAllElements_SizeIsSum) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2 = {30, 40};

    pq1.Append(pq2);

    EXPECT_EQ(pq1.Size(), 4) << "Ввод: Append PriorityQueue{30,40} к PriorityQueue{10,20}\n";
}

TEST(PriorityQueueExtensionsTest, Append_NewMaxElement_UpdatesTop) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2 = {30, 40};

    pq1.Append(pq2);

    EXPECT_EQ(pq1.Top(), 40) << "Ввод: Append PriorityQueue{30,40} к PriorityQueue{10,20}\n";
}

TEST(PriorityQueueExtensionsTest, Append_EmptyQueue_NoChange) {
    PriorityQueue<int> pq1 = {10, 20};
    PriorityQueue<int> pq2;

    pq1.Append(pq2);

    EXPECT_EQ(pq1.Size(), 2) << "Ввод: Append пустой очереди к PriorityQueue{10,20}\n";
}

// GetSubsequence          

TEST(PriorityQueueExtensionsTest, GetSubsequence_FullRange_SizeMatchesOriginal) {
    PriorityQueue<int> pq = {10, 20, 30};

    PriorityQueue<int> result = pq.GetSubsequence(0, 2);

    EXPECT_EQ(result.Size(), 3) << "Ввод: GetSubsequence(0, 2) из PriorityQueue{10,20,30}\n";
}

TEST(PriorityQueueExtensionsTest, GetSubsequence_PartialRange_SizeIsCorrect) {
    PriorityQueue<int> pq = {10, 20, 30, 40};

    PriorityQueue<int> result = pq.GetSubsequence(1, 2);

    EXPECT_EQ(result.Size(), 2) << "Ввод: GetSubsequence(1, 2) из PriorityQueue{10,20,30,40}\n";
}

TEST(PriorityQueueExtensionsTest, GetSubsequence_InvalidArgument) {
    PriorityQueue<int> pq = {10, 20};

    EXPECT_THROW(pq.GetSubsequence(5, 10), InvalidArgument) << "Ввод: GetSubsequence(5, 10) из PriorityQueue{10,20} (индексы за пределами)\n";
}

// IsSubsequence           

TEST(PriorityQueueExtensionsTest, IsSubsequence_Itself_ReturnsTrue) {
    PriorityQueue<int> pq = {5, 10, 15};

    bool result = pq.IsSubsequence(pq);

    EXPECT_TRUE(result) << "Ввод: IsSubsequence(pq, pq) — очередь сама является своей подпоследовательностью\n";
}

TEST(PriorityQueueExtensionsTest, IsSubsequence_SubsetIsContained_ReturnsTrue) {
    PriorityQueue<int> pq = {5, 10, 15};
    PriorityQueue<int> sub = {10};

    bool result = pq.IsSubsequence(sub);

    EXPECT_TRUE(result) << "Ввод: IsSubsequence PriorityQueue{5,10,15} содержит {10}\n";
}

TEST(PriorityQueueExtensionsTest, IsSubsequence_LongerSubQueue_ReturnsFalse) {
    PriorityQueue<int> pq = {10};
    PriorityQueue<int> sub = {10, 20};

    bool result = pq.IsSubsequence(sub);

    EXPECT_FALSE(result) << "Ввод: IsSubsequence PriorityQueue{10} содержит {10,20}?\n";
}

TEST(PriorityQueueExtensionsTest, IsSubsequence_DisjointQueues_ReturnsFalse) {
    PriorityQueue<int> pq1 = {1, 2, 3};
    PriorityQueue<int> pq2 = {4, 5, 6};

    bool result = pq1.IsSubsequence(pq2);

    EXPECT_FALSE(result) << "Ввод: IsSubsequence PriorityQueue{1,2,3} содержит {4,5,6}?\n";
}

// Split       

TEST(PriorityQueueExtensionsTest, Split_TrueQueueContainsMatchingElements) {
    PriorityQueue<int> pq = {1, 2, 3, 4, 5};

    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(trueQueue.Size(), 2) << "Ввод: Split(x%2==0) на PriorityQueue{1,2,3,4,5}\n";
}

TEST(PriorityQueueExtensionsTest, Split_FalseQueueContainsNonMatchingElements) {
    PriorityQueue<int> pq = {1, 2, 3, 4, 5};

    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(falseQueue.Size(), 3) << "Ввод: Split(x%2==0) на PriorityQueue{1,2,3,4,5}\n";
}

TEST(PriorityQueueExtensionsTest, Split_AllTrue_FalseQueueIsEmpty) {
    PriorityQueue<int> pq = {2, 4, 6};

    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });

    EXPECT_TRUE(falseQueue.Empty()) << "Ввод: Split(x%2==0) на PriorityQueue{2,4,6} (все чётные)\n";
}

TEST(PriorityQueueExtensionsTest, Split_EmptyQueue_BothResultsAreEmpty) {
    PriorityQueue<int> pq;

    auto [trueQueue, falseQueue] = pq.Split([](const int& x) { return x % 2 == 0; });

    EXPECT_TRUE(trueQueue.Empty() && falseQueue.Empty()) << "Ввод: Split на пустой очереди\n";
}