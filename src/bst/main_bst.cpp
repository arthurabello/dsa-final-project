#include <chrono>
#include <iostream>
#include <string>

#include "bst.h"
#include "../cli.h"
#include "../tree_utils.h"

using namespace std;
using namespace chrono;

static const string usage = "./bst <search|stats|view> <n_docs> <directory>";

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << usage << endl;
        return 1;
    }

    const string command     { argv[1] };
    const string numFilesStr { argv[2] };
    const string directory   { argv[3] };

    int numFiles = CLI::validateNumFiles(numFilesStr);
    if (numFiles < 0) return 1;

    cout << CLI::colorize(CLI::color::bold, "Command: ") << command << ", "
        << CLI::colorize(CLI::color::bold, "Documents: ") << numFiles << ", "
        << CLI::colorize(CLI::color::bold, "Directory: ") << directory << endl;

    TREE::BinaryTree* bst = TREE::createTree();

    CLI::VisualizationStats stats;
    stats.wordsIndexed = 0;
    int error = CLI::indexFilesInDir(
        directory, numFiles, &stats.docsIndexed,
        [&](string word, int documentId) {
            stats.wordsIndexed++;

            auto startTime = high_resolution_clock().now();

            auto ins = TREE::BST::insert(bst, word, documentId);

            auto endTime = high_resolution_clock().now();
            auto duration = duration_cast<nanoseconds>(endTime - startTime);
            stats.insertTimes.push_back((endTime - startTime).count());

            return ins;
        }
    );
    if (error != 0) return error;

    if (command == "stats") {
        CLI::testSearch(bst, &stats, directory);

        TREE::AggregateStats aggStats = CLI::collectAggStats(bst, &stats);
        CLI::saveAsCsv(aggStats, "bst.csv");
    }
    else if (command == "search") {
        CLI::searchFiles(bst);
    }
    else if (command == "view") {
        CLI::testSearch(bst, &stats, directory);
        CLI::startViewServer(bst, stats);
    }
    else {
        cerr << "Error: Unkowned command: " << command << "."
            << "Use " << usage << endl;

        TREE::destroy(bst);
        return 1;
    }

    cout << endl << "Deallocating tree..." << endl;

    TREE::destroy(bst);
    return 0;
}
