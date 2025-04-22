#include <iostream>
#include <fstream>
#include <sstream>
#include "bplus.h"
#include "trieName.h"
#include "trieSymp.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#define CROW_USE_ASIO   // tells Crow to use standalone ASIO
#include "crow.h"
#include "json.hpp"
#include <chrono>



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

    
    cout << "Patients with Fever: ";
    symp.search("Fever");
    auto sympruntime = symp.getRuntime();
    cout << "symp runtime: " << sympruntime << " microseconds" << endl;

    cout << "Patients with name Michael: ";
    trie.search("Michael");
    auto trieruntime = trie.getRuntime();
    cout << "trie runtime: " << trieruntime << " microseconds" << endl;

    crow::SimpleApp app;

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

    vector<pair<string, vector<string>>> patientList;
    double trieNameRuntime = 5.0;
    CROW_ROUTE(app, "/api/trieName").methods("POST"_method)([&trie, &patientList, &trieNameRuntime](const crow::request& req) {
        crow::response res;
        setCORS(res);

        // Parse the request body as plain text
        std::string body = req.body;
        std::cout << "Received body: " << body << std::endl;

        // Convert the plain text body to JSON format
        crow::json::wvalue result;
        result["input"] = body;
        patientList = trie.patientList(body);
        trieNameRuntime = trie.getRuntime();
        cout << "trieName runtime: " << trieNameRuntime << " microseconds" << endl;

        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        return std::move(res);
    });
    
    CROW_ROUTE(app, "/api/trieNameSearch")([&trie, &patientList]() {
        crow::response res;
        setCORS(res);

        // Serialize the patientList into JSON format
        crow::json::wvalue result;
        crow::json::wvalue::object patientsJson;

        for (const auto& [name, fields] : patientList) {
            crow::json::wvalue::list patientFieldsJson;
            for (const auto& field : fields) {
                patientFieldsJson.push_back(field); // Add each field (e.g., symptoms) to the JSON array
            }
            patientsJson[name] = std::move(patientFieldsJson); // Use the name as the key
        }
        result["patients"] = std::move(patientsJson);
        
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/trieNameTime")([&trieNameRuntime](){
        crow::response res;
        //set CORS headers
        setCORS(res);
        crow::json::wvalue result;
        result["time"] = std::to_string(trieNameRuntime);
        //res.body is the data to be sent back to the front end
        res.body = result.dump(); // Serialize the JSON response
        res.set_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });


    vector<string> patientNames;
    double trieSympRuntime;
    CROW_ROUTE(app, "/api/trieSymp").methods("POST"_method)([&symp, &patientNames, &trieSympRuntime](const crow::request& req) {
        crow::response res;
        setCORS(res);

        // Parse the request body as plain text
        std::string body = req.body;
        std::cout << "Received body: " << body << std::endl;

        // Convert the plain text body to JSON format
        crow::json::wvalue result;
        result["input"] = body;
        patientNames = symp.listPatients(body);
        trieSympRuntime = symp.getRuntime();

        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        return std::move(res);
    });

    CROW_ROUTE(app, "/api/trieSympSearch")([&symp, &patientNames]() {
        crow::response res;
        setCORS(res);

        // Serialize the patientList into JSON format
        crow::json::wvalue result;
        crow::json::wvalue::list patientsJson;
        for (const auto& patient : patientNames) {
    
            patientsJson.push_back(std::move(patient));
        }
        result["patients"] = std::move(patientsJson);
        
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/trieSympTime")([&trieSympRuntime](){
        crow::response res;
        //set CORS headers
        setCORS(res);
        crow::json::wvalue result;
        result["time"] = std::to_string(trieSympRuntime);
        //res.body is the data to be sent back to the front end
        res.body = result.dump(); // Serialize the JSON response
        res.set_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });

    vector<pair<string, vector<string>>> nameResults;
    double nameTreeRuntime;
    CROW_ROUTE(app, "/api/BPlusName").methods("POST"_method)([&nameTree, &nameResults, &nameTreeRuntime](const crow::request& req) {
        crow::response res;
        setCORS(res);

        // Parse the request body as plain text
        std::string body = req.body;
        std::cout << "Received body: " << body << std::endl;

        // Convert the plain text body to JSON format
        crow::json::wvalue result;
        result["input"] = body;
        nameResults = nameTree.searchName(body);
        nameTreeRuntime = nameTree.getNameRuntime();

        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        return std::move(res);
    });

    CROW_ROUTE(app, "/api/BPlusNameSearch")([&nameTree, &nameResults]() {
        crow::response res;
        setCORS(res);

        // Serialize the nameResults into JSON format
        crow::json::wvalue result;
        crow::json::wvalue::object patientsJson;

        for (int i=nameResults.size()-1; i>=0; i--) {
            string name = nameResults[i].first;
            vector<string> fields = nameResults[i].second;
            crow::json::wvalue::list patientFieldsJson;
            for (const auto& field : fields) {
                patientFieldsJson.push_back(field); // Add each field (e.g., symptoms) to the JSON array
            }
            patientsJson[name] = std::move(patientFieldsJson); // Use the name as the key
        }
        result["patients"] = std::move(patientsJson);
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/BPlusNameTime")([&nameTreeRuntime](){
        crow::response res;
        //set CORS headers
        setCORS(res);
        crow::json::wvalue result;
        result["time"] = std::to_string(nameTreeRuntime);
        //res.body is the data to be sent back to the front end
        res.body = result.dump(); // Serialize the JSON response
        res.set_header("Content-Type", "application/json");
        res.code = 200;
        return res;
    });


    vector<pair<string, vector<string>>> sympResults;
    double sympTreeRuntime;
    CROW_ROUTE(app, "/api/BPlusSymp").methods("POST"_method)([&symptomTree, &sympResults, &sympTreeRuntime](const crow::request& req) {
        crow::response res;
        setCORS(res);

        // Parse the request body as plain text
        std::string body = req.body;
        std::cout << "Received body: " << body << std::endl;

        // Convert the plain text body to JSON format
        crow::json::wvalue result;
        result["input"] = body;
        sympResults = symptomTree.searchSymp(body);
        sympTreeRuntime = symptomTree.getSympRuntime();
        cout << "sympTree runtime: " << sympTreeRuntime << " microseconds" << endl;

        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        return std::move(res);
    });

    CROW_ROUTE(app, "/api/BPlusSympSearch")([&symptomTree, &sympResults]() {
        crow::response res;
        setCORS(res);

        // Serialize the patientList into JSON format
        crow::json::wvalue result;
        crow::json::wvalue::object patientsJson;

        for (int i=sympResults.size()-1; i>=0; i--) {
            string symp = sympResults[i].first;
            vector<string> fields = sympResults[i].second;
            crow::json::wvalue::list patientFieldsJson;
            for (const auto& field : fields) {
                patientFieldsJson.push_back(field); // Add each field (e.g., symptoms) to the JSON array
            }
            patientsJson[symp] = std::move(patientFieldsJson); // Use the name as the key
        }
        result["patients"] = std::move(patientsJson);
        res.set_header("Content-Type", "application/json");
        res.body = result.dump(); // Serialize the JSON response
        res.code = 200;
        return res;
    });

    CROW_ROUTE(app, "/api/BPlusSympTime")([&sympTreeRuntime](){
        crow::response res;
        //set CORS headers
        setCORS(res);
        crow::json::wvalue result;
        result["time"] = std::to_string(sympTreeRuntime);
        //res.body is the data to be sent back to the front end
        res.body = result.dump(); // Serialize the JSON response
        res.set_header("Content-Type", "application/json");
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

    return 0;
}

