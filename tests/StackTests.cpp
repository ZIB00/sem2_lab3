#include <gtest/gtest.h>
#include "include/Stack.hpp"
#include "../include/sequence/Sequence/LinkedList.hpp"
#include <string>

// Конструкторы          

TEST(StackTest, DefaultConstructor_IsEmpty) {
    Stack<int> stack;

    EXPECT_TRUE(stack.IsEmpty()) << "Ввод: Stack<int>()\n";
}

TEST(StackTest, DefaultConstructor_SizeIsZero) {
    Stack<int> stack;

    EXPECT_EQ(stack.GetSize(), 0) << "Ввод: Stack<int>()\n";
}

TEST(StackTest, ConstructorFromInitializerList_SizeAndRootCorrect) {
    Stack<int> stack{1, 2, 3};

    EXPECT_EQ(stack.GetSize(), 3) << "Ввод: Stack{1, 2, 3}\n";
    EXPECT_EQ(stack.GetRoot(), 1) << "Ввод: Stack{1, 2, 3}\n";
}

TEST(StackTest, ConstructorFromLinkedList_SizeMatchesInput) {
    LinkedList<int> list{1, 2, 3};

    Stack<int> stack(list);

    EXPECT_EQ(stack.GetSize(), 3) << "Ввод: Stack из LinkedList {1, 2, 3}\n";
}

TEST(StackTest, ConstructorFromRvalueLinkedList_SourceSizeIsZero) {
    LinkedList<int> list{1, 2};

    Stack<int> stack(std::move(list));

    EXPECT_EQ(list.GetLength(), 0)
        << "Ввод: LinkedList {1, 2} передан через std::move\n"
        << "Ожидалось: list.GetLength() == 0 (данные перемещены)\n";
}

TEST(StackTest, CopyConstructor_RootMatchesOriginal) {
    Stack<int> original{10, 20};

    Stack<int> copy(original);

    EXPECT_EQ(copy.GetRoot(), original.GetRoot()) << "Ввод: копия Stack{10,20}\n";
}

TEST(StackTest, CopyConstructor_IsDeepCopy) {
    Stack<int> original{10};
    Stack<int> copy(original);

    copy.Push(99);

    EXPECT_EQ(original.GetSize(), 1)
        << "Ввод: Push(99) в копию Stack{10}\n"
        << "Ожидалось: оригинал не изменился, GetSize() == 1\n";
}

TEST(StackTest, MoveConstructor_DestinationHasData) {
    Stack<int> original{42, 24};

    Stack<int> moved(std::move(original));

    EXPECT_EQ(moved.GetRoot(), 42) << "Ввод: std::move(Stack{42,24})\n";
    EXPECT_EQ(moved.GetSize(), 2) << "Ввод: std::move(Stack{42,24})\n";
}

TEST(StackTest, MoveConstructor_SourceIsEmpty) {
    Stack<int> original{42};

    Stack<int> moved(std::move(original));

    EXPECT_TRUE(original.IsEmpty())
        << "Ввод: std::move(Stack{42})\n"
        << "Ожидалось: источник IsEmpty() == true (данные перемещены и очищены)\n";
}

// Оператор присваивания 

TEST(StackTest, Copy_RootMatchesSource) {
    Stack<int> original{10, 20};
    Stack<int> assigned;

    assigned = original;

    EXPECT_EQ(assigned.GetRoot(), original.GetRoot()) << "Ввод: assigned = Stack{10,20}\n";
    EXPECT_EQ(assigned.GetSize(), 2) << "Ввод: assigned = Stack{10,20}\n";
}

TEST(StackTest, Copy_Self_DataIntact) {
    Stack<int> stack{5, 15};

    stack = *&stack;

    EXPECT_EQ(stack.GetSize(), 2) << "Ввод: stack = stack (самоприсваивание), Stack{5,15}\n";
}

TEST(StackTest, Move_DestinationHasData) {
    Stack<int> src{10, 50};
    Stack<int> dest;

    dest = std::move(src);

    EXPECT_EQ(dest.GetRoot(), 10) << "Ввод: dest = std::move(Stack{10,50})\n";
    EXPECT_EQ(dest.GetSize(), 2) << "Ввод: dest = std::move(Stack{10,50})\n";
}

TEST(StackTest, Move_SourceIsEmpty) {
    Stack<int> src{10};
    Stack<int> dest;

    dest = std::move(src);

    EXPECT_TRUE(src.IsEmpty())
        << "Ввод: dest = std::move(Stack{10})\n"
        << "Ожидалось: источник IsEmpty() == true (данные перемещены)\n";
}

TEST(StackTest, Move_Self_DataIntact) {
    Stack<int> stack{5, 15};

    stack = std::move(stack);

    EXPECT_EQ(stack.GetSize(), 2) << "Ввод: stack = std::move(stack) (самоприсваивание), Stack{5,15}\n";
}

TEST(StackTest, Copy_Chained) {
    Stack<int> stack1{5};
    Stack<int> stack2;
    Stack<int> stack3;

    stack3 = stack2 = stack1;

    EXPECT_EQ(stack3.GetRoot(), 5) << "Ввод: stack3 = stack2 = Stack{5}\n";
}

// Push      

TEST(StackTest, Push_SingleElement_RootIsElement) {
    Stack<int> stack;

    stack.Push(5);

    EXPECT_EQ(stack.GetRoot(), 5) << "Ввод: Push(5) в пустой стек\n";
}

TEST(StackTest, Push_MultipleElements_SizeIncrementsCorrectly) {
    Stack<int> stack{5, 3};

    stack.Push(8);

    EXPECT_EQ(stack.GetSize(), 3) << "Ввод: Изначально Stack{5, 3}, затем Push(8)\n";
}

TEST(StackTest, Push_MultipleElements_RootRemainsFirstPushed) {
    Stack<int> stack{5};
    
    stack.Push(10);

    EXPECT_EQ(stack.GetRoot(), 10)
        << "Ввод: Stack{5}, затем Push(10)\n";
}

// Pop       

TEST(StackTest, Pop_DecrementsSizeByOne) {
    Stack<int> stack{10, 20, 30};

    stack.Pop();

    EXPECT_EQ(stack.GetSize(), 2) << "Ввод: Pop() из Stack{10,20,30}\n";
}

TEST(StackTest, Pop_RemovesLastPushedElement) {
    Stack<int> stack{10, 20, 30};

    stack.Pop(); 
    stack.Pop(); 

    EXPECT_EQ(stack.GetRoot(), 30) << "Ввод: Pop() дважды из Stack{10,20,30}\n";
    EXPECT_EQ(stack.GetSize(), 1) << "Ввод: Pop() дважды из Stack{10,20,30}\n";
}

TEST(StackTest, Pop_AllElements_StackIsEmpty) {
    Stack<int> stack{10, 20, 30};

    stack.Pop();
    stack.Pop();
    stack.Pop();

    EXPECT_TRUE(stack.IsEmpty()) << "Ввод: Pop() трижды из Stack{10,20,30}\n";
}

TEST(StackTest, Pop_SingleElement_StackIsEmpty) {
    Stack<int> stack{100};

    stack.Pop();

    EXPECT_TRUE(stack.IsEmpty()) << "Ввод: Pop() из Stack{100}\n";
}