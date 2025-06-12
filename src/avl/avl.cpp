#include "../tree_utils.h"
#include <chrono>

namespace TREE::AVL {

    int getHeight(Node* node) {
        return node ? node->height : -1;
    }
    
    // Balance factor: H(left) - H(right)
    int bf(Node* node) {
        return getHeight(node->right) - getHeight(node->left);
    }
    
    void leftRotation(BinaryTree& tree, Node* pivot) {
		Node* L = pivot->left;
		Node* grandpa = pivot->parent->parent;
        Node* papa = pivot->parent;
		
		if (L != nullptr) L->parent = papa;
		papa->right = L;
		
    	
    	// Guarantees that the tree is still a BST
    	// if (R->left!=nullptr) R->left->parent = pivot;
    	
		pivot->parent = grandpa;
		if (grandpa != nullptr) {
			if(grandpa->left == papa) grandpa->left = pivot;
			else grandpa->right = pivot;
		}
		
    	// Finishes changing pivot and R of places
    	pivot->left = papa;
		papa->parent = pivot;
		
    	if (tree.root == papa) tree.root = pivot;
    	
    	// Updates the heights of the nodes
    	papa->height = 1 + std::max(getHeight(papa->left), getHeight(papa->right));
    	pivot->height = 1 + std::max(getHeight(pivot->left), getHeight(pivot->right));
    	
    }
	
    void rightRotation(BinaryTree& tree, Node* pivot) {
		Node* R = pivot->right;
		Node* grandpa = pivot->parent->parent;
        Node* papa = pivot->parent;
		
		if (R != nullptr) R->parent = papa;
		papa->left = R;
		
    	// Guarantees that the tree is still a BST
    	// if (R->left!=nullptr) R->left->parent = pivot;
    	
		pivot->parent = grandpa;
		if (grandpa != nullptr) {
			if(grandpa->left == papa) grandpa->left = pivot;
			else grandpa->right = pivot;
		}
		
    	// Finishes changing pivot and R of places
    	pivot->right = papa;
		papa->parent = pivot;
		
    	if (tree.root == papa) tree.root = pivot;
    	
    	// Updates the heights of the nodes
    	papa->height = 1 + std::max(getHeight(papa->left), getHeight(papa->right));
    	pivot->height = 1 + std::max(getHeight(pivot->left), getHeight(pivot->right));
    }
	
    void balanceTree(BinaryTree& tree, Node* unbalancedNode) {
        if(tree.root == nullptr) return;
        // In this case, the tree still balanced
		if (unbalancedNode == nullptr)  return;
		
        // Cases of RR or LR rotations
        if(bf(unbalancedNode)<-1) {
        
			if (bf(unbalancedNode->left)<0)
                rightRotation(tree, unbalancedNode->left); return;
            
            if (bf(unbalancedNode->left)>0){
                leftRotation(tree, unbalancedNode->left->right);
                rightRotation(tree, unbalancedNode->left);
				return;
            }
        }
		// Cases of LL or RL rotations
        if(bf(unbalancedNode)>1) {
            if (bf(unbalancedNode->right)>0){ 
				leftRotation(tree, unbalancedNode->right);
				return;
        	}
            if (bf(unbalancedNode->right)<0) {
                rightRotation(tree, unbalancedNode->right->left);
                leftRotation(tree, unbalancedNode->right);
				return;
            }
        }
    
	}
    
    InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId) {
        InsertResult result;
        int comparisons = 0;
        auto start_time = std::chrono::high_resolution_clock::now();


        if (binary_tree.root == nullptr) {
            binary_tree.root = createNode(word, {documentId});
        }
        else {
            // Adds the node to the tree as usual
            Node* current = binary_tree.root;
            Node* parent = nullptr;
    	
            while (current != nullptr) { // Searches for the word
                comparisons++;
                parent = current;

                if (word == current->word) {
                    // If it's already in the tree, updates the documentsId
                    bool found = false;
                    for(size_t i = 0; i < current->documentIds.size(); i++){
                        if (current->documentIds[i] == documentId) {
                            found = true;
                            break;
                        }
                    } 
                    if (found == false) {
                        current->documentIds.push_back(documentId);
						break;
                    }
                }
                else if (word < current->word) {
                    current = current->left;
                    continue;
                }
                else if(word > current->word) {
                    current = current->right;
                    continue;
                }
            }
			
			Node* newNode = createNode(word, {documentId});
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else if(word>parent->word) {
                parent->right = newNode;
            }
			
			updateHeightUp(newNode);
			
			//Balancing moment
			Node* unbalancedNode = parent;
			
			
			
			while(unbalancedNode->parent != nullptr && std::abs(bf(unbalancedNode)) < 2)
				unbalancedNode = unbalancedNode->parent;
			
			balanceTree(binary_tree, unbalancedNode);
		}
		
		auto end_time = std::chrono::high_resolution_clock::now();
		double duration = std::chrono::duration_cast < std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

		result.numComparisons = comparisons;
		result.executionTime = duration;
		return result;
    }
}
