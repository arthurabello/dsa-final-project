#include "../tree_utils.h"
#include <chrono>
#include "avl.h"

namespace TREE::AVL {

	int getHeight(Node* node) {
	return node ? node->height : -1;
	}

	// Balance factor: H(right) - H(left)
	int bf(Node* node) {
	return getHeight(node->right) - getHeight(node->left);
	}

	Node* updateHeightUp(Node* node) {

	if (node == nullptr) {
	return nullptr;
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

	// Deal with the cases of immediate return.
	if (originalHeight == newHeight &&
	(node->right != nullptr ||
	node->left != nullptr)) return nullptr;

	if(std::abs(bf(node))>=2) return node;

	// If not returned, analyzes the father.
	return AVL::updateHeightUp(node->parent);

	}

	void leftRotation(BinaryTree& tree, Node* pivot) {
	Node* L = pivot->left;
	Node* grandpa = pivot->parent->parent;
	Node* papa = pivot->parent;

	if (L != nullptr) L->parent = papa;
	papa->right = L;

	// Begin changing pivot and papa of place. 
	pivot->parent = grandpa;
	if (grandpa != nullptr) {
	if(grandpa->left == papa) grandpa->left = pivot;
	else grandpa->right = pivot;
	}

	// Finishes changing pivot and papa of places.
	pivot->left = papa;
	papa->parent = pivot;

	if (tree.root == papa) tree.root = pivot;

	// Updates the heights of the nodes.
	papa->height = 1 + std::max(getHeight(papa->left), getHeight(papa->right));
	pivot->height = 1 + std::max(getHeight(pivot->left), getHeight(pivot->right));

	}

	void rightRotation(BinaryTree& tree, Node* pivot) {
	// Analogous to previous function.
	Node* R = pivot->right;
	Node* grandpa = pivot->parent->parent;
	Node* papa = pivot->parent;

	if (R != nullptr) R->parent = papa;
	papa->left = R;

	pivot->parent = grandpa;
	if (grandpa != nullptr) {
	if(grandpa->left == papa) grandpa->left = pivot;
	else grandpa->right = pivot;
	}

	pivot->right = papa;
	papa->parent = pivot;

	if (tree.root == papa) tree.root = pivot;

	papa->height = 1 + std::max(getHeight(papa->left), getHeight(papa->right));
	pivot->height = 1 + std::max(getHeight(pivot->left), getHeight(pivot->right));
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

	auto end_time = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration_cast < std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

	result.numComparisons = comparisons;
	result.executionTime = duration;
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

	auto end_time = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration_cast < std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

	result.numComparisons = comparisons;
	result.executionTime = duration;
	return result;
	}

	SearchResult search(BinaryTree* binary_tree, const std::string& word) {
	return TREE::search(binary_tree, word);
	}
}