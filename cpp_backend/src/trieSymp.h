#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class TrieSympNode {

public:
    unordered_map<char, TrieSympNode*> children;
    unordered_set<string> names;
    bool isEndOfSymp = false;

    TrieSympNode();
    ~TrieSympNode();

};


class TrieSymp {
    private:
    TrieSympNode* root;

public:
    TrieSymp();
    void insert(const string& symptom, const string& patient);
    unordered_set<string> search(const string& symptom);
    void printPatients(const string& symptom);
    ~TrieSymp();
};

// class Trie {
// public:
//     TrieNode* root;

//     Trie() : root(new TrieNode()) {}

//     ~Trie() {
//         delete root;
//     }

//     void insert(const std::string& word) {
//         TrieNode* current = root;
//         for (char c : word) {
//             int index = c - 'a';
//             if (!current->children[index]) {
//                 current->children[index] = new TrieNode();
//             }
//             current = current->children[index];
//         }
//         current->isEndOfWord = true;
//     }

//     bool search(const std::string& word) {
//         TrieNode* current = root;
//         for (char c : word) {
//             int index = c - 'a';
//             if (!current->children[index]) {
//                 return false;
//             }
//             current = current->children[index];
//         }
//         return current->isEndOfWord;
//     }

//     bool startsWith(const std::string& prefix) {
//         TrieNode* current = root;
//         for (char c : prefix) {
//             int index = c - 'a';
//             if (!current->children[index]) {
//                 return false;
//             }
//             current = current->children[index];
//         }
//         return true;
//     }
// };