//
// Created by PC on 16.01.2025.
//

#ifndef COURSE_PROJECT_INVERTEDINDEXTEST_H
#define COURSE_PROJECT_INVERTEDINDEXTEST_H
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <random>
#include <fstream>
#include <mutex>
#include <atomic>
#include <set>

#include "../data_structure/InvertedIndex.h"
#include "TestSuite.h"


void test_add_and_search_document();
void test_remove_document();
void test_tokenize_and_normalize_behavior();
void test_empty_document_behavior();
void stress_test_inverted_index(const std::string& files_directory, size_t file_amount, size_t thead_amount);

#endif //COURSE_PROJECT_INVERTEDINDEXTEST_H
