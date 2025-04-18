#include <iostream>
#include <fstream>
#include <sstream>
//#include "bplus.h"
#include "trieName.h"
#include "trieSymp.h"
#include <vector>


using namespace std;

void insertData(TrieName& trieName, TrieSymp& symp, const auto& line) {
    vector<string> symptoms = {
        "Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea", 
        "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath", 
        "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"
    };
    stringstream ss(line);
    string fullName, symptomData;
    int sympTrack = 0;

    // Read the full name (first column)
    getline(ss, fullName, ',');

        // Read the rest of the symptom data (remaining columns)
    string symptomsBinary;
    while (getline(ss, symptomData, ',')) {
        symptomsBinary += symptomData;
        if(symptomData == "1"){
        symp.insert(symptoms[sympTrack], fullName);
        }
        sympTrack++;
    }

    // Insert the full name and symptoms into the trie
    trieName.insert(fullName, symptomsBinary);
}


int main() {
    TrieName trie;
    TrieSymp symp;
    // Open the CSV file
    ifstream file("../data_gen/CliniQuery_Data.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return 1;
    }

    string line;
    getline(file, line); // Skip the header line

    // Read each line from the CSV
    while (getline(file, line)) {
        // stringstream ss(line);
        // string fullName, symptomData;

        // // Read the full name (first column)
        // getline(ss, fullName, ',');

        // // Read the rest of the symptom data (remaining columns)
        // string symptomsBinary;
        // while (getline(ss, symptomData, ',')) {
        //     symptomsBinary += symptomData;
        // }

        // // Insert the full name and symptoms into the trie
        // trie.insert(fullName, symptomsBinary);
        insertData(trie, symp, line);
    }

    file.close();

    //test the trie;
    trie.printSymptoms("Michael");
    symp.printPatients("fever");


    // // Test the trie
    // cout << "Searching for 'Josh Miles': " << (trie.search("Josh Miles") ? "Found" : "Not Found") << endl;
    // cout << "Searching for 'Jenna Shi': " << (trie.search("Jenna Shi") ? "Found" : "Not Found") << endl;




    // BPlus bPlusTree;
    // bPlusTree.insert("josh k", "0000000000000000");
    // bPlusTree.insert("josh l", "1000000000000000");
    // bPlusTree.insert("jenna", "0100000000000000");
    // bPlusTree.insert("josh p", "0000100000100000");
    // bPlusTree.insert("jerry", "0001000000110000");
    // bPlusTree.insert("derrick", "0001100000110000");
    // bPlusTree.insert("derek", "0000000100000001");
    // bPlusTree.insert("tanvi", "0000010100010001");

    // vector<pair<string, string>> results = bPlusTree.searchResults("josh");

    // for (int i=0; i<results.size(); i++) {
    //     cout << "Name: " << results[i].first << ", Symptoms: " << results[i].second << endl;
    // }


    return 0;
}

