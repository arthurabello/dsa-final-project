#include "tree_utils.h"
#include <vector>
#include <chrono>
#include <cstring>          
#include <algorithm>
namespace TREE {

    Node* createNode(std::string word, std::vector<int>documentIds, int color) { //sets for 0 if it the tree doesnt support red-black
        
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

    BinaryTree* createTree(){ 
        BinaryTree* newBinaryTree = new BinaryTree{nullptr};
        return newBinaryTree;
    }

    SearchResult search(BinaryTree* binary_tree, const std::string& word) {
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

	void deletionPostOrder(Node* n){
		if(n != nullptr){
			deletionPostOrder(n->left);
			deletionPostOrder(n->right);
			delete n;
		}
		delete n;
	}
	
    void destroy(BinaryTree* binary_tree){
        Node* root = binary_tree->root;
		deletionPostOrder(root);
		delete binary_tree;
    }

    int calculateHeight(Node* root){
        //Treats the case in which the root is empty.
        if(root == nullptr) return 0;
        
        //Copies the left subtree.
        Node* leftNode = root->left;
        
        //Copies the right subtree.
        Node* rightNode = root->right;
        
        //Calculate the height of the tree by a recursive call.
        int height = 1 + std::max(calculateHeight(rightNode),calculateHeight(leftNode));

        return height;
    }

    void updateHeightUp(Node* node) {
        if (node == nullptr) {
            return;
        }

        // Calculates the heights of the children and get the max
        int leftHeight;
        if (node->left == nullptr) {
            leftHeight = -1;
        } else {
            leftHeight = node->left->height;
        }

        int rightHeight;
        if (node->right == nullptr) {
            rightHeight = -1;
        } else {
            rightHeight = node->right->height;
        }

        int maxHeight;
        if (leftHeight > rightHeight) {
            maxHeight = leftHeight;
        } else {
            maxHeight = rightHeight;
        }

        int newHeight = 1 + maxHeight;

        if (node->height == newHeight) {
            return;
        }

        node->height = newHeight;

        // Calculates the height of the father by a recursive call
        updateHeightUp(node->parent);
    }

    // TODO
    void save_stats_to_csv(const AggregateStats& stats, const std::string& filename = "results.csv"){
        return;
    }

}
