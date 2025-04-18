
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;


class BPlus{

    // Large order for smaller height / shallower tree allowing for more efficient search
    // At most 100 children and 99 keys for non-leaf nodes, 99 keys for leaf node
    const static int order = 100;

    struct Node {
        bool isLeaf = false;
        vector<string> keys;
        int keyCount = 0;
        Node* parent = nullptr;
    };

    struct Leaf : public Node {
        vector<string> values;
        Leaf* prev = nullptr;
        Leaf* next = nullptr;

        Leaf();

        // Finds the position where a key should be inserted or is located
        int findPos(const string& key);

        // Inserts a key-value pair into the leaf node
        void insert(const string& key, const string& value);
    };

    struct InternalNode : public Node {
        vector<Node*> children; // Pointers to child nodes

        InternalNode();

        // Finds the index of the child pointer to follow for a given key
        int findChild(const string& key);

        // Inserts a key and a right child pointer (from a split)
        void insert(const string& key, Node* rightChild);
    };

    Node* root = nullptr;
    Leaf* head = nullptr; // Head of the doubly linked list of leaves
    Leaf* tail = nullptr; // Tail of the doubly linked list of leaves

    // Constants derived from order
    int minLeafKeys = static_cast<int>(ceil((order - 1) / 2.0));
    int minIntKeys = static_cast<int>(ceil(order / 2.0) - 1);


    // Private Helper Methods

    // Find the leaf node where a key should be
    Leaf* findLeaf(const string& key);

    // Split a leaf node and update linked list and parent
    void splitLeaf(Leaf* leaf);

    // Split an internal node
    void splitInternal(InternalNode* node);

    // Helper to insert a key (and potentially a new child) into a parent node
    void insertInParent(Node* leftChild, Node* rightChild, const string& key);

    // Recursive deletion of nodes (used by destructor)
    void destroyTree(Node* node) {
        if (node == nullptr){
            return;
        }
        if (node->isLeaf == false) {
            InternalNode* internal = static_cast<InternalNode*>(node);
            for (int i = 0; i <= internal->keyCount; i++) {
                destroyTree(internal->children[i]);
            }
        }
        delete node;
    }

    // Public methods

    public:

    BPlus() = default;

    ~BPlus() {
        destroyTree(root);
    }

    // Insert a key-value pair
    void insert(const string& key, const string& value);

    // Search for a key and return a pointer to its leaf and index (or INT_MAX if not found)
    pair<Leaf*, int> search(const string& key);


    // Search for a key and return the closest 50 results' key-value pairs
    vector<pair<string,string>> searchResults(const string& key);

};

