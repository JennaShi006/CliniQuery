#include "trieName.h"

void TrieName::insert(const std::string& name, const std::string& symptoms) {
    // Normalize the input: convert to lowercase
    std::string normalizedName = name;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);

    TrieNode* current = root;

    // Insert the first name
    size_t spacePos = normalizedName.find(' ');
    if (spacePos != std::string::npos) {
        std::string firstName = normalizedName.substr(0, spacePos);
        for (char c : firstName) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true; // Mark the end of the first name
        current->name = firstName;
        current->symptoms.insert(symptoms);

        // Reset current to root for inserting the full name
        current = root;
    }

    // Insert the full name
    for (char c : normalizedName) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true; // Mark the end of the full name
    current->name = normalizedName;
    current->symptoms.insert(symptoms);
}

bool TrieName::search(const std::string& word) {
    // Normalize the input: convert to lowercase
    std::string normalizedWord = word;
    std::transform(normalizedWord.begin(), normalizedWord.end(), normalizedWord.begin(), ::tolower);

    TrieNode* current = root;
    for (char c : normalizedWord) {
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            return false;
        }
        current = current->children[c];
    }
    return current->isEndOfWord;
}

bool TrieName::startsWith(const std::string& prefix) {
    // Normalize the input: convert to lowercase
    std::string normalizedPrefix = prefix;
    std::transform(normalizedPrefix.begin(), normalizedPrefix.end(), normalizedPrefix.begin(), ::tolower);
    
    TrieNode* current = root;
    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            return false;
        }
        current = current->children[c];
    }
    return true;
}

void TrieName::printPatients(const std::string& name){
    std::string normalizedName = name;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);
    TrieNode* current = root;
    for (char c : normalizedName) {
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            std::cout << "No patients found with the name: " << name << std::endl;
            return;
        }
        current = current->children[c];
    }
    for (const auto& symptom : current->symptoms) {
        std::cout << "Patient: " << current->name << ", Symptoms: " << symptom << std::endl;
    }
}