#include "../tree_utils.h"

namespace TREE::AVL {

    int getHeight(Node* node) {
        return node ? node->height : -1;
    }
    
    // Balance factor: H(right) - H(left)
    int bf(Node* node) {
        return getHeight(node->right) - getHeight(node->left);
    }
	
    void balanceTree(BinaryTree& tree, Node* unbalancedNode) {
        if(tree.root == nullptr) return;
        // In this case, the tree still balanced
		if (unbalancedNode == nullptr)  return;
		
        // Cases of RR or LR rotations
        if(bf(unbalancedNode)<-1) {
        
			if (bf(unbalancedNode->left)<0) {
                rightRotation(tree, unbalancedNode->left);
				return;
            }
            if (bf(unbalancedNode->left)>0) {
                leftRotation(tree, unbalancedNode->left->right);
                rightRotation(tree, unbalancedNode->left);
				return;
            }
        }
		// Cases of LL or RL rotations
        if(bf(unbalancedNode)>1) {
            if (bf(unbalancedNode->right)>0) { 
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

        if (binary_tree.root == nullptr) {
            binary_tree.root = createNode(word, {documentId});
        }
        else {
            // Adds the node to the tree as usual
            Node* current = binary_tree.root;
            Node* parent = nullptr;
    	
            while (current != nullptr) { // Searches for the word
                comparisons++;
				// Saves the parent of the inserted node.
                parent = current;

                if (word == current->word) {
                    // If it's already in the tree, just updates the documentIds. 
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
					
					result.numComparisons = comparisons;
					return result;
					
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
			// Insertion of the node.
			Node* newNode = createNode(word, {documentId});
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else if(word>parent->word) {
                parent->right = newNode;
            }
			
			// Balancing the tree.
			Node* unbalancedNode = AVL::updateHeightUp(newNode); 
			
			balanceTree(binary_tree, unbalancedNode);
		}
		
		result.numComparisons = comparisons;
		return result;
    }

    SearchResult search(BinaryTree* binary_tree, const std::string& word) {
        return TREE::search(binary_tree, word);
    }
}
