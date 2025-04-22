#include "trieName.h"
#include <chrono>
void TrieName::insert(const std::string& name, const std::string& symptoms) {
    // Normalize the input: convert to lowercase
    std::string normalizedName = name;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);

    TrieNode* current = root;

    // Insert the first name
    size_t spacePos = normalizedName.find(' ');
    if (spacePos != std::string::npos) {
        std::string firstName = normalizedName.substr(0, spacePos);
        std::string lastName = normalizedName.substr(spacePos + 1);
        for (char c : firstName) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true; // Mark the end of the first name
        current->name = firstName;
        current->symptoms.insert(symptoms);
        current->lastNameToSymptoms[lastName].insert(symptoms);
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

unordered_map<string, unordered_set<string>> TrieName::search(const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();
    // Normalize the input: convert to lowercase
    unordered_map<string, unordered_set<string>> result;
    std::string normalizedWord = name;
    std::transform(normalizedWord.begin(), normalizedWord.end(), normalizedWord.begin(), ::tolower);
    size_t spacePos = normalizedWord.find(' ');
    if (spacePos == std::string::npos) {
        TrieNode* current = root;
        // If no space is found
        for (char c : normalizedWord) {
            if (root->children.find(c) == root->children.end()) {
                // If the character is not present, return an empty map
                auto end = std::chrono::high_resolution_clock::now();
                runtime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                return {};
            }
            current = current->children[c];
        }
        if (current->isEndOfWord) {
            // If the word is found, return the symptoms
            for (const auto& [lastName, symptoms] : current->lastNameToSymptoms) {
                result[lastName] = symptoms;
            }
        }
    }
    else{
        TrieNode* current = root;
        for (char c : normalizedWord) {
            if (current->children.find(c) == current->children.end()) {
                // If the character is not present, return an empty map
                auto end = std::chrono::high_resolution_clock::now();
                runtime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                return {};
            }
            current = current->children[c];
        }
        if (current->isEndOfWord) {
            // If the word is found, return the symptoms

            std::string lastName = normalizedWord.substr(spacePos + 1);
            result[lastName] = current->symptoms;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    runtime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    return result;
}

vector<pair<string, vector<string>>> TrieName::patientList(const string& name) {
    // Normalize the input: convert to lowercase
    std::string normalizedName = name;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);

    unordered_map<string, unordered_set<string>> symptoms = search(normalizedName);
    vector<pair<string, vector<string>>> result;
    if (symptoms.empty()) {
        std::cout << "No symptoms found for the name: " << name << std::endl;
        return result;
    }

    for (const auto& [lastname, symptom] : symptoms) {
        vector<string> symps;
        if (!symptom.empty()) {
            const string& symptomBinary = *symptom.begin();  // Grab the first (or only) symptom string
            for (size_t i = 0; i < symptomBinary.size(); ++i) {
                if (symptomBinary[i] == '1' && i < symptomsList.size()) {
                    symps.push_back(symptomsList[i]);
                }
            }
            
        }
        result.push_back({lastname, symps});
        
        
    }
    return result;
}
double TrieName::getRuntime() {
    return(runtime);
}