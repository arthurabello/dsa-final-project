#ifndef CLI_H
#define CLI_H

#include <functional>
#include <string>
#include <vector>

#include "tree_utils.h"

namespace CLI {
    // Used inside 'colorize'
    namespace color {
        constexpr const char* reset   = "\033[0m";
        constexpr const char* bold    = "\033[1m";
        constexpr const char* red     = "\033[31m";
        constexpr const char* green   = "\033[32m";
        constexpr const char* yellow  = "\033[33m";
        constexpr const char* blue    = "\033[34m";
        constexpr const char* magenta = "\033[35m";
        constexpr const char* cyan    = "\033[36m";
    }

    struct VisualizationStats {
        std::vector<float> searchTimes;
        std::vector<float> insertTimes;
        std::vector<int> numComparisons;
        int docsIndexed;
        int wordsIndexed;
    };

    /**
     * @brief Converts a double into a string given a number of decimal places.
     *
     * @param num Number to convert
     * @param decimalPlaces Decimal places to use
     * @returns Converted number with the given decimal places
     */
    std::string formatDouble(double num, int decimalPlaces);

    /**
     * @brief Adds color information around a string.
     *
     * @param c Color character to use
     * @param str String to colorize
     * @returns String prefixed with the color and suffixed with a 'reset' char
     */
    std::string colorize(const char* c, std::string str);

    /**
     * @brief Starts a server that allows for tree visualization.
     *
     * @param tree Binary tree to be visualized
     */
    void startViewServer(TREE::BinaryTree* tree, VisualizationStats stats);

    /**
     * @brief Starts an interactive file search in the given binary search tree.
     *
     * @param tree BST to be used for searching
     */
    void searchFiles(TREE::BinaryTree* tree);

    /**
     * @brief Starts an interactive file search in the given binary search tree.
     *
     * @param tree BST to be used for searching
     */
    void printAndExportStats(TREE::AggregateStats stats);

    /**
     * @brief Checks if a given string is a valid number of files.
     *
     * A valid string should be a parseable positive integer.
     *
     * @param arg String containing the number of files
     * @returns The parsed number of files, if valid, else a negative int
     */
    int validateNumFiles(std::string arg);

    /**
     * @brief Indexes all data files in a directory with a given indexing callback.
     *
     * The words/docs must be added to the tree inside the 'callback' parameter
     *
     * @param directory Path to the directory to index
     * @param numFiles How many files should be indexed, at maximum
     * @param error Will be set to 1 if there occurs an error in the indexing
     * @param callback Function to be called for every word in every file
     * @returns 0 if successful, a positive integer otherwise
     */
    int indexFilesInDir(
        const std::string& directory,
        int numFiles,
        int *docsIndexed,
        std::function<TREE::InsertResult(std::string, int)> callback
    );

    /**
     * Searches the tree for words from the last documents in a directory as a
     * way to approximate search times and number of comparisons during search.
     *
     * @param tree Tree to be searched
     * @param stats Statistics used for visualization, where the times and
     * number of comparisons will be stored
     * @param directory Where to look for the words to search for
     */
    void testSearch(TREE::BinaryTree *tree, VisualizationStats *stats, std::string directory);

    /**
     * @brief Stores an AggregateStats object in a csv
     *
     * @param stats Statistics to be stored
     * @param filename Name of the file to store the data at
     */
    void saveAsCsv(TREE::AggregateStats stats, std::string filename);

    /**
     * @brief Collects aggregate statistics for a tree from visualization data
     *
     * @param tree Tree from which the statistics are being collected
     * @param stats Statistics used for visualization, from which the aggregate
     * stats object will be constructed
     */
    TREE::AggregateStats collectAggStats(TREE::BinaryTree *tree, VisualizationStats *stats);
}

#endif
