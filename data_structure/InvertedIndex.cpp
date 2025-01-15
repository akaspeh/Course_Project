//
// Created by PC on 13.01.2025.
//


#include "InvertedIndex.h"

std::vector<std::string> InvertedIndex::tokenize(const std::string& text){
    std::vector<std::string> tokens;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        tokens.push_back(normalize(word));
    }
    return tokens;
}
std::string InvertedIndex::normalize(const std::string& word){
    std::string result;
    for (char c : word) {
        if (std::isalnum(c)) { // Учитываем только буквы и цифры
            result += std::tolower(c);
        }
    }
    return result;
}
void InvertedIndex::add_document(const std::string& file_name, const std::string& content){
    std::vector<std::string> tokens = tokenize(content);
    for (const std::string& token : tokens) {
        m_hash_map.emplace(token, file_name);
    }
}
std::vector<int> InvertedIndex::search(const std::string& term){
    std::string normalized_term = normalize(term);
    m_hash_map.get(term);
    return {};
}