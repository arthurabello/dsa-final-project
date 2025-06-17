#include <chrono>
#include <iostream>
#include <string>

#include "rbt.h"
#include "../cli.h"
#include "../tree_utils.h"

using namespace std;
using namespace chrono;

static const string usage = "./rbt <search|stats> <n_docs> <directory>";

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

    TREE::BinaryTree* rbt = TREE::createTree();

    CLI::VisualizationStats stats;
    stats.wordsIndexed = 0;
    int error = CLI::indexFilesInDir(
        directory, numFiles, &stats.docsIndexed,
        [&](string word, int documentId) {
            stats.wordsIndexed++;

            auto startTime = high_resolution_clock().now();

            auto ins = TREE::RBT::insert(rbt, word, documentId);

            auto endTime = high_resolution_clock().now();
            auto duration = duration_cast<nanoseconds>(endTime - startTime);
            stats.insertTimes.push_back((endTime - startTime).count());

            return ins;
        }
    );
    if (error != 0) return error;

    if (command == "stats") {
        CLI::testSearch(rbt, &stats, directory);

        TREE::AggregateStats aggStats = CLI::collectAggStats(rbt, &stats);
        CLI::saveAsCsv(aggStats, "rbt.csv");
        
        CLI::startViewServer(rbt, stats);
    }
    else if (command == "search") {
        CLI::searchFiles(rbt);
    }
    else {
        cerr << "Error: Unkowned command: " << command << "."
            << "Use " << usage << endl;

        TREE::destroy(rbt);
        return 1;
    }

    cout << endl << "Deallocating tree..." << endl;

    TREE::destroy(rbt);
    return 0;
}
