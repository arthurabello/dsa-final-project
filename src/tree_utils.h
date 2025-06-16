#ifndef TREEUTILS_H  
#define TREEUTILS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

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
        // Indexing info
        int docsIndexed;
        int wordsIndexed;

        // Insertion/indexing time
        int64_t totalInsertionTimeNs;
        int64_t avgInsertionTimeNs;
        int64_t maxInsertionTimeNs;
        int64_t minInsertionTimeNs;

        // Search time
        int64_t totalSearchTimeNs;
        int64_t avgSearchTimeNs;
        int64_t maxSearchTimeNs;
        int64_t minSearchTimeNs;

        // Search comparisons
        int totalComparisonsSearch;
        int avgComparisonsSearch;
        int maxComparisonsSearch;
        int minComparisonsSearch;

        // Tree structure
        int nodeCount;          // unique words
        int treeHeight;         // Max depth
        int treeMinDepth;       // Min depth
        int balanceDiff;        // Max depth - min depth
        double relativeBalance; // Max depth / min depth
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

	void deletionPostOrder(Node* n);

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

    double getAverageSearchTime(const AggregateStats& stats);
    /**
     * @brief Calculates the average search time.
     *
     * This function computes the average time taken for searches
     * based on the total sum of search times and the total number of searches.
     *
     * @param stats The AggregateStats object containing search time data.
     * @return The average search time, or 0.0 if no searches were performed.
     */

    double getAverageComparisonsPerSearch(const AggregateStats& stats);
    /**
     * @brief Calculates the average number of comparisons per search.
     *
     * This function computes the average number of comparisons made during
     * searches based on the total number of comparisons and the total
     * number of searches performed.
     *
     * @param stats The AggregateStats object containing search comparison data.
     * @return The average number of comparisons per search, or 0.0 if no searches were performed.
     */

    double getRelativeBalance(const AggregateStats& stats);
    /**
     * @brief Calculates the relative balance of the tree.
     *
     * @param stats The AggregateStats object containing tree structure data.
     * @return The relative balance of the tree, or 0.0 if the minimum depth is 0.
     */

    int getBalanceDifference(const AggregateStats& stats);
    /**
     * @brief Calculates the difference between the height of the tree and minimum depth.
     *
     * @param stats The AggregateStats object containing tree structure data.
     * @return The difference between the height of the tree and minimum depth.
     */
}

#endif
