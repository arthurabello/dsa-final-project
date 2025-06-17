#ifndef AVL_H
#define AVL_H

#include "../tree_utils.h"


namespace TREE::AVL{
	
	/**
    * @brief Gets the height of a node
	*
    * @param node: Pointer to the node whose you wanna know the height.
    *
    * @return The height of the node
    */
	int getHeight(Node* node);
	
	/**
	* @brief Calculates the balancing factor of a node. 
	*
	* @param node: Pointer to the node whose you wanna know the height. 
	*
	* @return The balancing factor of the node.
	*/
	int bf(Node* node);
	
	/**
	* @brief Updates the heights of the nodes 
	* above the node added, returning the first
	* unbalanced node.
	*
	* @param node: new node added to the tree.
	*
	* @return The unbalanced node.
	*/
	Node* updateHeightUp(Node* node);

	/**
	* @brief Realizes a left rotation on the tree given a certain
	* pivot to realize it.
	*
	* @param tree: Reference to the tree.
	* @param pivot: Node that we will realize the left rotation.
	*/
	void leftRotation(BinaryTree& tree, Node* pivot);
	
	/**
	* @brief Realizes a left rotation on the tree given a certain
	* pivot to realize it.
	*
	* @param tree: Reference to the tree.
	* @param pivot: Node that we will realize the left rotation.
	*/
	void rightRotation(BinaryTree& tree, Node* pivot);
	
	/**
	* @brief Balances the tree, given the first unbalanced node.
	*
	* @param tree: Reference to the tree.
	* @param desbalancedNode: First unbalanced node of the tree
	* to appear in a DFS pre-order traversal.
	*/
	void balanceTree(BinaryTree& tree,Node* desbalancedNode);
	
	/**
    * @brief Inserts a word into the AVL tree.
    *
    * If the word already exists in the tree, the document ID is added
    * to its list (if not already present). If the word doesn't exist, a new node is created.
    * It also collects statistics like number of comparisons and execution time.
    *
    * @param binary_tree Pointer to the BinaryTree where the word will be inserted.
    * @param word The word to insert or update in the tree.
    * @param documentId The ID of the document where the word appears.
    *
    * @return InsertResult Struct containing the number of comparisons and the insertion time.
    */
	InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId);
	
	// See tree_utils.h
    SearchResult search(BinaryTree* binary_tree, const std::string& word);
}

#endif