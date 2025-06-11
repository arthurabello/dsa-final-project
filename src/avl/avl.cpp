#include "../tree_utils.cpp"
#include <chrono>


namespace TREE::AVL {
	
	int getHeight(Node* node) {
		return node ? node->height:0;
	}
	
	int bf(Node* node) {//gabriel carneiro
		return getHeight(node->left)-getHeight(node->right);
	}
	
	void leftRotation(BinaryTree& tree, Node* pivot) {//gabriel carneiro
		
		Node* R = pivot->right;
		
		//Start changing pivot and R of places.
		R->parent = pivot->parent;
		if (pivot->parent) {
			if (pivot->parent->left == pivot) pivot->parent->left = R;
			else pivot->parent->right = R;
		}
		
		//Garantees that still a BST.
		if(R->left){
			R->left->parent = pivot;
		}
		
		//Finishes changing pivot and R of places.
		pivot->right = R->left;
		pivot->parent = R;
		
		if (pivot == tree.root) tree.root = R;
		
		//Update the heights of the nodes.
		pivot->height = 1+std::max(getHeight(pivot->left),getHeight(pivot->right));
		R->height = 1+std::max(getHeight(R->left),getHeight(R->right));
		
	}
	
	void rightRotation(BinaryTree& tree, Node* pivot) {//gabriel carneiro
		Node* L = pivot->left;
		
		//Start changing pivot and L of places.
		L->parent = pivot->parent;
		if (pivot->parent) {
			if (pivot->parent->left == pivot) pivot->parent->left = L;
			else pivot->parent->right = L;
		}
		
		//Garantees that still a BST.
		if (L->right) L->right->parent = pivot;
		
		//Finishes changing pivot and L of places.
		pivot->left = L->right;
		pivot->parent = L;
		
		if (pivot == tree.root) tree.root = L;

		//Update the heights of the nodes.		
		pivot->height = 1+std::max(getHeight(pivot->left),getHeight(pivot->right));
		L->height = 1+std::max(getHeight(L->left),getHeight(L->right));
	}
	
	void balanceTree(BinaryTree& tree,Node* desbalancedNode) {//gabriel carneiro
		if(tree.root == nullptr) return;
		
		//In this case, the tree still balanced.
		if (desbalancedNode) return;		
		
		//Cases of RR or LR rotations.
		if(bf(desbalancedNode)>1) {
			if (bf(desbalancedNode->left)>0) rightRotation(tree,desbalancedNode);
			
			if (bf(desbalancedNode->left)<0) {
				leftRotation(tree,desbalancedNode->left);
				rightRotation(tree,desbalancedNode->left);
			}
		}
		//Cases of LL or RL rotations.
		if(bf(desbalancedNode)<0) {
			if (bf(desbalancedNode->right)<0) leftRotation(tree,desbalancedNode->right);
			
			if (bf(desbalancedNode->right)>0) {
				rightRotation(tree,desbalancedNode->right);
				leftRotation(tree,desbalancedNode->right);
			}
			
		}
	}
		
    InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId) {
        InsertResult result;
        int comparisons = 0;
        auto start_time = std::chrono::high_resolution_clock::now();

        Node* newNode = createNode(word, {documentId});

        if(binary_tree.root == nullptr) {
            binary_tree.root = newNode;
        } else {//Add normaly the node to the tree.
            Node* current = binary_tree.root;            
			Node* parent = nullptr;
			
            while (current != nullptr) {//search for the word.
				comparisons++;
				parent = current;
				
                if (word < current->word) {
                    current = current->left;
                    continue;
                }
				else if(word > current->word) {
					current = current->right;
					continue;
				} else {//if its already in the tree, updates the documentsId.
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
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
			
			updateHeightUp(newNode);
			
			
			//Balancing moment
			Node* desbalancedNode = parent;
			
			while(desbalancedNode != nullptr && std::abs(bf(desbalancedNode))<1) desbalancedNode = desbalancedNode->parent;
			
			balanceTree(binary_tree,desbalancedNode);	
		}

        return result;
    }
}
