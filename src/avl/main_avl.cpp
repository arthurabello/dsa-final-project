#include <iostream>
#include <string>

#include "avl.h"
#include "../cli.h"
#include "../tree_utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: ./avl <search|stats|view> <n_docs> <directory>" << endl;
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

    TREE::BinaryTree* avl_tree = TREE::createTree();

    int error;
    auto stats = CLI::indexFilesInDir(
        directory, numFiles, &error,
        [&](string word, int documentId) {
            return TREE::AVL::insert(*avl_tree, word, documentId);
        }
    );
    if (error) return 1;

    stats.final_node_count  = TREE::countNodes(avl_tree->root);
    stats.final_tree_height = TREE::calculateHeight(avl_tree->root);
    stats.final_tree_min_depth = TREE::calculateMinDepth(avl_tree->root);
    stats.final_tree_min_depth = TREE::calculateMinDepth(avl_tree->root);
    // stats.total_search_time_ms = 

    stats.tree_type = "AVL";

    if (command == "stats") {
        CLI::printAndExportStats(stats);
    }
    else if (command == "search") {
        CLI::searchFiles(avl_tree);
    }
    else if (command == "view") {
        CLI::startViewServer(avl_tree);
    }
    else {
        cerr << "Error: Unkowned command: " << command << "."
                  << "Use 'search', 'view' or 'stats'." << endl;

        TREE::destroy(avl_tree);
        return 1;
    }

    cout << endl << "Deallocating tree…" << endl;

    TREE::destroy(avl_tree);
    return 0;
}
