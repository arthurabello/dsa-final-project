#include "../tree_utils.h"


namespace TREE::AVL{
	
	
	void leftRotation(Node* pivot);
	
	void rightRotation(Node* pivot);
	
	void balance(Node* grandpa);
	
	InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId);
}
