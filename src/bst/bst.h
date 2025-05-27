#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include "tree_utils.h"
//here we will define necessary structures and functions for the classical binary search tree (BST)

namespace TREE::BST {

    /**
    * @brief Inserts a word into the binary search tree.
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
    InsertResult insert(BinaryTree* binary_tree, const std::string& word, int documentId);

}

#endif