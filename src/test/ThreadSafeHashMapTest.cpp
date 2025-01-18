//
// Created by PC on 18.01.2025.
//

#include "ThreadSafeHashMapTest.h"
void testInsertAndGet() {
    ThreadSafeHashMap<int, std::string> map;

    // Вставляем элементы
    map.emplace(1, "one");
    map.emplace(2, "two");

    // Проверяем получение значений
    auto value1 = map.get(1);
    ASSERT_NE(value1, nullptr);
    ASSERT_EQ(*value1, "one");

    auto value2 = map.get(2);
    ASSERT_NE(value2, nullptr);
    ASSERT_EQ(*value2, "two");
}

void testPop() {
    ThreadSafeHashMap<int, std::string> map;

    // Вставляем элементы
    map.emplace(1, "one");
    map.emplace(2, "two");

    // Проверяем, что элементы существуют
    auto value1 = map.get(1);
    ASSERT_NE(value1, nullptr);
    auto value2 = map.get(2);
    ASSERT_NE(value2, nullptr);

    // Удаляем элемент
    bool removed = map.pop(1);
    ASSERT_TRUE(removed);

    // Проверяем, что элемент был удален
    value1 = map.get(1);
    ASSERT_EQ(value1, nullptr);
}

void testThreadedInsert() {
    ThreadSafeHashMap<int, std::string> map;

    auto insert_function = [&map](int start, int end) {
        for (int i = start; i < end; ++i) {
            map.emplace(i, "value_" + std::to_string(i));
        }
    };

    // Запускаем несколько потоков для вставки
    std::thread t1(insert_function, 0, 100);
    std::thread t2(insert_function, 100, 200);

    t1.join();
    t2.join();

    // Проверяем, что все элементы были вставлены
    for (int i = 0; i < 200; ++i) {
        auto value = map.get(i);
        ASSERT_NE(value, nullptr);
        ASSERT_EQ(*value, "value_" + std::to_string(i));
    }
}

void testThreadedPop() {
    ThreadSafeHashMap<int, std::string> map;

    // Вставляем элементы
    for (int i = 0; i < 100; ++i) {
        map.emplace(i, "value_" + std::to_string(i));
    }

    auto pop_function = [&map](int start, int end) {
        for (int i = start; i < end; ++i) {
            bool removed = map.pop(i);
            ASSERT_TRUE(removed);
        }
    };

    // Запускаем несколько потоков для удаления
    std::thread t1(pop_function, 0, 50);
    std::thread t2(pop_function, 50, 100);

    t1.join();
    t2.join();

    // Проверяем, что все элементы были удалены
    for (int i = 0; i < 100; ++i) {
        auto value = map.get(i);
        ASSERT_EQ(value, nullptr);
    }
}