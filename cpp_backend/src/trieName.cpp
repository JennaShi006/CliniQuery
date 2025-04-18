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

unordered_set<string> TrieName::search(const std::string& name) {
    // Normalize the input: convert to lowercase
    std::string normalizedWord = name;
    std::transform(normalizedWord.begin(), normalizedWord.end(), normalizedWord.begin(), ::tolower);

    TrieNode* current = root;
    for (char c : normalizedWord) {
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            return {};
        }
        current = current->children[c];
    }
    return current->symptoms; // Return the symptoms associated with the name
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

void TrieName::printSymptoms(const string& name) {
    // Normalize the input: convert to lowercase
    std::string normalizedName = name;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);

    unordered_set<string> symptoms = search(normalizedName);
    if (symptoms.empty()) {
        std::cout << "No symptoms found for the name: " << name << std::endl;
        return;
    }

    for (const auto& symptom : symptoms) {
        std::cout << "Patient: " << name << ", Symptoms: ";
        //symptom << std::endl;
        for (int i=0; i<symptom.length(); i++){
            if(symptom[i] == '1'){
                std::cout << symptomsList[i] << " | ";
            }
            
        }
        std::cout << std::endl;
    }
}