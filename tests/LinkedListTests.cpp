#include <gtest/gtest.h>

#include "../include/sequence/Sequence/LinkedList.hpp"

TEST(LinkedListTests, PopBackWork) {
    LinkedList<int> list{1 ,2, 3, 4};

    list.PopBack();

    EXPECT_EQ(list.Get(0), 2) << "PopBack от {1, 2, 3, 4}";
}

TEST(LinkedListTests, PopBackTwice) {
    LinkedList<int> list{1 ,2, 3, 4};

    list.PopBack();
    list.PopBack();

    EXPECT_EQ(list.Get(0), 3) << "Два PopBack подрят от {1, 2, 3, 4}";
}

TEST(LinkedListTests, PopBackEmpty) {
    LinkedList<int> list{};

    try {
        list.PopBack();
    } catch(...) {
        EXPECT_TRUE(false) << "PopBack от пустого списка";
    }
    EXPECT_TRUE(true);
}