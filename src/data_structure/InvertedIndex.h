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
    InvertedIndex() :m_hash_map(8){}
    void add_document(const std::string& file_name, const std::string& content);
    std::set<std::string> search(const std::string& term);
    void remove_document(const std::string& file_name, const std::string& content);
};


#endif //COURSE_PROJECT_INVERTEDINDEX_H
