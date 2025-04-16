#include "trieSymp.h"

TrieSympNode::TrieSympNode() : isEndOfWord(false) {
    for (int i = 0; i < 26; i++) {
        children[i] = nullptr;
    }
}