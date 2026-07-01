#include <cstddef>
#include <gtest/gtest.h>
#include <initializer_list>
#include <memory>

#include "../include/sequence/Sequence/SeqCollection.hpp"
#include "../include/sequence/Sequence/ArraySequence.hpp"
#include "include/sequence/other/Exceptions.hpp"
#include "include/sequence/other/IEnumerator.hpp"

TEST(SeqCollectionTests, GetEnumeratorWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1, 2, 3, 4});
    SeqCollection<int> coll(seq);
    std::unique_ptr<IEnumerator<int>> enumerator = std::unique_ptr<IEnumerator<int>>(coll.GetEnumerator());

    enumerator->MoveNext();
    EXPECT_EQ(enumerator->Current(), 1) << "Проверка наличия первого элемента в {1}";

    delete seq;
}

TEST(SeqCollectionTests, CountWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1, 2, 3, 4});
    SeqCollection<int> coll(seq);

    size_t size = coll.Count();

    EXPECT_EQ(size, 4) << "Проверка размера {1, 2, 3, 4}";

    delete seq;
}

TEST(SeqCollectionTests, CountWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1, 2, 3, 4});
    SeqCollection<int> coll(seq, true);

    size_t size = coll.Count();

    EXPECT_EQ(size, 4) << "Проверка размера {1, 2, 3, 4} (объект не изменяет, всё нормально)";

    delete seq;
}

TEST(SeqCollectionTests, AddWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq);

    coll.Add(52);
    std::unique_ptr<IEnumerator<int>> enumerator = std::unique_ptr<IEnumerator<int>>(coll.GetEnumerator());

    enumerator->MoveNext();
    enumerator->MoveNext();
    EXPECT_EQ(enumerator->Current(), 52) << "Проверка наличия первого элемента в {1} после Add(52)";

    delete seq;
}

TEST(SeqCollectionTests, AddWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq, true);

    EXPECT_THROW(coll.Add(52), LogicError) << "Попытка добавить должна вызвать ошибку";

    delete seq;
}

TEST(SeqCollectionTests, IsReadOnlyWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq);

    EXPECT_FALSE(coll.IsReadOnly()) << "Создали коллекцию по умолчанию => не только для чтения";

    delete seq;
}

TEST(SeqCollectionTests, IsReadOnlyTrueInConstract) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq, true);

    EXPECT_TRUE(coll.IsReadOnly()) << "Создали коллекцию, указав, что только для чтения";

    delete seq;
}

TEST(SeqCollectionTests, ClearWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq);

    coll.Clear();
    std::unique_ptr<IEnumerator<int>> enumerator = std::unique_ptr<IEnumerator<int>>(coll.GetEnumerator());

    EXPECT_FALSE(enumerator->MoveNext()) << "MoveNext должен выдать false при конце последовательности, а у нас она пуста";

    delete seq;
}

TEST(SeqCollectionTests, ClearWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq, true);

    EXPECT_THROW(coll.Clear(), LogicError) << "Попытка очистить должна вызвать ошибку";

    delete seq;
}

TEST(SeqCollectionTests, ContainsWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq);

    EXPECT_TRUE(coll.Contains(1)) << "проверяем содержание 1 в {1}";

    delete seq;
}

TEST(SeqCollectionTests, ContainsWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq, true);

    EXPECT_TRUE(coll.Contains(1)) << "проверяем содержание 1 в {1} (объект не изменяет, всё нормально)";

    delete seq;
}

TEST(SeqCollectionTests, CopyToWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1, 2, 3, 4});
    SeqCollection<int> coll(seq);
    int* array = new int[4];

    coll.CopyTo(array);

    EXPECT_EQ(array[0], 1) << "проверяем содержание 1 в массиве, в который скопировали {1, 2, 3, 4}";

    delete seq;
}

TEST(SeqCollectionTests, CopyToWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1, 2, 3, 4});
    SeqCollection<int> coll(seq, true);
    int* array = new int[4];

    coll.CopyTo(array);

    EXPECT_EQ(array[0], 1) << "проверяем содержание 1 в массиве, в который скопировали {1, 2, 3, 4} (объект не изменяет, всё нормально)";

    delete seq;
}

TEST(SeqCollectionTests, RemoveWork) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq);

    EXPECT_TRUE(coll.Contains(1)) << "проверяем содержание 1 в {1}";
    EXPECT_TRUE(coll.Remove(1)) << "удаляем 1 из {1}, теперь {}";
    std::unique_ptr<IEnumerator<int>> enumerator = std::unique_ptr<IEnumerator<int>>(coll.GetEnumerator());

    EXPECT_FALSE(enumerator->MoveNext()) << "MoveNext должен выдать false при конце последовательности, а у нас она пуста";

    delete seq;
}

TEST(SeqCollectionTests, RemoveWithReadOnly) {
    MutableArraySequence<int>* seq = new MutableArraySequence<int>(std::initializer_list<int>{1});
    SeqCollection<int> coll(seq, true);

    EXPECT_THROW(coll.Remove(1), LogicError) << "Попытка удалить должна вызвать ошибку";

    delete seq;
}