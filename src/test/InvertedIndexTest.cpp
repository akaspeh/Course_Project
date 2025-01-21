//
// Created by PC on 16.01.2025.
//
#include "InvertedIndexTest.h"

namespace fs = std::filesystem;
// Global variables for tracking performance metrics
void stress_add_documents(InvertedIndex& index, const std::vector<std::pair<std::string, std::string>>& files, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        index.add_document(files[i].first, files[i].second);
    }
}

void stress_test_inverted_index(const std::string& files_directory, size_t file_amount, size_t thead_amount){
    InvertedIndex index;
    std::vector<std::pair<std::string, std::string>> files;
    for (const auto& entry : fs::recursive_directory_iterator(files_directory)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (file) {
                std::ostringstream content;
                content << file.rdbuf();
                files.emplace_back(entry.path().string(), content.str());
            }
        }
    }

    size_t total_files = file_amount;
    size_t num_threads = thead_amount; // Number of threads
    size_t files_per_thread = total_files / num_threads;

    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Create threads to add files to the index
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * files_per_thread;
        size_t end = (i == num_threads - 1) ? total_files : start + files_per_thread;

        threads.emplace_back(
                [&index, &files, start, end]() {
                    stress_add_documents(index, files, start, end);
                });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Added " << total_files << " files using " << num_threads << " threads in " << duration.count() << " seconds." << std::endl;
}


void test_add_and_search_document() {
    InvertedIndex index;

    // Add documents
    index.add_document("file1", "Hello world! Welcome to the inverted index.");
    index.add_document("file2", "Inverted index is a useful data structure.");
    index.add_document("file3", "Hello again, world!");

    // Search for terms
    auto result = index.search("hello");
    ASSERT_EQ(2, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());
    ASSERT_TRUE(result.find("file3") != result.end());

    result = index.search("world");
    ASSERT_EQ(2, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());
    ASSERT_TRUE(result.find("file3") != result.end());

    result = index.search("inverted");
    ASSERT_EQ(2, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());
    ASSERT_TRUE(result.find("file2") != result.end());

    result = index.search("nonexistent");
    ASSERT_EQ(0, result.size());
}


void test_add_and_multi_search_document() {
    InvertedIndex index;

    // Додаємо документи
    index.add_document("text1.txt", "dog cat log");
    index.add_document("text2.txt", "cat dog");
    index.add_document("text3.txt", "make dog");
    index.add_document("text4.txt", "text");

    // Пошук по двох термінах "dog" та "cat"
    auto result = index.search("dog cat");
    ASSERT_EQ(result.size(), 2); // Очікуємо, що буде 2 документи
    ASSERT_TRUE(result.find("text1.txt") != result.end());
    ASSERT_TRUE(result.find("text2.txt") != result.end());

    // Пошук по термінах "dog" та "log"
    result = index.search("dog log");
    ASSERT_EQ(result.size(), 1); // Очікуємо, що буде 1 документ
    ASSERT_TRUE(result.find("text1.txt") != result.end());

    // Пошук по термінах "cat" та "log"
    result = index.search("cat log");
    ASSERT_EQ(result.size(), 1); // Очікуємо, що буде 1 документ
    ASSERT_TRUE(result.find("text1.txt") != result.end());

    // Пошук по термінах "dog" та "make"
    result = index.search("dog make");
    ASSERT_EQ(result.size(), 1); // Очікуємо, що буде 1 документ
    ASSERT_TRUE(result.find("text3.txt") != result.end());

    // Пошук по термінах "dog" та "nonexistent"
    result = index.search("dog nonexistent");
    ASSERT_TRUE(result.empty()); // Очікуємо порожній результат, тому що термін "nonexistent" не знайдений

    // Пошук по термінах, яких немає
    result = index.search("nonexistent nonexistent");
    ASSERT_TRUE(result.empty()); // Очікуємо порожній результат
}

void test_remove_document() {
    InvertedIndex index;

    // Add documents
    index.add_document("file1", "Hello world! Welcome to the inverted index.");
    index.add_document("file2", "Inverted index is a useful data structure.");

    // Remove a document
    index.remove_document("file1", "Hello world! Welcome to the inverted index.");

    // Search after removal
    auto result = index.search("hello");
    ASSERT_EQ(0, result.size());

    result = index.search("inverted");
    ASSERT_EQ(1, result.size());
    ASSERT_TRUE(result.find("file2") != result.end());

    index.remove_document("file2", "Inverted index is a useful data structure.");
    result = index.search("inverted");
    ASSERT_EQ(0, result.size());
}

void test_tokenize_and_normalize_behavior() {
    InvertedIndex index;

    // Validate tokenization and normalization indirectly through `add_document`
    index.add_document("file1", "Hello, World! Welcome to 123.");
    auto result = index.search("hello");
    ASSERT_EQ(1, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());

    result = index.search("world");
    ASSERT_EQ(1, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());

    result = index.search("123");
    ASSERT_EQ(1, result.size());
    ASSERT_TRUE(result.find("file1") != result.end());
}

void test_empty_document_behavior() {
    InvertedIndex index;

    // Adding an empty document should not affect the index
    index.add_document("file1", "");
    auto result = index.search("anything");
    ASSERT_EQ(0, result.size());

    index.remove_document("file1", "");
    result = index.search("anything");
    ASSERT_EQ(0, result.size());
}