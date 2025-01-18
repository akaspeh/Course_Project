//
// Created by PC on 02.01.2025.
//

#ifndef COURSE_PROJECT_THREADSAFEHASHMAP_H
#define COURSE_PROJECT_THREADSAFEHASHMAP_H

#include <cassert>
#include <thread>
#include <memory>
#include <list>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <shared_mutex>
#include <algorithm>
#include <atomic>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;


template <typename key_t, typename value_t>
class ThreadSafeHashMap {
public:
    ThreadSafeHashMap(int32_t num_of_mutex = 4, int16_t a = 31, int16_t b = 7)
            : m_hashBuckets(num_of_mutex), m_locks(num_of_mutex), m_a(a), m_b(b) {}
    ~ThreadSafeHashMap() { clear(); }
public:
    void clear();
    bool pop(const key_t& key);
    template <typename... arguments>
    void emplace(const key_t &key, arguments&&... parameters);
    std::shared_ptr<value_t> get(const key_t& key);
    bool empty() const;
    size_t size() const;
private:
    void resize();
    size_t hash(const key_t& key);
public:
    ThreadSafeHashMap(const ThreadSafeHashMap& other) = delete;
    ThreadSafeHashMap(ThreadSafeHashMap&& other) = delete;
    ThreadSafeHashMap& operator=(const ThreadSafeHashMap& rhs) = delete;
    ThreadSafeHashMap& operator=(ThreadSafeHashMap&& rhs) = delete;
    std::vector<std::list<std::pair<key_t, value_t>>> m_hashBuckets;
private:
    std::vector<std::shared_mutex> m_locks;
    int16_t m_a, m_b;
    std::atomic<size_t> m_size = 0;
    std::atomic<size_t> m_process = 0;
    mutable read_write_lock m_rw_lock;
    float m_loadfactor_threshold = 2.0;
};

#include "ThreadSafeHashMap.tpp"

#endif //COURSE_PROJECT_THREADSAFEHASHMAP_H
