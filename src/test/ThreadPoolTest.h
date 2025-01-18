//
// Created by PC on 17.01.2025.
//

#ifndef COURSE_PROJECT_THREADPOOLTEST_H
#define COURSE_PROJECT_THREADPOOLTEST_H


#include <iostream>
#include <atomic>
#include <cassert>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include "../data_structure/ThreadPool.h"
#include "TestSuite.h"

void test_thread_pool_initialize();
void test_thread_pool_terminate();
void test_thread_pool_add_task();
void test_thread_pool_add_multiple_tasks();

#endif //COURSE_PROJECT_THREADPOOLTEST_H
