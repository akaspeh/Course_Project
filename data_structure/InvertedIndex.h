//
// Created by PC on 13.01.2025.
//

#ifndef COURSE_PROJECT_INVERTEDINDEX_H
#define COURSE_PROJECT_INVERTEDINDEX_H

#include <sstream>
#include "ThreadSafeHashMap.h"

class InvertedIndex {
private:
    ThreadSafeHashMap<std::string, std::string> m_hash_map;
    std::vector<std::string> tokenize(const std::string& text);
    std::string normalize(const std::string& word);
public:
    void add_document(const std::string& file_name, const std::string& content);
    std::vector<int> search(const std::string& term);
};


#endif //COURSE_PROJECT_INVERTEDINDEX_H
