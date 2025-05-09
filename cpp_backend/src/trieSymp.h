#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;
#include <chrono>

class TrieSympNode {

public:
    unordered_map<char, TrieSympNode*> children;
    set<string> names;
    bool isEndOfSymp = false;

    TrieSympNode();
    ~TrieSympNode();

};


class TrieSymp {
    private:
    TrieSympNode* root;
    double runtime;

public:
    TrieSymp();
    void insert(const string& symptom, const string& patient);
    set<string> search(const string& symptom);
    void printPatients(const string& symptom);
    vector<string> listPatients(const string& symptom);
    ~TrieSymp();
    double getRuntime();
    void insertSymptom(const string& symptom);
};

