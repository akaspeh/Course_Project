#include <iostream>
#include <random>
#include "./test/TestSuite.h"
#include "./test/TaskQueueTest.h"
#include "./test/InvertedIndexTest.h"
#include "./test/ThreadPoolTest.h"
#include "./test/ThreadSafeHashMapTest.h"


#ifdef _WIN32
#include "Server_win.h"
#else
#include "Server_linux.h"
#endif

int main(int argc, char* argv[]) {
    size_t thread_amount = 8;
    size_t port = 8080;
    if (argc == 2){size_t port =std::stoi(argv[1]);}
    else if (argc == 3){size_t thread_amount = std::stoi(argv[2]);}
    else if (argc > 3){std::cerr << "init default param." << std::endl;}
#ifdef _WIN32
    Server_win server(port, "src/storage/", thread_amount);
#else
    Server_linux server(port, "src/storage/", thread_amount);
#endif
    TestSuite suite;
    suite.add_test("test_add_and_search_document", test_add_and_search_document);
    suite.add_test("test_remove_document", test_remove_document);
    suite.add_test("test_tokenize_and_normalize_behavior", test_tokenize_and_normalize_behavior);
    suite.add_test("test_empty_document_behavior", test_empty_document_behavior);
    suite.add_test("test_task_queue_empty", test_task_queue_empty);
    suite.add_test("test_task_queue_size", test_task_queue_size);
    suite.add_test("test_task_queue_pop", test_task_queue_pop);
    suite.add_test("test_task_queue_clear", test_task_queue_clear);
    suite.add_test("test_task_queue_emplace", test_task_queue_emplace);
    suite.add_test("test_thread_pool_initialize", test_thread_pool_initialize);
    suite.add_test("test_thread_pool_terminate", test_thread_pool_terminate);
    suite.add_test("test_thread_pool_add_task", test_thread_pool_add_task);
    suite.add_test("test_thread_pool_add_multiple_tasks", test_thread_pool_add_multiple_tasks);
    suite.add_test("testInsertAndGet", testInsertAndGet);
    suite.add_test("testPop", testPop);
    suite.add_test("testThreadedInsert", testThreadedInsert);
    suite.add_test("testThreadedPop", testThreadedPop);
    suite.run();
    // Add tests to the test suite


    if (server.initialize()) {
        std::cout << "Server initialized." << std::endl;
        server.accept_connections();
    } else {
        std::cerr << "Server initialization failed.\n";
    }
    return 0;
}

