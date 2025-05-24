#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>

// Here we will define necessary structures and functions for the binary search tree (BST)

namespace BST {
    
    struct Node {
    std::string word;
    std::vector<int> documentIds;
    Node* parent;
    Node* left;
    Node* right;
    int height;    // usado na AVL
    };

    struct BinaryTree {
        Node* root;
    };

    struct InsertResult {
        int numComparisons;
        double executionTime;
    };
    
    struct SearchResult {
        int found;
        std::vector<int> documentIds;
        double executionTime;
        int numComparisons;
    };

    BinaryTree* create();

        /**
     * @brief Inserts a word into the binary search tree.
     * 
     * Initializes the binary search tree with a nullptr root.
     */
   
    InsertResult insert(BinaryTree* tree, const std::string& word, int documentId);
    
    SearchResult search(BinaryTree* tree, const std::string& word);
    
    void deleteBinaryTree(BinaryTree* tree);
}

#endif