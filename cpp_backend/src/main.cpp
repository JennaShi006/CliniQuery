#include <iostream>
#include <fstream>
#include <sstream>
//#include "bplus.h"
#include "trieName.h"
#include "trieSymp.h"
#include <vector>
#define CROW_USE_ASIO   // tells Crow to use standalone ASIO
#define CROW_ENABLE_MIDDLEWARE
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

void insertBPlus(BPlus& nameTree, BPlus& sympTree, const auto& line) {

    stringstream ss(line);
    string fullName, symptomData;

    // Read the full name (first column)
    getline(ss, fullName, ',');

    // Read the rest of the symptom data (remaining columns)
    string symptomsBinary;
    while (getline(ss, symptomData, ',')) {
        symptomsBinary += symptomData;
    }

    // Insert the full name and symptoms into the tree
    nameTree.insert(fullName, symptomsBinary);
    sympTree.insert(symptomsBinary, fullName);
}

int main() {
    // Initialize the trie for names and symptoms
    TrieName trie;
    TrieSymp symp;
    
    BPlus nameTree;
    BPlus symptomTree;

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
        insertData(trie, symp, line);
        insertBPlus(nameTree, symptomTree, line);
    }

    file.close();

    crow::SimpleApp app;
    ///api/data is just an endpoint to test the server
    CROW_ROUTE(app, "/api/data")([](){
        crow::response res;

        //set CORS headers
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        //res.body is the data to be sent back to the front end
        res.body = R"([{"name":"Josh Miles","symptoms":"0000000000000000"},{"name":"Jenna Shi","symptoms":"1000000000000000"},{"name":"Michael","symptoms":"0100000000000000"},{"name":"Tanvi","symptoms":"1100000000100000"},{"name":"Jerry","symptoms":"0011000000110000"},{"name":"Derrick","symptoms":"0011100000111111"},{"name":"Jacob","symptoms":"1111111111111111"}])";
        res.code = 200; // HTTP status code 200 OK
        return res;
    });
    
    //This doesn't work yet; this gets the data that is sent back from the frontend
    CROW_ROUTE(app, "/api/data").methods("OPTIONS"_method, "POST"_method)([](const crow::request& req, crow::response& res) {
        if (req.method == "OPTIONS"_method) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.code = 204; // No Content
            res.end();
            return;
        }

        try {
            auto jsonData = crow::json::load(req.body);
            if (!jsonData || !jsonData.has("name") || !jsonData.has("symptoms")) {
                res.code = 400;
                res.body = "Invalid JSON structure";
                return;
            }

            std::string name = jsonData["name"].s();
            std::string symptoms = jsonData["symptoms"].s();

            std::cout << "Received Name: " << name << ", Symptoms: " << symptoms << std::endl;

            // Process the data (e.g., insert into trie or database)
            res.code = 200;
            res.body = "Data received successfully";
        } catch (const std::exception& e) {
            res.code = 500;
            res.body = "Internal server error: " + std::string(e.what());
        }
    });

    //sends the data of all the symptoms to the front end
    //this is used to populate the dropdown menu in the front end
    //this one works, pulling data from backend in general works
    CROW_ROUTE(app, "/api/setup")([](){
        crow::response res;
        //set CORS headers
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Credentials", "true");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        //res.body is the data to be sent back to the front end
        res.body = R"(["Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea", "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath", "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"])";
        res.code = 200;
        return res;
    });

    app.port(8080).multithreaded().run();

    cout<<"server shutdown"<<endl;

    
    

    //test the trie;
    // trie.printSymptoms("Michael");
    // symp.printPatients("fever");


    // // Test the trie
    // cout << "Searching for 'Josh Miles': " << (trie.search("Josh Miles") ? "Found" : "Not Found") << endl;
    // cout << "Searching for 'Jenna Shi': " << (trie.search("Jenna Shi") ? "Found" : "Not Found") << endl;



    // Test B+ Tree
    
    vector<pair<string, string>> nameResults = nameTree.searchName("Derrick Ma");

    cout<<"Searching by name:"<<endl;

    for (int i=0; i<min(50,(int)nameResults.size()); i++) {
        cout << "Name: " << nameResults[i].first << ", Symptoms: " << nameResults[i].second << endl;
    }


    vector<pair<string, string>> sympResults = symptomTree.searchSymp("0000000000001000");

    cout<<endl<<"Searching by symptom:"<<endl;

    for (int i=0; i<min(50,(int)sympResults.size()); i++) {
        cout << "Name: " << sympResults[i].second << ", Symptoms: " << sympResults[i].first << endl;
    }


    return 0;
}

