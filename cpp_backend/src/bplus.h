
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
    int order = 100;

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

        Leaf() {
            this->isLeaf = true;
            // Reserve space based on order to potentially reduce reallocations
            this->keys.resize(order-1); // Max keys = order - 1 (l = n-1)
            this->values.resize(order-1);
            this->keyCount = 0;
        }

        // Finds the position where a key should be inserted or is located
        int findPos(string& key) {
            // Use lower_bound for efficient search in sorted keys
            auto it = lower_bound(this->keys.begin(), this->keys.begin() + this->keyCount, key);
            return distance(this->keys.begin(), it);
        }

        // Inserts a key-value pair into the leaf node
        void insert(string& key, string& value) {
            int pos = findPos(key);

            // Shift elements to make space for the new key-value pair
            for (int i = this->keyCount; i > pos; --i) {
                this->keys[i] = this->keys[i - 1];
                this->values[i] = this->values[i - 1];
            }

            // Insert the new key-value pair
            this->keys[pos] = key;
            this->values[pos] = value;
            this->keyCount++;
        }
    };

    struct InternalNode : public Node {
        vector<Node*> children; // Pointers to child nodes

        InternalNode() {
            this->isLeaf = false;
            // Max keys = order - 1
            this->keys.resize(order-1);
            // Max children = order
            this->children.resize(order);
            this->keyCount = 0;
        }

        // Finds the index of the child pointer to follow for a given key
        int findChild(string& key) {
            // Find the first key greater than or equal to the target key
            auto it = lower_bound(this->keys.begin(), this->keys.begin() + this->keyCount, key);
            // The index of this key corresponds to the child pointer after it
            // If key is smaller than all keys, it is index 0
            return distance(this->keys.begin(), it);
        }

        // Inserts a key and a right child pointer (from a split)
        void insert(string& key, Node* rightChild) {
            int pos = findChild(key); // Find position based on key

            // Shift keys and children to make space
            for (int i = this->keyCount; i > pos; --i) {
                this->keys[i] = this->keys[i - 1];
                this->children[i + 1] = this->children[i];
            }

            // Insert the new key and child
            this->keys[pos] = key;
            this->children[pos + 1] = rightChild;
            this->keyCount++;

            // Update parent pointers of children
            if (rightChild != nullptr) {
                rightChild->parent = this;
            }
             // Left child (children[pos]) parent remains the same
        }
    };

    Node* root = nullptr;
    Leaf* head = nullptr; // Head of the doubly linked list of leaves
    Leaf* tail = nullptr; // Tail of the doubly linked list of leaves

    // Constants derived from order
    int minLeafKeys = static_cast<int>(ceil((order - 1) / 2.0));
    int minIntKeys = static_cast<int>(ceil(order / 2.0) - 1);


    // Private Helper Methods

    // Find the leaf node where a key should be
    Leaf* findLeaf(string& key) {
        Node* curr = root;
        if (curr == nullptr) {
            return nullptr;
        }

        while (curr->isLeaf == false) {
            InternalNode* internal = static_cast<InternalNode*>(curr);
            int child = internal->findChild(key);
            curr = internal->children[child];

            if (curr==nullptr) { // Should not happen in a valid tree
                return nullptr; 
            }
        }
        return static_cast<Leaf*>(curr);
    }

    // Split a leaf node and update linked list and parent
    void splitLeaf(Leaf* leaf) {
        Leaf* newLeaf = new Leaf();
        string promote;

        // Calculate split point
        int split = int(order) / 2; // Usually order/2 keys go to the new node

        // Copy second half of keys/values to the new leaf
        newLeaf->keyCount = 0;
        for (int i = split; i < leaf->keyCount; i++) {
            newLeaf->keys[newLeaf->keyCount] = leaf->keys[i];
            newLeaf->values[newLeaf->keyCount] = leaf->values[i];
            newLeaf->keyCount++;
        }

        // Update original leaf's key count
        leaf->keyCount = split;

        // Key to promote is the first key in the new leaf
        promote = newLeaf->keys[0];

        // Update doubly linked list
        newLeaf->next = leaf->next;
        if (leaf->next) {
            leaf->next->prev = newLeaf;
        } else {
            tail = newLeaf; // Original leaf was the tail
        }
        leaf->next = newLeaf;
        newLeaf->prev = leaf;

        // Insert the promoted key into the parent
        insertInParent(leaf, promote, newLeaf);
    }

    // Split an internal node
    void splitInternal(InternalNode* node) {
        InternalNode* newNode = new InternalNode();
        string promote;

        // Calculate split point (middle key gets promoted)
        int split = order / 2; // Index of the key to promote

        promote = node->keys[split];

        // Copy keys and children after the split point to the new node
        // Keys from split + 1 onwards
        newNode->keyCount = 0;
        for (int i = split + 1; i < node->keyCount; i++) {
            newNode->keys[newNode->keyCount++] = node->keys[i];
        }
        // Children from split + 1 onwards
        for (int i = split + 1; i <= node->keyCount; i++) {
             newNode->children[i - (split + 1)] = node->children[i];
             if(node->children[i]) node->children[i]->parent = newNode; // Update parent pointer
        }
        newNode->children[node->keyCount - split] = node->children[node->keyCount + 1]; // Copy last child pointer potentially missed


        // Update original node's key count (key at split is removed)
        node->keyCount = split;

        // Insert the promoted key into the parent
        insertInParent(node, promote, newNode);
    }

    // Helper to insert a key (and potentially a new child) into a parent node
    void insertInParent(Node* leftChild, Node* rightChild, string& key) {
        if (leftChild == root) {
            // Create a new root
            InternalNode* newRoot = new InternalNode();
            newRoot->keys[0] = key;
            newRoot->children[0] = leftChild;
            newRoot->children[1] = rightChild;
            newRoot->keyCount = 1;
            root = newRoot;

            // Update parent pointers of children
            leftChild->parent = newRoot;
            rightChild->parent = newRoot;
        } else {
            InternalNode* parent = static_cast<InternalNode*>(leftChild->parent);
            parent->insert(key, rightChild); // Insert key and right child pointer

             // Update parent pointer of the newly added right child
            if (rightChild != nullptr) {
                rightChild->parent = parent;
            }

            // Check if parent needs splitting
            if (parent->keyCount > order-1) {
                 splitInternal(parent);
            }
        }
    }

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

    BPlusTree() = default;

    ~BPlusTree() {
        destroyTree(root);
        delete root;
        delete head;
        delete tail;
    }

    // Insert a key-value pair
    void insert(string& key, string& value) {
        if (root == nullptr) {
            // Create the first leaf node
            root = new Leaf();
            Leaf* leafRoot = static_cast<Leaf*>(root);
            leafRoot->keys[0] = key;
            leafRoot->values[0] = value;
            leafRoot->keyCount = 1;
            head = leafRoot;
            tail = leafRoot;
            return;
        }

        // Find the appropriate leaf node
        Leaf* leaf = findLeaf(key);
        if (leaf == nullptr) {
             // Should not happen if root is not null, indicates an error
             throw runtime_error("Could not find leaf for insertion.");
        }

        // Check for duplicates
        int existing = leaf->findPos(key);
        if (existing < leaf->keyCount && leaf->keys[existing] == key) {
            // Handle duplicate: Ignore
             return;
        }

        // Insert into the leaf
        leaf->insert(key, value);

        // Check if the leaf node needs splitting
        if (leaf->keyCount > order-1) {
            splitLeaf(leaf);
        }
    }

    // Search for a key and return a pointer to its value (or nullptr if not found)
    string* search(string& key) {
        Leaf* leaf = findLeaf(key);
        if (leaf == nullptr) {
            return nullptr; // Key not found
        }

        int pos = leaf->findPos(key);

        if (pos < leaf->keyCount && leaf->keys[pos] == key) {
            // Key found, return pointer to value
            return &leaf->values[pos];
        }
        // Key not found in the leaf
        return nullptr;
    }

    // Print all key-value pairs by traversing the doubly linked list of leaves
    void printLeaves() const {
        std::cout << "Leaf Nodes (Doubly Linked List Traversal):" << std::endl;
        Leaf* current = head;
        int leaf_num = 0;
        while (current != nullptr) {
            std::cout << "Leaf " << leaf_num++ << " [";
            if (current->prev) {
                 std::cout << "P:" << (current->prev ? current->prev->keys[0] : -1) << "|";
            } else {
                 std::cout << "P:N|";
            }
             std::cout << "Keys: ";
            for (int i = 0; i < current->keyCount; ++i) {
                std::cout << "(" << current->keys[i] << ":" << current->values[i] << ") ";
            }
            if (current->next) {
                 std::cout << "|N:" << (current->next ? current->next->keys[0] : -1);
            } else {
                 std::cout << "|N:N";
            }
            std::cout << "] -> ";
            current = current->next;
        }
         std::cout << "NULL" << std::endl;

        // Optional: Print tail for verification
         if(tail) {
             std::cout << "Tail points to leaf starting with key: " << tail->keys[0] << std::endl;
         } else {
             std::cout << "Tail is NULL" << std::endl;
         }
    }

     // Basic print function for tree structure (for debugging)
    void printTree() const {
        std::cout << "--- B+ Tree Structure ---" << std::endl;
        if (!root) {
            std::cout << "(Empty Tree)" << std::endl;
            return;
        }
        printNode(root, 0);
        std::cout << "-------------------------" << std::endl;
    }

private:
    void printNode(Node* node, int level) const {
         if (!node) return;

         // Indentation
         for (int i = 0; i < level; ++i) std::cout << "  ";

         if (node->isLeaf) {
             Leaf* leaf = static_cast<Leaf*>(node);
             std::cout << "Leaf [";
             for (int i = 0; i < leaf->keyCount; ++i) {
                 std::cout << leaf->keys[i] << ":" << leaf->values[i] << (i == leaf->keyCount - 1 ? "" : ", ");
             }
             std::cout << "] (Parent: " << node->parent << ")" << std::endl;
         } else {
             InternalNode* internal = static_cast<InternalNode*>(node);
             std::cout << "Internal [";
             for (int i = 0; i < internal->keyCount; ++i) {
                 std::cout << internal->keys[i] << (i == internal->keyCount - 1 ? "" : ", ");
             }
             std::cout << "] (Parent: " << node->parent << ")" << std::endl;

             // Recursively print children
             for (int i = 0; i <= internal->keyCount; ++i) {
                 printNode(internal->children[i], level + 1);
             }
         }
     }


};

