//
// Created by PC on 13.01.2025.
//

#ifndef COURSE_PROJECT_INVERTEDINDEX_H
#define COURSE_PROJECT_INVERTEDINDEX_H

#include <iostream>
#include <sstream>
#include <set>
#include "ThreadSafeHashMap.h"

class InvertedIndex {
private:
    std::vector<std::string> tokenize(const std::string& text);
    std::string normalize(const std::string& word);
    ThreadSafeHashMap<std::string, std::set<std::string>> m_hash_map;
public:
    explicit InvertedIndex(size_t num_of_threads = 8) :m_hash_map(num_of_threads){}
    void add_document(const std::string& file_name, const std::string& content);
    std::set<std::string> search(const std::string& term);
    void remove_document(const std::string& file_name, const std::string& content);
};


#endif //COURSE_PROJECT_INVERTEDINDEX_H
