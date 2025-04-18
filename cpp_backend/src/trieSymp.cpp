#include "trieSymp.h"
#include <cctype>

// -------- TrieNode Implementation --------

TrieSympNode::TrieSympNode() {
    isEndOfSymp = false;
}

TrieSympNode::~TrieSympNode() {
    for (auto& pair : children) {
        delete pair.second;
    }
}

// -------- SymptomTrie Implementation --------

TrieSymp::TrieSymp() {
    root = new TrieSympNode();
}


// void TrieSymp::deleteTrie(TrieSympNode* node) {
//     for (auto& pair : node->children) {
//         deleteTrie(pair.second);
//     }
//     delete node;
// }

void TrieSymp::insert(const string& symptom, const string& patient) {
    TrieSympNode* node = root;
    for (char c : symptom) {
        char lower = std::tolower(c);
        if (node->children.find(lower) == node->children.end()) {
            node->children[lower] = new TrieSympNode();
        }
        node = node->children[lower];
    }
    node->isEndOfSymp = true;
    node->names.insert(patient);
}

// std::unordered_set<std::string> SymptomTrie::getPatientsBySymptom(const std::string& symptom) {
//     TrieSympNode* node = root;
//     for (char c : symptom) {
//         char lower = std::tolower(c);
//         if (node->children.find(lower) == node->children.end()) {
//             return {};
//         }
//         node = node->children[lower];
//     }
//     return node->isEndOfSymp ? node->patients : std::unordered_set<std::string>();
// }

// void TrieSymp::dfs(TrieSympNode* node, std::unordered_set<std::string>& result) {
//     if (node->isEndOfSymp) {
//         result.insert(node->patients.begin(), node->patients.end());
//     }
//     for (auto& pair : node->children) {
//         dfs(pair.second, result);
//     }
// }

// std::unordered_set<std::string> SymptomTrie::getPatientsBySymptomPrefix(const std::string& prefix) {
//     TrieNode* node = root;
//     for (char c : prefix) {
//         char lower = std::tolower(c);
//         if (node->children.find(lower) == node->children.end()) {
//             return {};
//         }
//         node = node->children[lower];
//     }

//     std::unordered_set<std::string> result;
//     dfs(node, result);
//     return result;
//}