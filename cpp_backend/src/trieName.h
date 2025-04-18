#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>

using namespace std;

class TrieNode {

public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
    string name; // Store the word at the end of the node
    unordered_set<string> symptoms;

    TrieNode() = default;
    ~TrieNode() = default;
};

class TrieName {
    public:
        TrieNode* root;
    
        TrieName() {
            root = new TrieNode();
        }
    
        ~TrieName() {
            deleteTrie(root);
        }
        void deleteTrie(TrieNode* node) {
            if (!node) return;
            for (auto& pair : node->children) {
                deleteTrie(pair.second);
            }
            delete node;
        }
    
        void insert(const string& name, const string& symptoms);
    
        bool search(const std::string& word);
    
        bool startsWith(const std::string& prefix);
        void printPatients(const std::string& name);
    };

