#include "bst.h"
#include "../tree_utils.h"
#include <vector>
#include <chrono>
#include <cstring>

namespace TREE::BST {

    InsertResult insert(BinaryTree* binary_tree, const std::string& word, int documentId){ //eliane
        InsertResult result;
        int comparisons = 0;
        auto start_time = std::chrono::high_resolution_clock::now();

        Node* newNode = nullptr;
        
        if(binary_tree->root == nullptr){
            newNode = createNode(word, {documentId});
            binary_tree->root = newNode;
        } else {
            Node* current = binary_tree->root;
            Node* parent = nullptr;

            while (current != nullptr){
                parent = current;
                comparisons++;

                if(word == current->word){
                    //checks if documentId has already been added
                    bool found = false;
                    for(size_t i = 0; i < current->documentIds.size(); i++){
                        if (current->documentIds[i] == documentId) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (found == false) {
                        current->documentIds.push_back(documentId);
                    }

                    auto end_time = std::chrono::high_resolution_clock::now();
                    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
    
                    result.numComparisons = comparisons;
                    result.executionTime = duration;
                    return result;

                } else if(word < current->word){
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            newNode = createNode(word, {documentId});
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }

            updateHeightUp(newNode);
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

        result.numComparisons = comparisons;
        result.executionTime = duration;
        return result; 
    }   
}
