#include <iostream>
#include <fstream>
#include <sstream>
//#include "bplus.h"
#include "trieName.h"
#include "trieSymp.h"
#include <vector>
#define CROW_USE_ASIO   // tells Crow to use standalone ASIO
#include "crow.h"
#include "json.hpp"



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
    crow::SimpleApp app;
    CROW_ROUTE(app, "/api/data")([](){
        nlohmann::json data = {
            {{"name", "Example"}, {"value", 42}}
        };
        return crow::json::wvalue(crow::json::rvalue(data));
    });

    app.port(18080).multithreaded().run();

    cout<<"server shutdown"<<endl;
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

    // vector<pair<string, string>> nameResults = bPlusTree.searchName("derrick");

    // cout<<"Searching by name:"<<endl;

    // for (int i=0; i<min(50,(int)nameResults.size()); i++) {
    //     cout << "Name: " << nameResults[i].first << ", Symptoms: " << nameResults[i].second << endl;
    // }

    // BPlus symptomTree;
    // symptomTree.insert("1000000000000000", "josh k");
    // symptomTree.insert("1000000000000001", "john");
    // symptomTree.insert("1000000000001111", "josh l");
    // symptomTree.insert("0000000000000001", "josh p");
    // symptomTree.insert("0000000000000011", "jenna");
    // symptomTree.insert("0000000100001011", "jacob");

    // vector<pair<string, string>> sympResults = symptomTree.searchSymp("0000000000001000");

    // cout<<endl<<"Searching by symptom:"<<endl;

    // for (int i=0; i<min(50,(int)sympResults.size()); i++) {
    //     cout << "Name: " << sympResults[i].second << ", Symptoms: " << sympResults[i].first << endl;
    // }


    return 0;
}

