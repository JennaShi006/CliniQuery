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

void setCORS(crow::response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Credentials", "true");
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}


int main() {
    // Initialize the trie for names and symptoms
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
        insertData(trie, symp, line);
    }

    file.close();

    crow::SimpleApp app;

    // CROW_ROUTE(app, "/<path>")
    // .methods(crow::HTTPMethod::OPTIONS)
    // ([](const crow::request&, crow::response& res, std::string /*path*/) {
    //     res.code = 204;
    //     res.set_header("Access-Control-Allow-Origin", "*");
    //     res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    //     res.set_header("Access-Control-Allow-Headers", "Content-Type");
    //     return move(res);
    // });

    CROW_ROUTE(app, "/api/data").methods("OPTIONS"_method)([] {
        crow::response res;
        
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Credentials", "true");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.code = 200;
        res.end();
        return res;
    });

    vector<vector<string>> patientList;
    CROW_ROUTE(app, "/api/data").methods("POST"_method)([&trie, &patientList](const crow::request& req) {
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        // Parse the request body as plain text
        std::string body = req.body;
        std::cout << "Received body: " << body << std::endl;

        // Convert the plain text body to JSON format
        crow::json::wvalue result;
        result["input"] = body;
        patientList = trie.patientList(body);

        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        return std::move(res);
    });
    
    CROW_ROUTE(app, "/api/trieNameSearch")([&trie, &patientList]() {
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        // Serialize the patientList into JSON format
        crow::json::wvalue result;
        crow::json::wvalue::list patientsJson;
        for (const auto& patient : patientList) {
            crow::json::wvalue::list patientJson;
            for (const auto& field : patient) {
                patientJson.push_back(field);
            }
            patientsJson.push_back(std::move(patientJson));
        }
        result["patients"] = std::move(patientsJson);
        
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/setup")([](){
        crow::response res;
        //set CORS headers
        setCORS(res);

        //res.body is the data to be sent back to the front end
        res.body = R"(["Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea", "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath", "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"])";
        res.code = 200;
        return res;
    });
    CROW_ROUTE(app, "/api/setup").methods("OPTIONS"_method)([] {
        crow::response res;
        setCORS(res);
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

