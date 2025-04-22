#include "trieSymp.h"
#include <cctype>
#include <iostream>

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

set<string> TrieSymp::search(const string& symptom) {
    auto start = std::chrono::high_resolution_clock::now();
    TrieSympNode* node = root;
    for (char c : symptom) {
        char lower = std::tolower(c);
        if (node->children.find(lower) == node->children.end()) {
            return {};
        }
        node = node->children[lower];
    }
    if (node->isEndOfSymp) {
        auto end = std::chrono::high_resolution_clock::now();
        runtime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        return node->names;
    } else {
        auto end = std::chrono::high_resolution_clock::now();
        runtime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        return set<string>();
    }
    
}

void TrieSymp::printPatients(const string& symptom){
   

    set<string> names = search(symptom);
    for(string s: names){
        cout<<s<<" | ";
    }
   
}

vector<string> TrieSymp::listPatients(const string& symptom) {
    set<string> names = search(symptom);
    vector<string> result;
    for (const string& name : names) {
        result.push_back(name);
    }
    return result;
}

TrieSymp::~TrieSymp(){
    delete root;
}

double TrieSymp::getRuntime() {
    return(runtime);
}