#include <iostream>
#include "trieName.h"

using namespace std;

int main(){

    TrieName trie;
    trie.insert("josh", "0000000000000000");
    trie.insert("josh miles", "1000000000000000");
    trie.insert("Jenna Shi", "0100000000000000");
    //cout<< "inserted jenna" << endl;

    //checking basic insertion
    cout << "Searching for 'josh': " << (trie.search("josh") ? "Found" : "Not Found") << endl;
    //checking case insensitivity, first name last name
    cout << "Searching for 'Josh Miles': " << (trie.search("Josh Miles") ? "Found" : "Not Found") << endl;
    //checking case insensitivity, first name only
    cout << "Searching for 'Jenna Shi': " << (trie.search("Jenna") ? "Found" : "Not Found") << endl;
    cout << "Searching for prefix 'v': " << (trie.startsWith("v") ? "Found" : "Not Found") << endl;

    //printing all patients with the name "josh"
    trie.printPatients("josh");
    return 0;
}