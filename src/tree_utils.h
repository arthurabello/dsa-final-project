#ifndef TREEUTILS_H
#define TREEUTILS_H

#include <iostream>
#include <vector>
#include <fstream>

namespace TREE {
    /**
     * @brief Here we present the creation of a more general BST
     * 
     * The BST will have substructs: The RBT and AVL tree
     */
    
    struct Node {
        std::string word;
        std::vector<int> documentIds;
        Node* parent;
        Node* left;
        Node* right;
        int height;
        int isRed;
    };

    struct BinaryTree {
        Node* root;
    };

    struct InsertResult {
        int numComparisons;
        double executionTime;
    };
    
    struct SearchResult {
        int found;
        std::vector<int> documentIds;
        double executionTime;
        int numComparisons;
    };

    struct AggregateStats {
        std::string tree_type;
        int num_docs_indexed;

        long long total_indexing_time_ms = 0;
        long long total_words_processed = 0;
        long long total_comparisons_insertion = 0;
        double sum_of_insertion_times_ms = 0.0;

        int final_node_count = 0; // unique words
        int final_tree_height = 0;
    };

    BinaryTree* createTree();

    /**
     * @brief Creates a new binary search tree.
     *
     * @returns the tree
     */

    Node* createNode(std::string word,std::vector<int>documentIds,int color = 0);
    /**
    * @brief Creates a new node of a tree.
    *
    * @param word: word to be put in the node.
    * @param documentIds: ID of the document in which the word appears.
    * @param color: color of the Node. 0 for black and 1 for red.

    * @returns Node*: Node with the passed word and documentIds
    */
    
    SearchResult search(BinaryTree* binary_tree, const std::string& word);

    /**
     * @brief Searches for a given word in a binary search tree.
     * 
     * This function performs a search for the specified `word` within the provided 
     * binary search tree. It traverses the tree starting from the root, comparing 
     * the target word with the current node's word, moving left or right accordingly,
     * until the word is found or the tree is fully traversed.
     * 
     * The function also measures and returns the time taken to perform the search 
     * (in milliseconds) as well as the number of comparisons made during the search.
     * 
     * @param binary_tree Pointer to the binary search tree to search within.
     * @param word The target word to search for.
     * 
     * @return SearchResult A struct containing:
     *  - found (int): 1 if the word was found, 0 otherwise.
     *  - documentIds (std::vector<int>): List of document IDs associated with the found word. Empty if not found.
     *  - duration (double): Time taken for the search in milliseconds.
     *  - number_of_comparisons (int): Total number of node comparisons performed.
     */

     void destroy(BinaryTree* binary_tree);
     /**
     * @brief Delete the passed binary tree    
     *
     * @param binary_tree: BinaryTree you want to delete.  
     */


    int calculateHeight(Node* root);
    /**
     * @brief Calculates the height of a binary tree.
     * 
     * @param binary_tree: BinaryTree you wish to know the height.BinaryTree.
     * @return height of binary_tree.
     * 
     */

    void updateHeightUp(Node* node); 
    /**
    * @brief Recursively updates the height of a node up to the root.
    *
    * This function walks the path from a node to the root of the tree,
    * recomputing the height of each node based on the heights of its
    * left and right children. If the height of the node does not change,
    * the recursion ends early, since no further modifications are needed 
    * to the ancestors.
    *
    * @param node Pointer to the node from which the update should start.
    * @note This function **does not recursively update the children**.
    * It assumes that the heights of the children are already correct, which is the case
    * after a simple insert.
    */

    // TODO
    void save_stats_to_csv(const AggregateStats& stats, const std::string& filename = "results.csv");

    int calculateMinDepth(Node* root);
    /**
     * @brief Calculates the minimum depth of a binary tree.
     *
     * This function recursively calculates the minimum depth from the root
     * to the nearest leaf node (node with no children).
     *
     * @param root Pointer to the root node of the tree.
     * @return The minimum depth of the tree. Returns 0 if the tree is empty.
     */

    int countNodes(Node* root);
    /**
     * @brief Counts the total number of nodes in a binary tree.
     *
     * This function traverses the tree recursively and counts each node.
     *
     * @param root Pointer to the root node of the tree.
     * @return The total number of nodes in the tree.
     */

    void updateFinalNodeCount(AggregateStats& stats, BinaryTree* tree);
    /**
     * @brief Updates the final node count in the AggregateStats.
     *
     * This function calculates the total number of nodes in the given
     * binary tree and updates the 'final_node_count' field in the
     * AggregateStats object.
     *
     * @param stats Reference to the AggregateStats object to update.
     * @param tree Pointer to the BinaryTree.
     */

    void updateFinalTreeHeight(AggregateStats& stats, BinaryTree* tree);
    /**
     * @brief Updates the final tree height in the AggregateStats.
     *
     * This function calculates the height of the given binary tree
     * and updates the 'final_tree_height' field in the AggregateStats object.
     *
     * @param stats Reference to the AggregateStats object to update.
     * @param tree Pointer to the BinaryTree.
     */

    double getAverageInsertionTime(const AggregateStats& stats);
    /**
     * @brief Calculates the average insertion time.
     *
     * This function computes the average time taken for insertions
     * based on the total sum of insertion times and the total number
     * of words processed.
     *
     * @param stats The AggregateStats object containing insertion time data.
     * @return The average insertion time, or 0.0 if no words were processed.
     */

    double getAverageComparisonsPerInsertion(const AggregateStats& stats);
    /**
     * @brief Calculates the average number of comparisons per insertion.
     *
     * This function computes the average number of comparisons made during
     * insertions based on the total number of comparisons and the total
     * number of words processed.
     *
     * @param stats The AggregateStats object containing insertion comparison data.
     * @return The average number of comparisons per insertion, or 0.0 if no words were processed.
     */

}

#endif
