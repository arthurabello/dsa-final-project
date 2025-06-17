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
        int rotations = 0; // for rbt
    };

    struct SearchResult {
        int found;
        std::vector<int> documentIds;
        double executionTime;
        int numComparisons;
    };

    struct AggregateStats {
        std::string tree_type;
        int num_docs_indexed = 0;

        // Insertion data
        long long total_indexing_time_ms = 0;
        long long total_words_processed = 0;
        long long total_comparisons_insertion = 0;
        double sum_of_insertion_times_ms = 0.0;
        double max_insertion_time_ms = 0.0;

        // Search data
        double total_search_time_ms = 0.0;
        long long total_searches = 0;
        long long total_comparisons_search = 0;
        double sum_of_search_times_ms = 0.0;
        double max_search_time_ms = 0.0;

        // Tree structure
        int final_node_count = 0;       // Unique words
        int final_tree_height = 0;      // max depth
        int final_tree_min_depth = 0;   // min depth
        double relative_balance = 0.0;  // max_depth / min_depth
        int balance_difference = 0;     // max_depth - min_depth

        // Averages
        double average_insertion_time_ms = 0.0;
        double average_comparisons_insertion = 0.0;
        double average_search_time_ms = 0.0;
        double average_comparisons_search = 0.0;
    };

    
    /**
     * @brief Creates a new binary search tree.
     *
     * @returns the tree
     */
    BinaryTree* createTree();

    /**
     * @brief Creates a new node of a tree.
     *
     * @param word: word to be put in the node.
     * @param documentIds: ID of the document in which the word appears.
     * @param color: color of the Node. 0 for black and 1 for red.
     
     * @returns Node*: Node with the passed word and documentIds
     */
   Node* createNode(std::string word,std::vector<int>documentIds,int color = 0);

   
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
   SearchResult search(BinaryTree* binary_tree, const std::string& word);
   
	
   void deletionPostOrder(Node* n);

   
   /**
    * @brief Delete the passed binary tree
    *
    * @param binary_tree: BinaryTree you want to delete.
    */
   void destroy(BinaryTree* binary_tree);
   
   /**
    * @brief Calculates the height of a binary tree.
    *
    * @param binary_tree: BinaryTree you wish to know the height.BinaryTree.
    * @return height of binary_tree.
    *
    */
    int calculateHeight(Node* root);

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
   void updateHeightUp(Node* node);

   /**
    * @brief Saves the aggregate statistics to a CSV file.
    *
    * This function writes the statistical data contained in the AggregateStats
    * object into a CSV file. The CSV will contain a header row followed by
    * a single row with the corresponding values.
    *
    * @param stats The AggregateStats object containing the data to save.
    * @param filename The name of the CSV file to write to "results.csv".
    */
    void save_stats_to_csv(const AggregateStats& stats, const std::string& filename = "results.csv");

    /**
     * @brief Calculates the minimum depth of a binary tree.
     *
     * This function recursively calculates the minimum depth from the root
     * to the nearest leaf node (node with no children).
     *
     * @param root Pointer to the root node of the tree.
     * @return The minimum depth of the tree. Returns 0 if the tree is empty.
     */
    int calculateMinDepth(Node* root);

    /**
     * @brief Counts the total number of nodes in a binary tree.
     *
     * This function traverses the tree recursively and counts each node.
     *
     * @param root Pointer to the root node of the tree.
     * @return The total number of nodes in the tree.
     */
    int countNodes(Node* root);

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
    double getAverageInsertionTime(const AggregateStats& stats);

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
    double getAverageComparisonsPerInsertion(const AggregateStats& stats);
    
    /**
     * @brief Calculates the average search time.
     *
     * This function computes the average time taken for searches
     * based on the total sum of search times and the total number of searches.
     *
     * @param stats The AggregateStats object containing search time data.
     * @return The average search time, or 0.0 if no searches were performed.
     */
    double getAverageSearchTime(const AggregateStats& stats);

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
    double getAverageComparisonsPerSearch(const AggregateStats& stats);

    /**
     * @brief Calculates the relative balance of the tree.
     *
     * @param stats The AggregateStats object containing tree structure data.
     * @return The relative balance of the tree, or 0.0 if the minimum depth is 0.
     */
    double getRelativeBalance(const AggregateStats& stats);
    
    /**
     * @brief Calculates the difference between the height of the tree and minimum depth.
     *
     * @param stats The AggregateStats object containing tree structure data.
     * @return The difference between the height of the tree and minimum depth.
     */
    int getBalanceDifference(const AggregateStats& stats);

    /**
     * @brief Updates all aggregate statistics for the given binary tree.
     *
     * This function update functions to compute the final node count, tree height, minimum depth
     * relative balance, difference balance, and all averages
     * (insertion times, comparisons per insertion, search times, and comparisons per search)
     * and stores them in the AggregateStats object.
     *
     * @param stats Reference to the AggregateStats object to update.
     * @param tree Pointer to the BinaryTree whose statistics are being calculated.
     */
    void updateAllAggregateStats(AggregateStats& stats, BinaryTree* tree);


    /**
     * @brief Performs a standard left rotation on the subtree rooted at node x.
     *
     * The right child `y` of a node becomes the new root of this subtree, and the original pivot `x`
     * becomes the left child of `y`. The Binary Search Tree property is maintained.
     *
     * @param root A double pointer to the root of the entire tree. This is used to
     * update the tree's root if the rotation occurs at the top level.
     * @param x A pointer to the node that will be the pivot of the rotation. This
     * node moves down to become the left child of its original right child.
     *
     * @note This function only manipulates pointers (`left`, `right`, `parent`).
     * It does NOT update node properties like height or color. The calling
     * function (e.g., in `avl.cpp` or `rbt.cpp`) is responsible for that.
     */
    void rotateLeft(Node** root, Node* x);

    /**
     * @brief Performs a standard right rotation on the subtree rooted at node y.
     *
     * A right rotation is the mirror operation of a left rotation.
     * The left child `x` of a node becomes the new root of this subtree, and the original pivot `y`
     * becomes the right child of `x`. The Binary Search Tree property is maintained.
     *
     * @param root A double pointer to the root of the entire tree. This is used to
     * update the tree's root if the rotation occurs at the top level.
     * @param y A pointer to the node that will be the pivot of the rotation. This
     * node moves down to become the right child of its original left child.
     *
     * @note This function only manipulates pointers (`left`, `right`, `parent`).
     * It does NOT update node properties like height or color. The calling
     * function (e.g., in `avl.cpp` or `rbt.cpp`) is responsible for that.
     */
    void rotateRight(Node** root, Node* y);
}

#endif
