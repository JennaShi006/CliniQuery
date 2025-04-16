#include "trieName.h"
void TrieName::insert(const string& name, const string& symptoms) {
    TrieNode* current = root;
    for (char c : name) {
        int index = c - 'a';
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, create a new TrieNode
            // and insert it into the children map
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
    current->name = name; // Store the name at the end of the node
    current->symptoms.insert(symptoms); // Store the symptoms at the end of the node
}

bool TrieName::search(const std::string& word) {
    TrieNode* current = root;
    for (char c : word) {
        int index = c - 'a';
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            return false;
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

bool TrieName::startsWith(const std::string& prefix) {
    TrieNode* current = root;
    for (char c : prefix) {
        int index = c - 'a';
        if (current->children.find(c) == current->children.end()) {
            // If the character is not present, return false
            return false;
        }
        current = current->children[index];
    }
    return true;
}