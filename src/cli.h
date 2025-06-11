#ifndef CLI_H
#define CLI_H

#include <functional>
#include <string>

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
    void startViewServer(TREE::BinaryTree* tree);

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
     * @returns Indexing statistics
     */
    TREE::AggregateStats indexFilesInDir(
        const std::string& directory,
        int numFiles,
        int *error,
        std::function<TREE::InsertResult(std::string, int)> callback
    );
}

#endif
