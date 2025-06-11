#ifndef CLI_H
#define CLI_H

#include <functional>
#include <string>

#include "tree_utils.h"

namespace CLI {
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

    std::string formatDouble(double num, int decimalPlaces);

    std::string colorize(const char* c, std::string str);

    void startViewServer(TREE::BinaryTree* tree);

    void searchFiles(TREE::BinaryTree* tree);

    void printAndExportStats(TREE::AggregateStats stats);

    int validateNumFiles(std::string arg);

    TREE::AggregateStats indexFilesInDir(
        const std::string& directory,
        int numFiles,
        int *error,
        std::function<TREE::InsertResult(std::string, int)> callback
    );
}

#endif
