#include "tree_utils.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>          

namespace TREE {

    Node* createNode(std::string word, std::vector<int>documentIds, int color = 0) { //sets for 0 if it the tree doesnt support red-black, gabriel carneiro
        
        Node* newNode = new Node;
        newNode->word = word;
        newNode->documentIds = documentIds;
        newNode->parent = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1; //height of a new node is 1
        newNode->isRed = color; //0 for red, 1 for black
        return newNode;
    }

    BinaryTree* createTree(){ //artu
        BinaryTree* newBinaryTree = new BinaryTree{nullptr};
        return newBinaryTree;
    }

    SearchResult search(BinaryTree* binary_tree, const std::string& word) { //artu
        auto start_time = std::chrono::high_resolution_clock::now(); //start measuring time
        
        if (binary_tree == nullptr || binary_tree->root == nullptr) {
            auto end_time = std::chrono::high_resolution_clock::now(); //done lol
            double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
            return {0, {}, duration, 0};
            
        } else {
            Node* current_node = binary_tree->root;
            int number_of_comparisons = 0;
            
            while (current_node != nullptr) {
                number_of_comparisons++;
                
                int compareResult = strcmp(word.c_str(), current_node->word.c_str());
                
                if (compareResult == 0) { //found!
                    auto end_time = std::chrono::high_resolution_clock::now();
                    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
                    return {1, current_node->documentIds, duration, number_of_comparisons};

                } else if (compareResult < 0) {
                    current_node = current_node->left; //go left because word is smaller
                } else {
                    current_node = current_node->right; //go right because word is bigger
                }
            }
            
            //if word not found
            auto end_time = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
            return {0, {}, duration, number_of_comparisons};
        }
    }

    void deleteBinaryTree(BinaryTree* binary_tree){ //gabriel carneiro
        Node* root = binary_tree->root;
        
        if(root != nullptr){
            Node* leftNode = root->left;
            BinaryTree* leftSubTree = createTree();
            leftSubTree->root = leftNode;
            
            Node* rightNode = root->right;
            BinaryTree* rightSubTree = createTree();
            rightSubTree->root = rightNode;
            
            delete root;

            deleteBinaryTree(leftSubTree);
            deleteBinaryTree(rightSubTree);

            delete binary_tree;
        }
    }
}