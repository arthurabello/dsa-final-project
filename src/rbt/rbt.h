#ifndef RBT_H
#define RBT_H

#include <iostream>
#include <vector>
#include "../tree_utils.h"

// Here we define the necessary structures and functions for the Red-Black Tree (RBT)

namespace TREE::RBT {

    /**
     * @brief Inserts a word into the Red-Black Tree (RBT).
     *
     * If the word already exists in the tree, the document ID is added
     * to its list (if not already present). If the word doesn't exist, a new red node is created
     * and the tree is rebalanced using the Red-Black Tree insertion algorithm.
     * It also collects statistics like number of comparisons and execution time.
     *
     * @param binary_tree Pointer to the BinaryTree where the word will be inserted.
     * @param word The word to insert or update in the tree.
     * @param documentId The ID of the document where the word appears.
     *
     * @return InsertResult Struct containing the number of comparisons and the insertion time.
     */
    InsertResult insert(BinaryTree* binary_tree, const std::string& word, int documentId);

    /**
     * @brief Restores Red-Black Tree properties after inserting a node.
     *
     * After inserting a new node into the Red-Black Tree, this function ensures that
     * all Red-Black Tree properties are preserved by performing recoloring and rotations.
     *
     * @param root Double pointer to the root of the tree.
     * @param z Pointer to the newly inserted node.
     */
    void fixInsert(Node** root, Node* z);

    /**
     * @brief Returns the uncle of a given node in the Red-Black Tree.
     *
     * The uncle of a node is the sibling of its parent. This function returns the uncle
     * if it exists; otherwise, it returns nullptr.
     *
     * @param node Pointer to the node whose uncle is being retrieved.
     * @return Node* Pointer to the uncle node, or nullptr if not applicable.
     */
    Node* getUncle(Node* node);

    /**
     * @brief Returns the sibling of a given node in the Red-Black Tree.
     *
     * The sibling of a node is the other child of its parent. This function returns the sibling
     * if it exists; otherwise, it returns nullptr.
     *
     * @param node Pointer to the node whose sibling is being retrieved.
     * @return Node* Pointer to the sibling node, or nullptr if not applicable.
     */
    Node* getSibling(Node* node);

    // See tree_utils.h
    SearchResult search(BinaryTree* binary_tree, const std::string& word);
}

#endif
