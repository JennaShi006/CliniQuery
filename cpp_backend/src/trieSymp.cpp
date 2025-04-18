#include "trieSymp.h"
#include <cctype>


TrieSympNode::TrieSympNode() {
    isEndOfSymp = false;
}

TrieSympNode::~TrieSympNode() {
    for (auto& pair : children) {
        delete pair.second;
    }
}


TrieSymp::TrieSymp() {
    root = new TrieSympNode();
}


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

unordered_set<string> TrieSymp::search(const string& symptom) {
    TrieSympNode* node = root;
    for (char c : symptom) {
        char lower = std::tolower(c);
        if (node->children.find(lower) == node->children.end()) {
            return {};
        }
        node = node->children[lower];
    }
    if (node->isEndOfSymp) {
        return node->names;
    } else {
        return unordered_set<string>();
    }
}

void TrieSymp::printPatients(const string& symptom){
    unordered_set<string> names = search(symptom);
    for(string s: names){
        cout<<s<<" | ";
    }
}

TrieSymp::~TrieSymp(){
    delete root;
}