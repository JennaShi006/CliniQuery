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
    unordered_map<string, unordered_set<string>> lastNameToSymptoms;
    
    TrieNode() = default;
    ~TrieNode() = default;
};

class TrieName {
    private:
    double runtime;
    vector<string> symptomsList = {
        "Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea",
        "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath",
        "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"
    };

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
    
        unordered_map<string, unordered_set<string>> search(const std::string& name);
        vector<pair<string, vector<string>>> patientList(const string& name);
        
        double getRuntime();
    };

