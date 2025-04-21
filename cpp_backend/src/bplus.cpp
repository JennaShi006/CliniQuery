#include "bplus.h"

BPlus::Leaf::Leaf() {
    this->isLeaf = true;
    // Reserve space based on order to potentially reduce reallocations
    this->keys.resize(order); // Max keys = order - 1 (l = n-1)
    this->values.resize(order);
    this->keyCount = 0;
}

// Finds the position where a key should be inserted or is located
int BPlus::Leaf::findPos(const string& key) {
    // Use lower_bound for efficient search in sorted keys
    auto it = lower_bound(this->keys.begin(), this->keys.begin() + this->keyCount, key);
    return distance(this->keys.begin(), it);
}

void BPlus::Leaf::insert(const string& key, const string& value) {
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

BPlus::InternalNode::InternalNode() {
    this->isLeaf = false;
    // Max keys = order - 1
    this->keys.resize(order);
    // Max children = order
    this->children.resize(order+1);
    this->keyCount = 0;
}

// Finds the index of the child pointer to follow for a given key
int BPlus::InternalNode::findChild(const string& key) {
    // Find the first key greater than or equal to the target key
    auto it = lower_bound(this->keys.begin(), this->keys.begin() + this->keyCount, key);
    // The index of this key corresponds to the child pointer after it
    // If key is smaller than all keys, it is index 0
    return distance(this->keys.begin(), it);
}

// Inserts a key and a right child pointer (from a split)
void BPlus::InternalNode::insert(const string& key, Node* rightChild) {
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

// Find the leaf node where a key should be
BPlus::Leaf* BPlus::findLeaf(const string& key) {
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
void BPlus::splitLeaf(Leaf* leaf) {
    Leaf* newLeaf = new Leaf();
    string promote;

    // Calculate split point
    int split = int(order) / 2; // Usually order/2 keys go to the new node

    // Copy second half of keys/values to the new leaf
    newLeaf->keyCount = 0;
    for (int i = split; i < leaf->keyCount; ++i) {
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
    insertInParent(leaf, newLeaf, promote);
}

// Split an internal node
void BPlus::splitInternal(InternalNode* node) {
    InternalNode* newNode = new InternalNode();
    string promote;

    // Calculate split point (middle key gets promoted)
    int split = order / 2; // Index of the key to promote

    promote = node->keys[split];

    // Copy keys and children after the split point to the new node
    // Keys from split + 1 onwards
    newNode->keyCount = 0;
    for (int i = split + 1; i < node->keyCount; ++i) {
        newNode->keys[newNode->keyCount++] = node->keys[i];
    }
    // Children from split + 1 onwards
    for (int i = split + 1; i <= node->keyCount; ++i) {
         newNode->children[i - (split + 1)] = node->children[i];
         if(node->children[i]) node->children[i]->parent = newNode; // Update parent pointer
    }
    newNode->children[node->keyCount - split] = node->children[node->keyCount + 1]; // Copy last child pointer potentially missed


    // Update original node's key count (key at split is removed)
    node->keyCount = split;

    // Insert the promoted key into the parent
    insertInParent(node, newNode, promote);
}

// Helper to insert a key (and potentially a new child) into a parent node
void BPlus::insertInParent(Node* leftChild, Node* rightChild, const string& key) {
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

 // Insert a key-value pair
 void BPlus::insert(const string& key, const string& value) {
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

// Search for a key and return a pointer to its leaf and index (or INT_MAX if not found)
pair<BPlus::Leaf*, int> BPlus::search(const string& key) {
    pair<Leaf*, int> result(nullptr, INT_MAX);
    Leaf* leaf = findLeaf(key);
    if (leaf == nullptr) {
        return result; // Key not found
    }

    int pos = leaf->findPos(key);

    if (pos < leaf->keyCount) {
        result = {leaf, pos}; // Key found
    }

    return result;
}

// Search for a name and return the closest results' key-value pairs
vector<pair<string,vector<string>>> BPlus::searchName(const string& key) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<string> symptoms = {
        "Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea",
        "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath",
        "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"
    };
    vector<pair<string,vector<string>>> results;
    pair<Leaf*, int> topResult = search(key);
    if (topResult.first != nullptr) {
        vector<string> firstSymps;
        for (int i=0; i<16; i++) {
            if (topResult.first->values[topResult.second].substr(i, 1) == "1") {
                firstSymps.push_back(symptoms[i]);
            }
        }
        results.push_back({topResult.first->keys[topResult.second], firstSymps});

        // Only has to search right since search() returns the first instance of the first name
        int right = topResult.second + 1;
        Leaf* rightLeaf = topResult.first;

        // Add the top result and the closest results in descending order
        while (rightLeaf != nullptr){

            // Try adding from the right
            if (rightLeaf != nullptr) {
                if (right < rightLeaf->keyCount) {
                    // Break if different first name
                    if (rightLeaf->keys[right].substr(0, key.size())==key) {
                        break;
                    }
                    // Valid index in current rightLeaf
                    // Make a vector containing the symptoms written out
                    vector<string> symps;
                    for (int i=0; i<16; i++) {
                        if (rightLeaf->values[right].substr(i, 1) == "1") {
                            symps.push_back(symptoms[i]);
                        }
                    }
                    results.push_back({rightLeaf->keys[right], symps});
                    right++;
                } else {
                    // Index is out of bounds, move to the next leaf
                    rightLeaf = rightLeaf->next;
                    if (rightLeaf != nullptr) {
                        // Reset index to the start of the new next leaf
                        right = 0;
                    }
                    // If rightLeaf became nullptr, the outer `if (rightLeaf != nullptr)` will fail next time
                    continue;
                }
            }
        }
    }
    return results;
    auto end = std::chrono::high_resolution_clock::now();
    nameRuntime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

// Search for a symptom and return the closest results' key-value pairs
vector<pair<string,vector<string>>> BPlus::searchSymp(const string& key) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<string> symptoms = {
        "Fever", "Chest pains", "Abdominal pain", "Cough", "Fatigue", "Nausea",
        "Bleeding", "Seizures", "Dizziness", "Headaches", "Shortness of breath",
        "Memory loss", "Swelling", "Diarrhea", "Constipation", "Joint pain"
    };

    // Find index of positive symptom
    int symp;
    string lowKey = key;
    transform(lowKey.begin(), lowKey.end(), lowKey.begin(), ::tolower);
    string bitstring = "";

    for (int i=0; i<16; i++) {
        string s = symptoms[i];
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == lowKey) {
            symp = i;
            bitstring += "1";
        } else {
            bitstring += "0";
        }
    }

    vector<pair<string,vector<string>>> results;
    pair<Leaf*, int> topResult = search(bitstring);
    if (topResult.first != nullptr) {
        vector<string> firstSymps;
        for (int i=0; i<16; i++) {
            if (topResult.first->keys[topResult.second].substr(i, 1) == "1") {
                firstSymps.push_back(symptoms[i]);
            }
        }
        results.push_back({topResult.first->values[topResult.second], firstSymps});
        int left = topResult.second - 1;
        int right = topResult.second + 1;
        Leaf* leftLeaf = topResult.first;
        Leaf* rightLeaf = topResult.first;

        // Add the top result and the closest results in descending order
        while (leftLeaf != nullptr || rightLeaf != nullptr){
            // Try adding from the right
            if (rightLeaf != nullptr) {
                if (right < rightLeaf->keyCount) {
                    // Valid index in current rightLeaf and has matching symptom
                    if (rightLeaf->keys[right].substr(symp,1) == "1") {
                        vector<string> symps;
                        for (int i=0; i<16; i++) {
                            if (rightLeaf->keys[right].substr(i, 1) == "1") {
                                symps.push_back(symptoms[i]);
                            }
                        }
                        results.push_back({rightLeaf->values[right], symps});
                    }
                    right++;
                    // Keep going for same symptom bitstring that matches exactly
                    if (rightLeaf->keys[right-1]==key) {
                        continue;
                    }
                } else {
                    // Index is out of bounds, move to the next leaf
                    rightLeaf = rightLeaf->next;
                    if (rightLeaf != nullptr) {
                        // Reset index to the start of the new next leaf
                        right = 0;
                    }
                    // If rightLeaf became nullptr, the outer `if (rightLeaf != nullptr)` will fail next time
                }
            }

            if (leftLeaf != nullptr) {
                if (left >= 0) {
                    // Valid index in current leftLeaf and has matching symptom
                    if (leftLeaf->keys[left].substr(symp,1) == "1") {
                        vector<string> symps;
                        for (int i=0; i<16; i++) {
                            if (leftLeaf->keys[left].substr(i, 1) == "1") {
                                symps.push_back(symptoms[i]);
                            }
                        }
                        results.push_back({leftLeaf->values[left], symps});
                    }
                    left--;
                } else {
                    // Index is out of bounds, move to the previous leaf
                    leftLeaf = leftLeaf->prev;
                    if (leftLeaf != nullptr) {
                        // Reset index to the end of the new previous leaf
                        left = leftLeaf->keyCount - 1;
                    }
                    // If leftLeaf became nullptr, the outer `if (leftLeaf != nullptr)` will fail next time
                }
            }

        }
    }
    return results;
    auto end = std::chrono::high_resolution_clock::now();
    sympRuntime =  static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}
