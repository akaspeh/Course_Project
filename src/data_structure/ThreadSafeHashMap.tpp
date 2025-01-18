
#ifndef THREAD_SAFE_HASH_MAP_TPP
#define THREAD_SAFE_HASH_MAP_TPP


#include "ThreadSafeHashMap.h"

template <typename key_t, typename value_t>
size_t ThreadSafeHashMap<key_t, value_t>::hash(const key_t& key){
    if constexpr (std::is_same<key_t, std::string>::value) {
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;  // A common string hashing method
        }
        return hash % 9149658775000477;
    } else {
        return ((m_a * key + m_b) % 9149658775000477);
    }
}

template <typename key_t, typename value_t>
void ThreadSafeHashMap<key_t, value_t>::resize(){

    size_t new_bucket_count = m_hashBuckets.size() * 2;
    std::vector<std::list<std::pair<key_t, value_t>>> new_buckets(new_bucket_count);

    for (auto& bucket : m_hashBuckets) {
        for (const auto& [key, value] : bucket) {
            size_t new_hashed_key = (hash(key) % new_bucket_count);
            new_buckets[new_hashed_key].emplace_back(key, value);
        }
    }
    // Replace old buckets with new buckets
    m_hashBuckets.swap(new_buckets);
}

template <typename key_t, typename value_t>
size_t ThreadSafeHashMap<key_t, value_t>::size() const{
    return m_size;
}

template <typename key_t, typename value_t>
bool ThreadSafeHashMap<key_t, value_t>::empty() const{
    return m_size.load() == 0;
}


template <typename key_t, typename value_t>
void ThreadSafeHashMap<key_t, value_t>::clear(){
    {
        write_lock _(m_rw_lock);
    }

    m_hashBuckets.clear();
    std::vector<std::list<std::pair<key_t, value_t>>> newBuckets(m_locks.size());
    // Swap the new vector with the current one.
    m_hashBuckets.swap(newBuckets);
    m_size.store(0);
}

template <typename key_t, typename value_t>
bool ThreadSafeHashMap<key_t, value_t>::pop(const key_t& key){
    {
        write_lock _(m_rw_lock);
        m_process++;
    }
    size_t hashed_key = hash(key) % m_hashBuckets.size();
    std::unique_lock<std::shared_mutex> lock(m_locks[hashed_key % m_locks.size()]);
    if (empty()) {
        m_process--;
        return false;
    }
    {
        auto& bucket = m_hashBuckets[hashed_key];
        auto it = std::find_if(
                bucket.begin(),
                bucket.end(),
                [&key](const std::pair<key_t, value_t>& pair) {
                    return pair.first == key;
                }
        );
        if (it != bucket.end()) {
            // Erase the item from the list if found
            bucket.erase(it);
            m_size--;
            m_process--;
            return true;  // Successfully removed
        }
    }
    m_process--;
    return false;
}

template <typename key_t, typename value_t>
std::shared_ptr<value_t> ThreadSafeHashMap<key_t, value_t>::get(const key_t& key){
    {
        read_lock _(m_rw_lock);
        m_process++;
    }
    size_t hashed_key = hash(key) % m_hashBuckets.size();
    std::shared_lock<std::shared_mutex> lock(m_locks[hashed_key % m_locks.size()]);
    // Lock the mutex corresponding to the bucket
    const auto& bucket = m_hashBuckets[hashed_key];

    // Find the key in the bucket
    auto it = std::find_if(bucket.begin(), bucket.end(),
                           [&key](const std::pair<key_t, value_t>& element) {
                               return element.first == key;
                           });

    // If the key is found, return a shared pointer to its value
    if (it != bucket.end()) {
        m_process--;
        return std::make_shared<value_t>(it->second);
    }
    m_process--;
    return nullptr;
}

template <typename key_t, typename value_t>
template <typename... arguments>
void ThreadSafeHashMap<key_t, value_t>::emplace(const key_t &key, arguments&&... parameters){
    {
        write_lock _(m_rw_lock);
        if (m_size / static_cast<double>(m_hashBuckets.size()) >= m_loadfactor_threshold) {
            while(m_process > 0){}
            resize();
        }
        m_process++;
    }
    size_t hashed_key = hash(key) % m_hashBuckets.size();
    std::unique_lock<std::shared_mutex> lock(m_locks[hashed_key % m_locks.size()]);
    // Lock the corresponding mutex for the selected bucket to ensure thread safety
    // Access the bucket list corresponding to the calculated index
    auto& bucket = m_hashBuckets[hashed_key];
    // Use emplace to insert the new value into the bucket (key-value pair)
    auto it = std::find_if(bucket.begin(), bucket.end(),
                           [&key](const std::pair<key_t, value_t>& element) {
                               return element.first == key;
                           });
    if (it != bucket.end()) {
        // Key exists: update the value (or handle as needed)
        it->second = value_t(std::forward<arguments>(parameters)...);
    } else {
        // Key doesn't exist: insert a new pair
        bucket.emplace_back(key, value_t(std::forward<arguments>(parameters)...));
        ++m_size;
    }
    m_process--;
}

#endif // THREAD_SAFE_HASH_MAP_TPP