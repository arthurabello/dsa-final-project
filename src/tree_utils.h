#ifndef TREEUTILS_H  
#define TREEUTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>  

namespace TREE {

/**
 * @struct Node
 * @brief  A node of a *binary* search tree (plain BST, AVL or RBT).
 *
 * This is an intentionally minimal container; balancing metadata such
 * as colour (`isRed`) and height are included so that specialised trees
 * (e.g. Red-Black or AVL) do not need to extend the structure.
 */
struct Node {
    std::string      word;        //!< normalised term stored in the node
    std::vector<int> documentIds; //!< IDs of documents containing @p word

    Node* parent   = nullptr;     //!< up-link (for rotations & re-balancing)
    Node* left     = nullptr;     //!< left  child
    Node* right    = nullptr;     //!< right child

    int   height   = 1;           //!< AVL convenience field (root=1, leaf=1)
    int   isRed    = 0;           //!< Red-Black colour flag (0 = RED, 1 = BLACK)
};

/**
 * @struct BinaryTree
 * @brief  Thin wrapper around the root pointer so we can pass trees by
 *         pointer/reference and still mutate the root inside helpers.
 */
struct BinaryTree {
    Node* root = nullptr;         //!< root node (may be @c nullptr)
};

/**
 * @struct InsertResult
 * @brief  Metrics returned by an **insertion** operation.
 */
struct InsertResult {
    int    numComparisons = 0;  //!< key comparisons performed
    double executionTime  = 0;  //!< duration in **milliseconds**
};

/**
 * @struct SearchResult
 * @brief  Return type of the **search** routine.
 */
struct SearchResult {
    int              found           = 0;  //!< 1 if key found, 0 otherwise
    std::vector<int> documentIds;          //!< copy of posting list (may be empty)
    double           executionTime   = 0;  //!< time in **milliseconds**
    int              numComparisons  = 0;  //!< node comparisons performed
};

/**
 * @struct AggregateStats
 * @brief  Aggregate performance profile collected during **batch** indexing
 *         and querying.  One instance corresponds to a *whole run* over a
 *         corpus using a single tree type.
 *
 */
struct AggregateStats {
    std::string tree_type;  //!< "BST", "AVL", "RBT", …

    int         num_docs_indexed = 0;   //!< #documents parsed

    /* ───────────── Insertion phase ───────────── */
    long long   total_indexing_time_ms    = 0;  //!< wall-clock time (ms)
    long long   total_words_processed     = 0;  //!< sum(tokens)
    long long   total_comparisons_insertion = 0;
    double      sum_of_insertion_times_ms = 0.0;
    double      max_insertion_time_ms     = 0.0;

    /* ───────────── Search phase ─────────────── */
    double      total_search_time_ms      = 0.0;
    long long   total_searches            = 0;
    long long   total_comparisons_search  = 0;
    double      sum_of_search_times_ms    = 0.0;
    double      max_search_time_ms        = 0.0;

    /* ───────────── Final tree shape ─────────── */
    int         final_node_count   = 0;   //!< unique words (nodes)
    int         final_tree_height  = 0;   //!< max depth
    int         final_tree_min_depth = 0; //!< min depth

