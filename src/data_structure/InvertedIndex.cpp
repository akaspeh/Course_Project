//
// Created by PC on 13.01.2025.
//


#include "InvertedIndex.h"

std::vector<std::string> InvertedIndex::splitString(const std::string& str)
 {
        std::vector<std::string> words;
        std::stringstream ss(str);
        std::string word;

        while (ss >> word) {
            words.push_back(word);
        }

        return words;
    }
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
        if (std::isalnum(c)) {
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

std::set<std::string> InvertedIndex::search(const std::string& str){
    std::vector<std::string> terms = splitString(str);

    std::map<std::string, std::map<std::string, int>> document_frequency;

    for (const auto& term : terms) {
        auto result = m_hash_map.get(normalize(term));
        if (result != nullptr) {
            for (const auto& document : *result) {
                document_frequency[document][term]++;
            }
        }
    }

    std::set<std::string> result_set;

    for (const auto& [document, frequencies] : document_frequency) {
        bool contains_all_terms = true;
        for (const auto& term : terms) {
            if (frequencies.find(term) == frequencies.end()) {
                contains_all_terms = false;
                break;
            }
        }

        if (contains_all_terms) {
            result_set.insert(document);
        }
    }
    return result_set;
}


