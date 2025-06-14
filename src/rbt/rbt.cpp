#include "rbt.h"
#include "../tree_utils.h"
#include <vector>
#include <chrono>
#include <cstring>

namespace TREE::RBT {

    InsertResult insert(BinaryTree* binary_tree, const std::string& word, int documentId){
        insertResult result;
        auto start_time = std::chrono::high_resolution_clock::now();
        int num_comparisons = 0;
        
        Node* newNode = createNode(word, {documentId}, 1); // New node is always red
        Node* current = binary_tree->root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            num_comparisons++;
            int compareResult = strcmp(word.c_str(), current->word.c_str());
            if (compareResult < 0) {
                current = current->left;
            } else if (compareResult > 0) {
                current = current->right;
            } else {
                // Word already exists, add documentId and return
                current->documentIds.push_back(documentId);
                delete newNode; // Don't need the new node
                
                auto end_time = std::chrono::high_resolution_clock::now();
                double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

                result.numComparisons = comparisons;
                result.executionTime = duration;
                return result; 
            }
        }

        newNode->parent = parent;
        
        if (parent == nullptr) {
            binary_tree->root = newNode;
        } else {
            num_comparisons++; // Comparison to decide left/right child
            if(word < parent->word){
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }

        fixInsert(&binary_tree->root, newNode);

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

        result.numComparisons = comparisons;
        result.executionTime = duration;
        return result; 
    }

    void fixInsert(Node** root, Node* z) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        Node* uncle = nullptr;

        while (z != *root && z->parent->isRed) {
            parent = z->parent;
            grandparent = z->parent->parent;
            uncle = getUncle(z);
            
            if (uncle != nullptr && uncle->isRed) {
                // Case 1: Uncle is red
                uncle->isRed = 0;
                parent->isRed = 0;
                grandparent->isRed = 1;
                z = grandparent;

            } else {
                // Case 2: Uncle is black
                if (z == parent->right && parent == grandparent->left) { 
                    // Case 2.1: Right-left 
                    rotateLeft(root, parent);
                    z = parent;
                    parent = z->parent;
                } else if (z == parent->left && parent == grandparent->right) { 
                    // Case 2.2: Left-right
                    rotateRight(root, parent);
                    z = parent;
                    parent = z->parent;
                } else {
                    // Case 2.3: Left-left or Right-right
                    if (z == parent->left) {
                        // Case 2.3.1: Left-left
                        rotateRight(root, grandparent);
                    } else {
                        // Case 2.3.2: Right-right
                        rotateLeft(root, grandparent);
                    }
                    parent->isRed = 0;
                    grandparent->isRed = 1;
                    z = grandparent;
                }
            }

        }
        root->isRed = 0;
    }

    Node* getUncle(Node* node) {
        if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr) {
            return nullptr;
        }

        Node* grandparent = node->parent->parent;
        if (grandparent->left == node->parent) {
            return grandparent->right;
        } else {
            return grandparent->left;
        }
    }

    Node* getSibling(Node* node) {
        if (node == nullptr || node->parent == nullptr) {
            return nullptr;
        }

        if (node->parent->left == node) {
            return node->parent->right;
        } else {
            return node->parent->left;
        }
    }

    SearchResult search(BinaryTree* binary_tree, const std::string& word) {
        return TREE::search(binary_tree, word);
    }
}