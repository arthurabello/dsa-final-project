#include "tree_utils.h"
#include <vector>
#include <cstring>

namespace TREE{
	// Sets for 0 if it the tree doesnt support red-black
    Node* createNode(std::string word, std::vector<int>documentIds, int color) {

        Node* newNode = new Node;
        newNode->word = word;
        newNode->documentIds = documentIds;
        newNode->parent = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 0; //height of a new node is 0
        newNode->isRed = color; //0 for red, 1 for black
        return newNode;
    }

    BinaryTree* createTree(){
        BinaryTree* newBinaryTree = new BinaryTree{nullptr};
        return newBinaryTree;
    }

    SearchResult search(BinaryTree* binary_tree, const std::string& word) {
        if (binary_tree == nullptr || binary_tree->root == nullptr) {
            return {0, {}, 0, 0};

        } else {
            Node* current_node = binary_tree->root;
            int number_of_comparisons = 0;

            while (current_node != nullptr) {
                number_of_comparisons++;

                int compareResult = strcmp(word.c_str(), current_node->word.c_str());

                if (compareResult == 0) { //found!
                    return {1, current_node->documentIds, 0, number_of_comparisons};

                } else if (compareResult < 0) {
                    current_node = current_node->left; //go left because word is smaller
                } else {
                    current_node = current_node->right; //go right because word is bigger
                }
            }

            //if word not found
            return {0, {}, 0, number_of_comparisons};
        }
    }

    void deletionPostOrder(Node* n){
        if(n != nullptr){
            deletionPostOrder(n->left);
            deletionPostOrder(n->right);
            delete n;
        }
    }
    
    void destroy(BinaryTree* binary_tree){
        Node* root = binary_tree->root;
		    deletionPostOrder(root);
		    delete binary_tree;
    }

    int calculateHeight(Node* root){
        //Treats the case in which the root is empty.
        if(root == nullptr) return -1; //I fixed this
        
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
		
		int originalHeight = node->height;
		
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
		
        int maxHeight = std::max(leftHeight,rightHeight);

        int newHeight = 1 + maxHeight;

        node->height = newHeight;
		
        if (originalHeight == newHeight && (node->right != nullptr || node->left != nullptr)) {
            return;
        }
		
        // Calculates the height of the father by a recursive call
        updateHeightUp(node->parent);
    }

    int calculateMinDepth(Node* root) {
        if (root == nullptr) {
            return 0;
        }

        // If is a leaf
        if (root->left == nullptr && root->right == nullptr) {
            return 0;
        }

        // Recurse only on the right subtree
        if (root->left == nullptr) {
            return 1 + calculateMinDepth(root->right);
        }

        // Recurse only on the left subtree
        if (root->right == nullptr) {
            return 1 + calculateMinDepth(root->left);
        }

        // Smaller depth between left and right
        int leftDepth = calculateMinDepth(root->left);
        int rightDepth = calculateMinDepth(root->right);

        return 1 + std::min(leftDepth, rightDepth);
    }

    int countNodes(Node* root) {
        if (root == nullptr) {
            return 0;
        }

        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    void rotateLeft(Node** root, Node* x) {
        // y becomes the new root of the subtree.
        Node* y = x->right;

        // Turn y's left subtree into x's right subtree.
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }

        // Link x's original parent to y.
        y->parent = x->parent;
        if (x->parent == nullptr) {
            // If x was the root, y is now the new root of the entire tree.
            *root = y;
        } else if (x == x->parent->left) {
            // If x was a left child, y becomes the new left child.
            x->parent->left = y;
        } else {
            // If x was a right child, y becomes the new right child.
            x->parent->right = y;
        }
        // Put x on y's left.
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node** root, Node* y) {
        // x becomes the new root of the subtree.
        Node* x = y->left;

        // Turn x's right subtree into y's left subtree.
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;

        }
        // Link y's original parent to x.
        x->parent = y->parent;
        if (y->parent == nullptr) {
            // If y was the root, x is now the new root of the entire tree.
            *root = x;
        } else if (y == y->parent->left) {
            // If y was a left child, x becomes the new left child
            y->parent->left = x;
        } else {
            // If y was a right child, x becomes the new right child.
            y->parent->right = x;
        }
        // Put y on x's right.
        x->right = y;
        y->parent = x;
    }
}

