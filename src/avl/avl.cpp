#include "../tree_utils.cpp"
#include <chrono>

using namespace std::chrono;

namespace TREE::AVL {
	
	int bf(Node* node) {//gabriel carneiro
		int lHeight,rHeight;
		
		lHeight = node->left == nullptr? 0 : node->left->height; 
		rHeight = node->right == nullptr? 0 : node->right->height;		
		
		return lHeight - rHeight;
	}
	
	//I gotta calculate the heights missing here
	void transplant(Node* A, Node* B) {//srry Mateus I stole your idea;  gabriel carneiro
		if (B != A->left && B != A->right) return;
		
		Node* grandpa = A->parent;
		
		if (grandpa == nullptr) {
			if (B == A->left) A->left = nullptr;
			else if (B == A->right) A->right = nullptr;
		}
		
		if (A == grandpa->left) {
			B = grandpa->left;
		}
		else if (A == grandpa->right) {
			B = grandpa->right;
		}
	}
	
	void leftRotation(Node* pivot) {//gabriel carneiro
		Node* papa = pivot->parent;
		papa->right = pivot->left;
		transplant(papa,pivot);
		pivot->left = papa;
	}
	
	void rightRotation(Node* pivot) {//gabriel carneiro
		Node* papa = pivot->parent;
		papa->left = pivot->right;
		transplant(papa,pivot);
		pivot->right = papa;
	}
	void balanceTree(Node* desbalancedNode) {//gabriel carneiro
		// I think I gotta fix the heights of the nodes bruh.
		if(desbalancedNode == nullptr) return;
		
		if (std::abs(bf(desbalancedNode))<=1) return;		
		
		if(bf(desbalancedNode)>0) {
			if (bf(desbalancedNode->left)>0) {
				rightRotation(desbalancedNode->left);
				
			}
			if (bf(desbalancedNode->left)<0) {
				leftRotation(desbalancedNode->left->right);
				rightRotation(desbalancedNode->left);
			}
		}
		
		if(bf(grandpa)<0) {
			if (bf(grandpa->right)<0) leftRotation(grandpa->right);
			
			if (bf(grandpa->right)>0) {
				rightRotation(grandpa->right->left);
				leftRotation(grandpa->right);
			}
			
		}
		
		//else if (bf(grandpa)*bf(grandpa->left)>0) rightRotation(grandpa->left);
		//else if (bf(grandpa)*bf(grandpa->right)>0) leftRotation(grandpa->right);
		
	}
	
	
    InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId) {
        InsertResult result;
        int comparisons = 0;
        auto start_time = high_resolution_clock::now();

        Node& newNode = createNode(word, {documentId});

        if(binary_tree.root == nullptr) {
            binary_tree.root = &newNode;
        } else {
            Node* current = binary_tree.root;            
			Node* parent = nullptr;
            while (current != nullptr) {
				comparisons++;
				parent = current;
				
                if (word < current->word) {
                    current = current->left;
                    continue;
                }
				else if(word > current->word) {
					current = current->right;
				} else {
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
					
				}
				
            }
			newNode = createNode(word, {documentId});
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
			
			updateHeightUp(newNode); //idea: maybe i'll already balance the tree here?
			
			Node* desbalance = newNode;
			
			while(std::abs(bf(desbalance))<=1) desbalance = desbalance ->parent;
			
			balanceTree(desbalance); //implement this function, with all the rotation combinations possible
		
		
		}

        return result;
    }
}