    /* ───────────── Derived averages ─────────── */
    double      average_insertion_time_ms       = 0.0;
    double      average_comparisons_insertion   = 0.0;
    double      average_search_time_ms          = 0.0;
    double      average_comparisons_search      = 0.0;
};

// ───────────────────────────── Factory helpers ─────────────────────────── //

/**
 * @brief Allocate an *empty* tree (root = @c nullptr).
 * @return Pointer to the new @c BinaryTree.
 */
BinaryTree* createTree();

/**
 * @brief  Allocate and initialise a single @c Node.
 *
 * @param word         normalised key to store.
 * @param documentIds  list of documents where @p word appears.
 * @param color        colour flag for Red-Black trees (0 = RED, 1 = BLACK).
 *                     for AVL/BST variants this value is ignored.
 * @return pointer to the newly-created node.
 */
Node* createNode(std::string word,
                 std::vector<int> documentIds,
                 int color = 0);

// ───────────────────────────── Core operations ─────────────────────────── //

/**
 * @brief search for @p word in @p binary_tree.
 *
 * runs an iterative tree traversal recording elapsed time and the number
 * of key comparisons.  Returns a populated #SearchResult struct.
 */
SearchResult search(BinaryTree* binary_tree, const std::string& word);

/**
 * @brief recursively free **all** nodes in @p binary_tree and the wrapper
 *        itself.  safe to call with an empty tree.
 *
 * @warning pointers returned by previous operations become invalid.
 */
void destroy(BinaryTree* binary_tree);

// ───────────────────────────── Structural queries ──────────────────────── //

/**
 * @brief compute the **height** of the tree rooted at @p root.
 *
 * height is defined as *the number of nodes on the longest path from
 * @p root down to a leaf*.  an empty subtree has height 0.
 */
int calculateHeight(Node* root);

/**
 * @brief walk upwards from @p node, updating the @c height field of each
 *        ancestor until it stabilises.
 *
 * assumes that children heights are already correct (i.e. call it right
 * after inserting a node without structural re-balancing).
 */
void updateHeightUp(Node* node);

/**
 * @brief persist @p stats to @p filename in **CSV** format (single row +
 *        header).  existing files are overwritten.
 */
void save_stats_to_csv(const AggregateStats& stats,
                       const std::string&   filename = "results.csv");

// ───────────────────────────── Depth & count helpers ───────────────────── //

/**
 * @brief minimum depth (= shortest path root→leaf) of the subtree rooted at
 *        @p root.  empty tree ⇒ 0.
 */
int calculateMinDepth(Node* root);

/**
 * @brief total number of nodes reachable from @p root (recursive count).
 */
int countNodes(Node* root);

// ───────────────────────────── Averages (derived) ──────────────────────── //

/** @name average helpers  
 *  convenience wrappers around raw totals collected inside #AggregateStats. */
/**@{*/
double getAverageInsertionTime        (const AggregateStats& stats);
double getAverageComparisonsPerInsertion(const AggregateStats& stats);
double getAverageSearchTime           (const AggregateStats& stats);
double getAverageComparisonsPerSearch (const AggregateStats& stats);
/**@}*/

// ───────────────────────────── Aggregate updaters ─────────────────────── //

/**
 * @brief refresh the *final* node count field of @p stats using @p tree.
 */
void updateFinalNodeCount(AggregateStats& stats, BinaryTree* tree);

/**
 * @brief refresh the *final* height field of @p stats using @p tree.
 */
void updateFinalTreeHeight(AggregateStats& stats, BinaryTree* tree);

/**
 * @brief refresh the *final* minimum depth field of @p stats using @p tree.
 */
void updateFinalTreeMinDepth(AggregateStats& stats, BinaryTree* tree);

/**
 * @brief compute the average insertion time (ms) and store in
 *        @c stats.average_insertion_time_ms.
 */
void updateAverageInsertionTime(AggregateStats& stats);

/**
 * @brief compute the average number of comparisons per insertion and store
 *        in @c stats.average_comparisons_insertion.
 */
void updateAverageComparisonsPerInsertion(AggregateStats& stats);

/**
 * @brief compute the average search time (ms) and store in
 *        @c stats.average_search_time_ms.
 */
void updateAverageSearchTime(AggregateStats& stats);

/**
 * @brief compute the average number of comparisons per search and store
 *        in @c stats.average_comparisons_search.
 */
void updateAverageComparisonsPerSearch(AggregateStats& stats);

/**
 * @brief convenience wrapper that calls **all** individual updater
 *        functions above so that @p stats holds a complete, coherent
 *        snapshot of the current state of @p tree.
 */
void updateAllAggregateStats(AggregateStats& stats, BinaryTree* tree);

} 
#endif 
