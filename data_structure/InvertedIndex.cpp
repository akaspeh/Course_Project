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
void InvertedIndex::add_document(const std::string& file_name, const std::string& content) {
    std::vector<std::string> tokens = tokenize(content);
    for (const std::string& token : tokens) {
        auto file_list = m_hash_map.get(token); // Получаем ссылку на список файлов
        if (file_list == nullptr) {
            // If no set exists for this token, create a new set
            auto new_set = std::set<std::string>();
            new_set.insert(file_name); // Add the file name to the set
            m_hash_map.emplace(token, new_set);
        } else {
            // If the set exists, add the file name to it
            file_list->insert(file_name);
            m_hash_map.emplace(token, *file_list);
        }
    }

}

void InvertedIndex::remove_document(const std::string& file_name, const std::string& content) {
    std::vector<std::string> tokens = tokenize(content);
    for (const std::string& token : tokens) {
        auto file_list = m_hash_map.get(token); // Get the set associated with the token
        if (file_list != nullptr) {
            // Remove the file name from the set
            file_list->erase(file_name);

            // If the set becomes empty, remove the token from the map
            if (file_list->empty()) {
                m_hash_map.pop(token); // Use pop or equivalent to erase the token from the map
            } else {
                m_hash_map.emplace(token, *file_list); // Update the set in the hash map
            }
        }
    }
}

std::set<std::string> InvertedIndex::search(const std::string& term){
    auto result = m_hash_map.get(normalize(term));
    if (result != nullptr) {
        // Dereference the shared_ptr to get the actual set
        return *result;
    }
    // Return an empty set if the term is not found
    return {};
}