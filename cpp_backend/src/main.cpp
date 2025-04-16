#include <iostream>
#include "trieName.h"

using namespace std;

int main(){

    TrieName trie;
    trie.insert("josh", "0000000000000000");
    trie.insert("josh", "1000000000000000");
    trie.insert("jenna", "0100000000000000");

    cout << "Searching for 'josh': " << (trie.search("josh") ? "Found" : "Not Found") << endl;
    cout << "Searching for 'jenna': " << (trie.search("jen") ? "Found" : "Not Found") << endl;
    cout << "Searching for prefix 'jen': " << (trie.startsWith("jen") ? "Found" : "Not Found") << endl;

    return 0;
}